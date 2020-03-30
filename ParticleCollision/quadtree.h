#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include "particle.h"
#include <iostream>
#include <GL/glut.h>

using namespace std;

class Quadtree {
public:
    Quadtree(float x, float y, float width, float height, int level, int maxLevel);
    ~Quadtree();
    int size = 0;
    bool insert(Particle* particles);
    vector<vector<Particle*>> getObjects(vector<vector<Particle*>>);
    void show(); 
private:
    float x, y, width, height;
    int level, maxLevel = 0, maxNoOfParticlesBeforeSplit = 3;
    bool split = false, contains(Particle* object);;
    void splitQuadtree(); 
    vector<Particle*>objects; 
    Quadtree *NW = nullptr, *NE = nullptr, *SW = nullptr, *SE = nullptr;
};
#endif
