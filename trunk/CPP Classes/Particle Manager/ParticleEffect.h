/*
 *  ParticleEffect.h
 *  DjinnEngine
 *
 *  Created by Craig Giles on 6/17/09.
 *
 */

//	Particle Effect:
//	the particle effect houses various particle emitters to achieve the desired effect.
//	for example, when going for a fire effect in my test, I noticed that only one emitter
//	was not making for 'realistic fire' and thus, I attempted to put two emitters together,
//	one that emitted a flame texture, while the other emitted a basic fire "flow." These
//	two emitters working togehter achieved the effect I was going for, and therefore it
//	was required for me to house several emitters into one particle effect for the game.
//
//	Particle Effect Serialized Data Example:
//	Asset Name:					FIRE
//	Number of Emitters			2
//	Emitter Asset Names:		FLAME_BACK
//								FLAME_FRONT

#ifndef PARTICLEEFFECT_H
#define PARTICLEEFFECT_H

#include "Serializable.h"
#include "ParticleEmitter.h"
#include "FileUtil.h"

#include <iostream>

class ParticleEffect : public Serializable
{
public:
	//	Overloaded >> and << operators used for serialization and deserialization
	friend std::istream& operator >>( std::istream &inStream, ParticleEffect &value );
	friend std::ostream& operator <<( std::ostream &outStream, ParticleEffect &value );
	ParticleEffect& operator =( const ParticleEffect &copy );
	
	ParticleEffect();
	ParticleEffect( const ParticleEffect &copy );
	~ParticleEffect();

	//	Is the effect still active, or has it run its course.
	bool isActive() const { return active; }
	
	//	allocates memory for the particle emitters used to create this
	void loadParticleEmitters( const std::string *assets, const int numOfEmitters );
	
	//	Activates the effect, cycling through each particle emitter associated with
	//	the effect, and setting its starting location / variance / duration to the 
	//	specified parameters. NOTE: Duration of -1 is infinite. 
	void startParticleEffect( const Vector2 location, const Vector2 variance, const float duration );

	//	Stops the particle effect, labling it as 'active = false' so the controller
	//	konws it can be deallocated from memory.
	void stopParticleEffect();
	
	//	Handle any update and draw logic for the effect, cycling through each particle
	//	emitter and updating and drawing them as well.
	void update( const float deltaTime );
	void draw( const float deltaTime );
	
private:
	int numOfEmitters;
	std::string *emitterAssets;
	ParticleEmitter *emitters;

	bool active;
};

#endif