#include "particle_collision.h"

ParticleCollision::ParticleCollision(Particle* particle, Vector2 size, int noOfParticles) {
    this->screenSize = size;
    this->particles = particle;
    this->particleCount = noOfParticles;
    createTree();

}
ParticleCollision::~ParticleCollision() {
    delete this;
}  

void ParticleCollision::createTree() {
    if (rootNode != nullptr) delete rootNode;
    rootNode = new Quadtree(0, 0, screenSize.x, screenSize.y, 0, 0);
    for (int i = 0; i < particleCount; i++) { rootNode->insert(&particles[i]);}
}

void ParticleCollision::update() {
    createTree();
}


unsigned ParticleCollision::addContact(ParticleContact * contact, unsigned limit) const {
    const static float restitution = 1.0f;
    unsigned used = 0;
    vector<vector<Particle*>> treeNode;
    treeNode = rootNode->getObjects(treeNode);
    // Cycles through the tree nodes
    for (vector<vector<Particle*>>::iterator i = treeNode.begin(); i != prev(treeNode.end()); i++) {
        // check if there is at least 2 particles in the curerent tree node
        if ((*i).size() >= 2) {
            // Checks particle j against other particles in the particle array
            // that are in the same tree node
            for (vector<Particle*>::iterator j = (*i).begin(); j != prev((*i).end()); j++) {
                if (used >= limit) return used;
                for (vector<Particle*>::iterator k = next(j); k != (*i).end(); k++) {
                    Vector2 positionOne = (*j)->getPosition(), positionTwo = (*k)->getPosition(), distanceVector = positionOne - positionTwo;
                    float radius1 = (*j)->getRadius(), radius2 = (*k)->getRadius(), distance = distanceVector.magnitude();
                    // Checks if the particles have penetrated
                    if (checkCollision((*j), (*k), distance)) {
                        // if they have penetrated then 
                        // simulate collision and separation
                        contact->contactNormal =  distanceVector.unit();
                        contact->restitution = restitution;
                        contact->particle[0] = (*j);
                        contact->particle[1] = (*k);
                        contact->penetration = (radius1 + radius2) - distance;
                        used++;
                        contact++;
                    }
                }
            }
        }
    }
    return used;
}

void ParticleCollision::makeQuadtree() {
    rootNode->show();
}

bool ParticleCollision::checkCollision(Particle* particle1, Particle* particle2, float distance) const {
    if (distance  <= particle1->getRadius() + particle2->getRadius()) {
        return true;
    }
    else {
        return false;
    }
}
