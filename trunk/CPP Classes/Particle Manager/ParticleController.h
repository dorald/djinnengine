/*
 *  ParticleController.h
 *  DjinnEngine
 *
 *  Created by Craig Giles on 6/15/09.
 *
 */

//	Particle Controller:
//	I had to do quite a bit of research to get this to behave the way
//	I thought it should. For reference, I used Mike's blog and tutorial
//	series located at: http://www.71squared.co.uk/
//
//	The Particle Controller controls all active particle effects and 
//	determines if the effects have run its course, and if they have
//	clean up any memory associated with their creation. Like the AnimationController,
//	when you load a new effect into the particle controller, it will allocate
//	memory to house the effect and load the effect into a map with the
//	effects asset keyword (IE: CAMPFIRE) When the client wishes to play that
//	effect at a specific location, the big difference between this and
//	the animation controller, is NEW MEMORY WILL BE ALLOCATED on play. This
//	was done to ensure that if you wanted to play 5 campfires at various locations,
//	they will be their OWN particle effect and not be perfectly in sync or rely on
//	a previous effect. To that end, when you "startParticleEffect" it will find
//	the effect you wish to play, create a copy of that effect and load it into
//	the activeParticleEffects list. Once the effect has run its course,
//	the memory will be deallocated.
//
//	It should be of note, the structure of a particle effect is as follows;
//	the particle controller will control, update, and draw every 'effect'
//	A particle effect may have several particle emitters. When updating an effect,
//	it will cycle through each particle emitter, which will cycle through each particle
//	as part of that emitter. 

#ifndef PARTICLECONTROLLER_H
#define PARTICLECONTROLLER_H

#define Particles ParticleController::getInstance()

#include "ParticleEffect.h"

#include <string>
#include <map>
#include <list>

class ParticleController
{
public:
	//get the instance of the particle controller
	static ParticleController* getInstance();

	//	Create a particle effect with the specific name at the specific location.
	//	This will construct a new particle effect at the specific location for the
	//	specified time frame. Once the particle effect has run its course, it will be
	//	deallocated from memory.
	//	duration of -1 is infinite. 
	void startParticleEffect( const std::string name, const Vector2 location,
							 const Vector2 variance, const GLfloat duration );
	
	//	loads a particle effect into memory. This particle effect will be loaded
	//	into a map, and when any particle effects are started, they will be copied
	//	from this loaded effect.
	bool loadParticleEffect( const std::string asset );
	
	//	cycles through all active particle effects, and updates each emitter within
	//	the effect. If the particle effect has run its course, it will flush the
	//	effect, and call the function to deallocate the effect from memory.
	void update( const float deltaTime );
	
	//	Renders each active particle effect to the screen.
	void draw( const float deltaTime );
	
private:
	//	Only 1 copy of the controller will be constructed, and it should never
	//	be constructed by any outside class.
	ParticleController();
	~ParticleController();
	
	static ParticleController *instance;

	std::map<string, ParticleEffect*> effects;
	std::list<ParticleEffect*> activeParticleEffects;
	ParticleEffect* *effectsToDelete;
	int effectsToDeleteCount;

	const int MAX_EFFECTS;
	void flushParticleEffects();
};

#endif