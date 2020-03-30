#include <list>
#include <vector>
#include "quadtree.h"
#include "particle.h"
#include "pcontacts.h"

class ParticleCollision : public ParticleContactGenerator {
private:
    Vector2 screenSize;
    int particleCount;
    Quadtree* rootNode = nullptr;
    bool checkCollision(Particle* particle1, Particle* particle2, float distance) const;
public:
    ParticleCollision(Particle* particles, Vector2 size, int numOfP);
    ~ParticleCollision();
    Particle* particles;
    void update(), makeQuadtree(), createTree();
    virtual unsigned addContact(ParticleContact* contact, unsigned limit) const;
};
