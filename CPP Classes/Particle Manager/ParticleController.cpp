/*
 *  ParticleController.cpp
 *  DjinnEngine
 *
 *  Created by Craig Giles on 6/15/09.
 *
 */

#include "ParticleController.h"

#pragma mark Constructor(s) / Destructor

ParticleController* ParticleController::instance = 0;

ParticleController* ParticleController::getInstance()
{
	if ( instance == 0 )
		instance = new ParticleController();
		
	return instance;
}

ParticleController::ParticleController() : MAX_EFFECTS( 32 )
{
	effectsToDeleteCount = 0;
	effectsToDelete = new ParticleEffect*[ MAX_EFFECTS ];
}

ParticleController::~ParticleController()
{
}

#pragma mark Functions

void ParticleController::startParticleEffect( const std::string name, const Vector2 location,
						 const Vector2 variance, const GLfloat duration )
{
	ParticleEffect *effect = new ParticleEffect( *effects[ name ] );
	effect->startParticleEffect( location, variance, duration );
	activeParticleEffects.push_back( effect );
}

void ParticleController::flushParticleEffects()
{
	for ( int i = 0; i < effectsToDeleteCount; ++i )
	{
		ParticleEffect *effect = effectsToDelete[i];
		activeParticleEffects.remove( effect );
		delete effect;
	}
	
	effectsToDeleteCount = 0; 
}

bool ParticleController::loadParticleEffect( const std::string asset )
{
	ParticleEffect *effect = new ParticleEffect;
	Files->unSerialize( asset, *effect );
	
	effects[ asset ] = effect;
		
	return true;
}

#pragma mark Update / Draw functions.

void ParticleController::update( const float deltaTime )
{
	//	So far this update function does nothing more than cycle through
	//	each particle emitter and tell that emitter to update itself
	std::list<ParticleEffect*>::iterator it;
	
	for ( it = activeParticleEffects.begin(); it != activeParticleEffects.end(); it++ )
	{
		(*it)->update( deltaTime );
		if ( !(*it)->isActive() )
		{
			effectsToDelete[ effectsToDeleteCount ] = *it;
			++effectsToDeleteCount;
		}
	}

	flushParticleEffects();
	
}

void ParticleController::draw( const float deltaTime )
{
	//	So far this draw function does nothing more than cycle through
	//	each particle emitter and tell that emitter to draw itself
	std::list<ParticleEffect*>::iterator it;
	
	for ( it = activeParticleEffects.begin(); it != activeParticleEffects.end(); it++ )
		(*it)->draw( deltaTime );
}