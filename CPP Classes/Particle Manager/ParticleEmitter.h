/*
 *  ParticleEmitter.h
 *  DjinnEngine
 *
 *  Created by Craig Giles on 6/15/09.
 *
 */

//	Particle Emitter Class:
//	The particle emitter is like the stick of a sparkler... Each sparkle
//	that flys out is a particle, but the emitter is the source of that particle.
//	Particle emitters can be amazingly complex, but I feel this version of a 
//	particle emitter isn't too terrible. 
//
//	Once again, I used Mike's tutorials as a starting point for this emitter class,
//	Mike's tutorials can be found on his blog at: http://www.71squared.co.uk/
//
//	Particle Emitter Serialization Guide Example:
//	Texture Asset				particle
//	Angle (-90 is up)			-90
//	Angle Variance				18
//	Speed						0.2
//	Speed Variance				0
//	Gravity						0	-0.01
//	Lifespan					0.6
//	Lifespan Variance			0.6
//	
//	Starting Color				1.0	0.3	0.1	1.0
//	Starting Color Variance		0.0	0.0	0.0	0.0
//	Ending Color				1.0	0.3	0.0	0.0
//	Ending Color Variance		0.0	0.0	0.0	0.0
//	
//	Particle Size				24
//	Particle Size Variance		12
//	Max Particles				20
//	Emitter Duration			-1
//	Blend Additive				0

#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "Serializable.h"
#include "TextureController.h"
#include "Texture2D.h"
#include "Vector2.h"
#include "RNG.h"
#include "MathUtil.h"
#include "Particle.h"
#include "Color.h"

#include <string>
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

class ParticleEmitter : public Serializable
{
public:
	friend std::istream& operator >> ( std::istream &inStream, ParticleEmitter &value );
	
	ParticleEmitter();
	
	void setEmitterParameters( const std::string textureName,
					const GLfloat angle,
					const GLfloat angleVariance,
					const GLfloat speed,
					const GLfloat speedVariance,
					const Vector2 gravity,
					const GLfloat lifespan,
					const GLfloat lifespanVariance,
					const Color startColor,
					const Color startColorVariance,
					const Color endColor,
					const Color endColorVariance,
					const GLfloat size,
					const GLfloat sizeVariance,
					const GLuint maxParticles,
					const bool blendAdditive);
	
	void setTextureName( const std::string textureName );
	void setSourceLocation( const Vector2 source ) { this->source = source; }
	void setSourceVariance( const Vector2 sourceVariance ) { this->sourceVariance = sourceVariance; }
	void setAngle( const GLfloat angle ) { this->angle = angle; }
	void setAngleVariance( const GLfloat angleVariance ) { this->angleVariance = angleVariance; }
	void setSpeed( const GLfloat speed ) { this->speed = speed; }
	void setSpeedVariance( const GLfloat speedVariance ) { this->speedVariance = speedVariance; }
	void setGravity( const Vector2 gravity ) { this->gravity = gravity; }
	void setLifespan( const GLfloat lifespan ) { this->lifespan = lifespan; }
	void setLifespanVariance( const GLfloat lifespanVariance ) { this->lifespanVariance = lifespanVariance; }
	void setStartColor( const Color startColor ) { this->startColor = startColor; }
	void setStartColorVariance( const Color startColorVariance ) { this->startColorVariance = startColorVariance; }
	void setEndColor( const Color endColor ) { this->endColor = endColor; }
	void setEndColorVariance( const Color endColorVariance ) { this->endColorVariance = endColorVariance; }
	void setSize( const GLfloat size ) { this->size = size; }
	void setSizeVariance( const GLfloat sizeVariance ) { this->sizeVariance = sizeVariance; }
	void setMaxParticles( const GLuint maxParticles ) { this->maxParticles = maxParticles; }
	void setEffectDuration( const GLfloat duration ) { this->duration = duration; }
	void setBlendAdditive( const bool blendAdditive ) { this->blendAdditive = blendAdditive; }
	
	~ParticleEmitter(); 
	
	bool isActive() const { return active; }
	
	void update( const float deltaTime );
	void draw( const float deltaTime );
	
	void startParticleEmitter( const Vector2 &source, const Vector2 &sourceVariance, const GLfloat duration );
	void stopParticleEmitter();
private:
	ParticleEmitter( const ParticleEmitter &copy ) { }
	
	Texture2D *texture;			//Texture used by every particle in this emitter.
	Vector2 source;
	Vector2 sourceVariance;
	GLfloat angle;
	GLfloat angleVariance;
	GLfloat speed;
	GLfloat speedVariance;
	Vector2 gravity;
	GLfloat lifespan;
	GLfloat lifespanVariance;
	Color startColor;
	Color startColorVariance;
	Color endColor;
	Color endColorVariance;
	GLfloat size;
	GLfloat sizeVariance;
	GLuint maxParticles;
	GLint particleCount;
	GLfloat emissionRate;
	GLfloat emitCounter;
	GLuint verticesID;
	GLuint colorsID;
	
	Particle *particles;
	PointSprite *vertices;
	Color *colors;
	
	bool active;
	bool stopEmitting;
	GLint particleIndex;
	GLfloat elapsedTime;
	GLfloat duration;
	bool blendAdditive;
	
	bool addParticle();
	void initParticle( Particle &particle );
	
	//	called from the update function: Checks to see if enough time has
	//	passed for a new particle to be emitted from the particle emitter.
	void checkForNewParticles( const float deltaTime);
	
	//	called from the update function: Updates all active particles as 
	//	well as updates the vertices and color arrays in preperation for
	//	OpenGL draw calls. Will check to see if a particle has died, and if
	//	a particle has died, it will adjust the array to reflect the death
	//	of the brave little soldier. 
	void updateParticles( const float deltaTime );
};

#endif