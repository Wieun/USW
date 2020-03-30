    /**
     * A particle is the simplest object that can be simulated in the
     * physics system.

     */

#ifndef PARTICLE_H
#define PARTICLE_H

#include "coreMath.h"

class Particle {
protected:
    float inverseMass, damping, radius;
    Vector2 position, velocity, forceAccum, acceleration;
public:
    bool hasFiniteMass() const;
    
    void clearAccumulator()                           ,
         setRadius(const float r)                     ,
         integrate(float duration)                    ,
         setMass(const float mass)                    ,
         addForce(const Vector2 &force)               ,
         setDamping(const float damping)              ,
         setPosition(const Vector2 &position)         ,
         getPosition(Vector2 *position) const         ,
         setVelocity(const Vector2 &velocity)         ,
         getVelocity(Vector2 *velocity) const         ,
         setInverseMass(const float inverseMass)      ,
         setPosition(const float x, const float y)    ,
         setVelocity(const float x, const float y)    ,
         setAcceleration(const Vector2 &acceleration) ,
         setAcceleration(const float x, const float y);
         
    float getMass() const       ,
          getDamping() const    ,
          getRadius() const     ,
          getInverseMass() const;
         
    Vector2 getPosition() const    , 
            getVelocity() const    , 
            getAcceleration() const;
};
#endif // 
