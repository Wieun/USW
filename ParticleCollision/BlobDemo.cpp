/*
 * The Blob demo.
 *
 */
#include <GL/glut.h>
#include <stdio.h>
#include <cassert>
#include "app.h"
#include "coreMath.h"
#include "pcontacts.h"
#include "pworld.h"


#include "quadtree.h"
#include "particle_collision.h"

#define PLATFORM_COUNT 4
#define BLOB_COUNT     20
const Vector2 Vector2::GRAVITY = Vector2(0, -9.81);

/**
 * Platforms are two dimensional: lines on which the
 * particles can rest. Platforms are also contact generators for the physics.
 */

class Platform : public ParticleContactGenerator {
public:
    Vector2 start;
    Vector2 end;
    float restitution;
    /**
     * Holds a pointer to the particles we're checking for collisions with.
    */
    Particle *particles;
    virtual unsigned addContact(
        ParticleContact *contact, unsigned limit) const;
};

unsigned Platform::addContact(ParticleContact *contact, unsigned limit) const
{
	//const static float restitution = 0.8f;
	const static float restitution = 1.0f;
	unsigned used = 0;
	unsigned i = 0;
	for (unsigned i = 0; i < BLOB_COUNT; i++)
	{
		if (used >= limit) return used;
		// Check for penetration
		Vector2 toParticle = particles[i].getPosition() - start;
		Vector2 lineDirection = end - start;

		float projected = toParticle * lineDirection;
		float platformSqLength = lineDirection.squareMagnitude();
		float squareRadius = particles[i].getRadius()*particles[i].getRadius();;

		if (projected <= 0)
		{

			// The blob is nearest to the start point
			if (toParticle.squareMagnitude() < squareRadius)
			{
				// We have a collision
				contact->contactNormal = toParticle.unit();
				contact->restitution = restitution;
				contact->particle[0] = particles + i;
				contact->particle[1] = 0;
				contact->penetration = particles[i].getRadius() - toParticle.magnitude();
				used++;
				contact++;
			}

		}
		else if (projected >= platformSqLength)
		{
			// The blob is nearest to the end point
			toParticle = particles[i].getPosition() - end;
			if (toParticle.squareMagnitude() < squareRadius)
			{
				// We have a collision
				contact->contactNormal = toParticle.unit();
				contact->restitution = restitution;
				contact->particle[0] = particles + i;
				contact->particle[1] = 0;
				contact->penetration = particles[i].getRadius() - toParticle.magnitude();
				used++;
				contact++;
			}
		}
		else
		{
			// the blob is nearest to the middle.
			float distanceToPlatform = toParticle.squareMagnitude() - projected * projected / platformSqLength;
			if (distanceToPlatform < squareRadius)
			{
				// We have a collision
				Vector2 closestPoint = start + lineDirection * (projected / platformSqLength);

				contact->contactNormal = (particles[i].getPosition() - closestPoint).unit();
				contact->restitution = restitution;
				contact->particle[0] = particles + i;
				contact->particle[1] = 0;
				contact->penetration = particles[i].getRadius() - sqrt(distanceToPlatform);
				used++;
				contact++;
			}
		}
	}
	return used;
}


class BlobDemo : public Application
{
	Particle *blob;

	Platform *platform;

	ParticleWorld world;

    ParticleCollision *pCollision;
public:
	/** Creates a new demo object. */
	BlobDemo();
	virtual ~BlobDemo();

	/** Returns the window title for the demo. */
	virtual const char* getTitle();

	/** Display the particles. */
	virtual void display();

	/** Update the particle positions. */
	virtual void update();
};

// Method definitions
BlobDemo::BlobDemo() :world(1000, PLATFORM_COUNT) {
	width = 400; height = 400;
	nRange = 100.0;

	float margin = 1.0, standard_restitution = 1.0;
	// Create the blob storage
	blob = new Particle[BLOB_COUNT];

	// Create the platform
	platform = new Platform[PLATFORM_COUNT];

	platform[0].restitution = standard_restitution;
	platform[0].start = Vector2(-nRange * margin, -nRange * margin);
	platform[0].end = Vector2(nRange*margin, -nRange * margin);

	platform[1].restitution = standard_restitution;
	platform[1].start = Vector2(-nRange * margin, nRange*margin);
	platform[1].end = Vector2(nRange*margin, nRange*margin);

	platform[2].restitution = standard_restitution;
	platform[2].start = Vector2(-nRange * margin, -nRange * margin);
	platform[2].end = Vector2(-nRange * margin, nRange*margin);

	platform[3].restitution = standard_restitution;
	platform[3].start = Vector2(nRange*margin, -nRange * margin);
	platform[3].end = Vector2(nRange*margin, nRange*margin);

	// Make sure the platform knows which particle it should collide with.
	for (unsigned i = 0; i < PLATFORM_COUNT; i++) {
		// Make sure the platform knows which particles it should collide with
		platform[i].particles = blob;
		world.getContactGenerators().push_back(platform + i);
	}

	// Create the blob.
	for (unsigned i = 0; i < BLOB_COUNT; i++) {
		blob[i].setPosition(-90.0 + i * 10, 90.0);
		blob[i].setRadius(5);
		blob[i].setVelocity(0, 0);
		blob[i].setDamping(0.9);
		blob[i].setAcceleration(Vector2::GRAVITY * 5.0f * (i + 1));
		blob[i].setMass(100.0f);
		blob[i].clearAccumulator();

		world.getParticles().push_back(blob + i);
	}
	
	    pCollision = new ParticleCollision(blob, Vector2(nRange *2.0, nRange * 2.0), BLOB_COUNT);
		world.getContactGenerators().push_back(pCollision);
		pCollision->update(); 
}

BlobDemo::~BlobDemo() {
    delete blob;
}

void BlobDemo::display() {
	Application::display();

	glBegin(GL_LINES);
	glColor3f(0, 1, 1);

	for (unsigned i = 0; i < PLATFORM_COUNT; i++) {
		const Vector2 &p0 = platform[i].start;
		const Vector2 &p1 = platform[i].end;
		glVertex2f(p0.x, p0.y);
		glVertex2f(p1.x, p1.y);
	}
	glEnd();

	for (unsigned i = 0; i < BLOB_COUNT; i++) {
		glColor3f((i % 2) ? 0 : 1, (i % 2) ? 1 : 0, 0);
		const Vector2 &p = blob[i].getPosition();
		glPushMatrix();
		glTranslatef(p.x, p.y, 0);
		glutSolidSphere(blob[i].getRadius(), 12, 12);
		glPopMatrix();
	}
	pCollision->makeQuadtree();
	glutSwapBuffers();
}

void BlobDemo::update() {
	// Recenter the axes
	float duration = timeinterval / 1000;
	// Run the simulation
	world.runPhysics(duration);

    pCollision->update();

	Application::update();
}

const char* BlobDemo::getTitle()
{
	return "Blob Demo";
}

/**
 * Called by the common demo framework to create an application
 * object (with new) and return a pointer.
 */
Application* getApplication()
{
	return new BlobDemo();
}
