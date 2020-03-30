#include "quadtree.h"

Quadtree::Quadtree(float x, float y, float width, float height, int level, int maxLevel) {
    this->x        = x;
    this->y        = y;
    this->level    = level;
    this->width    = width;
    this->height   = height;
    this->maxLevel = maxLevel;
}

Quadtree::~Quadtree() {
	if(NW != nullptr) delete NW;
	if(NE != nullptr) delete NE;
	if(SW != nullptr) delete SW;
	if(SE != nullptr) delete SE;
}

bool Quadtree::insert(Particle* particles) {
    if (!contains(particles)) return false;
    // if the current node's capacity is less than the limit for splitting and
    // the max level of the tree hasnt been reached in node
    if (objects.capacity() < maxNoOfParticlesBeforeSplit && maxLevel <= 3) {
        // add particle to node
        objects.push_back(particles); return true;
        }
    else {
        if (!split && maxLevel <= 3) {
            splitQuadtree();
            for (int i = 0; i < maxNoOfParticlesBeforeSplit; i++) {
                NW->insert(objects[i]);
                NE->insert(objects[i]);
                SW->insert(objects[i]);
                SE->insert(objects[i]);
            }
            objects.clear();
            maxNoOfParticlesBeforeSplit = 0;
        }
        if (NW->insert(particles)) { NW->size++; return true; }
        if (NE->insert(particles)) { NE->size++; return true; }
        if (SW->insert(particles)) { SW->size++; return true; }
        if (SE->insert(particles)) { SE->size++; return true; }
        else return false;
    }
}

void Quadtree::splitQuadtree() {
    NW = new Quadtree(x - (width / 4), y + (height / 4), width / 2, height / 2, size, maxLevel);
    NE = new Quadtree(x + (width / 4), y + (height / 4), width / 2, height / 2, size, maxLevel);
    SW = new Quadtree(x - (width / 4), y - (height / 4), width / 2, height / 2, size, maxLevel);
    SE = new Quadtree(x + (width / 4), y - (height / 4), width / 2, height / 2, size, maxLevel);
    split = true;
}

vector<vector<Particle*>> Quadtree::getObjects(vector<vector<Particle*>> particleSectors) {
    if (split == true) {
        particleSectors = NW->getObjects(particleSectors);
        particleSectors = NE->getObjects(particleSectors);
        particleSectors = SW->getObjects(particleSectors);
        particleSectors = SE->getObjects(particleSectors);
        return particleSectors;
    }
    else {
        particleSectors.push_back(objects);
        return particleSectors;
    }
}

void Quadtree::show() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(1);
    glBegin(GL_LINES); // red
    glVertex2f(x - (width / 2), y);
    glVertex2f(x + (width / 2), y);
    glVertex2f(x, y - (height / 2));
    glVertex2f(x, y + (height / 2));
    glEnd();
    
    if (NW->NW != nullptr) NW->show();
    if (NE->NE != nullptr) NE->show();
    if (SW->SW != nullptr) SW->show();
    if (SE->SE != nullptr) SE->show();
}

bool Quadtree::contains(Particle* object) {
    if (object->getPosition().x + object->getRadius() >= x - (width / 2) && 
        object->getPosition().x - object->getRadius() <= x + (width / 2)) {
        if (object->getPosition().y + object->getRadius() >= y - (height / 2) && 
            object->getPosition().y - object->getRadius() <= y + (height / 2)) 
            return true;
    }
    return false;
}
