/*
 *  ParticleEffect.cpp
 *  DjinnEngine
 *
 *  Created by Craig Giles on 6/17/09.
 *
 */

#include "ParticleEffect.h"

#pragma mark
#pragma mark Object Serialization
#pragma mark ----------

std::istream& operator >>( std::istream &inStream, ParticleEffect &value )
{
	string asset;
	int numOfEmitters;
	
	inStream >> asset;
	inStream >> numOfEmitters;
	
	std::string emitterAssets[ numOfEmitters ];
	for ( int i = 0; i < numOfEmitters; ++i )
		inStream >> emitterAssets[i];
	
	value.setAsset( asset );
	value.loadParticleEmitters( emitterAssets, numOfEmitters );
	
	return inStream;
}

std::ostream& operator <<( std::ostream &outStream, ParticleEffect &value )
{
	return outStream;
}

#pragma mark
#pragma mark Constructor(s) / Destructor
#pragma mark ----------

ParticleEffect& ParticleEffect::operator =( const ParticleEffect &copy )
{
	if ( this == &copy )
		return *this;
	
	string asset = copy.getAsset();
	Files->unSerialize( asset, *this );
	
	return *this;
}

ParticleEffect::ParticleEffect()
{
	numOfEmitters = 0;
	emitterAssets = NULL;
	emitters = NULL;
	active = false;
}

ParticleEffect::ParticleEffect( const ParticleEffect& copy )
{
	*this = copy;
}

ParticleEffect::~ParticleEffect()
{	
	delete [] emitters;
	delete [] emitterAssets;
	
	emitterAssets = NULL;
	emitters = NULL;
}

#pragma mark
#pragma mark Loading / Unloading Effects
#pragma mark ----------

void ParticleEffect::loadParticleEmitters( const std::string *assets, const int numOfEmitters )
{
	this->numOfEmitters = numOfEmitters;
	emitterAssets = new std::string[ numOfEmitters ];
	emitters = new ParticleEmitter[ numOfEmitters ];
	
	for (int i = 0; i < numOfEmitters; ++i )
	{
		emitterAssets[ i ] = assets[ i ];
		Files->unSerialize( emitterAssets[i], emitters[i] );
	}
}

void ParticleEffect::startParticleEffect( const Vector2 location, const Vector2 variance, const float duration )
{
	active = true;
	
	for ( int i = 0; i < numOfEmitters; ++i )
	{
		emitters[i].startParticleEmitter( location, variance, duration );
	}	
}

void ParticleEffect::stopParticleEffect()
{
	active = false;
}

#pragma mark
#pragma mark Update / Draw
#pragma mark ----------

void ParticleEffect::update( const float deltaTime )
{
	bool allEmittersDead = true;
	
	for ( int i = 0; i < numOfEmitters; ++i )
	{
		emitters[i].update( deltaTime );
		if (emitters[i].isActive())
			allEmittersDead = false;
	}
	
	if (allEmittersDead)
		stopParticleEffect();
}

void ParticleEffect::draw( const float deltaTime )
{
	for ( int i = 0; i < numOfEmitters; ++i )
		emitters[i].draw( deltaTime );
}




















