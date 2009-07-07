/*
 *  ParticleEmitter.cpp
 *  DjinnEngine
 *
 *  Created by Craig Giles on 6/15/09.
 *
 */

#include "ParticleEmitter.h"

#pragma mark Object De-Serialization >> function.

std::istream& operator >> ( std::istream &inStream, ParticleEmitter &value )
{
	std::string textureName;
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
	bool blendAdditive;
	
	inStream >> textureName;
	inStream >> angle;
	inStream >> angleVariance;
	inStream >> speed;
	inStream >> speedVariance;
	inStream >> gravity.x >> gravity.y;
	inStream >> lifespan;
	inStream >> lifespanVariance;
	inStream >> startColor.red >> startColor.green >> startColor.blue >> startColor.alpha;
	inStream >> startColorVariance.red >> startColorVariance.green >> startColorVariance.blue >> startColorVariance.alpha;
	inStream >> endColor.red >> endColor.green >> endColor.blue >> endColor.alpha;
	inStream >> endColorVariance.red >> endColorVariance.green >> endColorVariance.blue >> endColorVariance.alpha;
	inStream >> size;
	inStream >> sizeVariance;
	inStream >> maxParticles;
	inStream >> blendAdditive;
	
	value.setEmitterParameters( textureName,
							   angle,
							   angleVariance,
							   speed,
							   speedVariance,
							   gravity,
							   lifespan,
							   lifespanVariance,
							   startColor,
							   startColorVariance,
							   endColor,
							   endColorVariance,
							   size,
							   sizeVariance,
							   maxParticles,
							   blendAdditive);
	
	return inStream;
}

#pragma mark Constructor(s) / Destructor

ParticleEmitter::ParticleEmitter()
{
	texture = NULL;
	particles = NULL;
	vertices = NULL;
	colors = NULL;
}

ParticleEmitter::~ParticleEmitter()
{
	delete [] vertices;
	delete [] colors;
	delete [] particles;
	glDeleteBuffers(1, &verticesID);
	glDeleteBuffers(1, &colorsID);
	texture->releaseReference();
	
	texture = NULL;
	particles = NULL;
	vertices = NULL;
	colors = NULL;
}

#pragma mark Initialization Functions

void ParticleEmitter::setEmitterParameters( const std::string textureName,
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
										   const bool blendAdditive)
{
	if (this->texture != NULL)
	{
		delete [] this->vertices;
		delete [] this->colors;
		delete [] this->particles;
		glDeleteBuffers(1, &this->verticesID);
		glDeleteBuffers(1, &this->colorsID);
		this->texture->releaseReference();
		
		this->texture = NULL;
		this->particles = NULL;
		this->vertices = NULL;
		this->colors = NULL;
	}
	
	this->texture = Textures->getTexture( textureName );
	this->texture->addReference();
	this->angle = angle;
	this->angleVariance = angleVariance;
	this->speed = speed;
	this->speedVariance = speedVariance;
	this->gravity = gravity;
	this->lifespan = lifespan;
	this->lifespanVariance = lifespanVariance;
	this->startColor = startColor;
	this->startColorVariance = startColorVariance;
	this->endColor = endColor;
	this->endColorVariance = endColorVariance;
	this->size = size;
	this->sizeVariance = sizeVariance;
	this->maxParticles = maxParticles;
	this->blendAdditive = blendAdditive;
	this->emissionRate = maxParticles / lifespan;
	
	particles = new Particle[ maxParticles ];
	vertices = new PointSprite[ maxParticles ];
	colors = new Color[ maxParticles ];
	
	glGenBuffers( 1, &verticesID );
	glGenBuffers( 1, &colorsID );
	
	active = false;
	
	particleCount = 0;
	elapsedTime = 0;
}

void ParticleEmitter::setTextureName( const std::string textureName )
{ 
	//this->texture = new Texture2D( textureName, PNG );	
	//this->texture = Textures->getTexture( textureName );
}

void ParticleEmitter::initParticle( Particle &particle )
{
	particle.position.x = source.x + sourceVariance.x * RNG::getRandomFloat( -1.0f, 1.0f);
	particle.position.y = source.y + sourceVariance.y * RNG::getRandomFloat( -1.0f, 1.0f);
	
	float newAngle = (GLfloat)MathUtil::DegreesToRadians( angle + angleVariance * RNG::getRandomFloat( -1.0f, 1.0f) );
	Vector2 newVector( cosf(newAngle) , sinf(newAngle) );
	float vectorSpeed = speed + speedVariance * RNG::getRandomFloat( -1.0f, 1.0f);
	particle.direction = newVector * vectorSpeed;
	particle.size = size + sizeVariance * RNG::getRandomFloat( -1.0f, 1.0f);
	particle.timeToLive = lifespan + lifespanVariance * RNG::getRandomFloat( -1.0f, 1.0f);
	
	Color start(0,0,0,0);
	start.red = startColor.red + startColorVariance.red * RNG::getRandomFloat( -1.0f, 1.0f);
	start.green = startColor.green + startColorVariance.green * RNG::getRandomFloat( -1.0f, 1.0f);
	start.blue = startColor.blue + startColorVariance.blue * RNG::getRandomFloat( -1.0f, 1.0f);
	start.alpha = startColor.alpha + startColorVariance.alpha * RNG::getRandomFloat( -1.0f, 1.0f);

	Color end(0,0,0,0);
	end.red = endColor.red + endColorVariance.red * RNG::getRandomFloat( -1.0f, 1.0f);
	end.green = endColor.green + endColorVariance.green * RNG::getRandomFloat( -1.0f, 1.0f);
	end.blue = endColor.blue + endColorVariance.blue * RNG::getRandomFloat( -1.0f, 1.0f);
	end.alpha = endColor.alpha + endColorVariance.alpha * RNG::getRandomFloat( -1.0f, 1.0f);
	
	particle.color = start;
	particle.deltaColor.red = (end.red - start.red) / particle.timeToLive;
	particle.deltaColor.green = (end.green - start.green) / particle.timeToLive;
	particle.deltaColor.blue = (end.blue - start.blue) / particle.timeToLive;
	particle.deltaColor.alpha = (end.alpha - start.alpha) / particle.timeToLive;
}

#pragma mark Start / Stop Particle Emitter

void ParticleEmitter::startParticleEmitter( const Vector2 &source, const Vector2 &sourceVariance, const GLfloat duration )
{
	active = true;
	stopEmitting = false;
	elapsedTime = 0;
	emitCounter = 0;
	
	this->source = source;
	this->sourceVariance = sourceVariance;
	this->duration = duration;
}

void ParticleEmitter::stopParticleEmitter()
{
	active = false;
	stopEmitting = true;
	elapsedTime = 0;
	emitCounter = 0;
}

#pragma mark Update / Draw Functions

bool ParticleEmitter::addParticle()
{
	if ( particleCount == maxParticles )
		return false;
	
	Particle *particle = &particles[ particleCount ];
	initParticle( *particle );
	
	++particleCount;
	return true;
}

void ParticleEmitter::update( const float deltaTime )
{
	if ( active && emissionRate )
	{
		float rate = 1.0f / emissionRate;
		emitCounter += deltaTime;
		
		while ( !stopEmitting && particleCount < maxParticles && emitCounter > rate )
		{
			addParticle();
			emitCounter -= rate;
		}
		
		elapsedTime += deltaTime;
		if ( duration != -1 && duration < elapsedTime )
		{
			stopEmitting = true;
			
			if (particleCount == 0)
			{
				stopParticleEmitter();
			}
		}
	}
	
	//	reset the particle index before updating the particles
	particleIndex = 0;
	
	//	update all the particles in the array
	while ( particleIndex < particleCount )
	{
		Particle *currentParticle = &particles[ particleIndex ];
		
		if ( currentParticle->timeToLive > 0 )
		{
			currentParticle->direction += gravity;
			currentParticle->position += currentParticle->direction;
			
			//reduce the lifespan
			currentParticle->timeToLive -= deltaTime;
			
			vertices[ particleIndex ].x = currentParticle->position.x;
			vertices[ particleIndex ].y = currentParticle->position.y;
			
			vertices[ particleIndex ].size = currentParticle->size;
			
			//update particle color
			currentParticle->color.red += ( currentParticle->deltaColor.red * deltaTime );
			currentParticle->color.green += ( currentParticle->deltaColor.green * deltaTime );
			currentParticle->color.blue += ( currentParticle->deltaColor.blue * deltaTime );
			currentParticle->color.alpha += ( currentParticle->deltaColor.alpha * deltaTime );
			
			//place the color of the current particle in to the color array
			colors[ particleIndex ] = currentParticle->color;
			
			++particleIndex;
		}
		else
		{
			if ( particleIndex != particleCount - 1 )
			{
				particles[ particleIndex ] = particles[ particleCount - 1 ];
			}
			
			--particleCount;
		}
	}
	
	//if (allParticlesDead)
	//	stopParticleEmitter();
	
	glBindBuffer( GL_ARRAY_BUFFER, verticesID );
	glBufferData( GL_ARRAY_BUFFER, sizeof(PointSprite) * maxParticles, vertices, GL_DYNAMIC_DRAW );
	glBindBuffer( GL_ARRAY_BUFFER, colorsID );
	glBufferData( GL_ARRAY_BUFFER, sizeof(Color) * maxParticles, colors, GL_DYNAMIC_DRAW );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void ParticleEmitter::draw( const float deltaTime )
{
#warning Obviously this isn't the way i want to do this... point sprites currently giving me an error when its the only thing being rendered. This was just testing a theory...
	for (int i = 0; i < particleCount; ++i )
	{
		glEnable( GL_BLEND );
		glAlphaFunc(GL_GREATER, 0.1f);
		glEnable(GL_ALPHA_TEST);
		
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_ONE, GL_SRC_COLOR);
		
		//glColor4f(particles[i].color.red, particles[i].color.green, particles[i].color.blue, particles[i].color.alpha);
		texture->draw( Rectangle(particles[i].position.x, particles[i].position.y, 32, 32) );	
		
		glDisable( GL_ALPHA_TEST );
		glDisable( GL_BLEND );
	}
//	glEnable( GL_TEXTURE_2D );
//
//	texture->bindTexture();
//
//	
//	glEnable( GL_BLEND );
//	
//	if ( blendAdditive ) 
//	{
//		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
//		//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
//	}
//	else
//	{
//		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
//	}	
//
//	//
//	glAlphaFunc(GL_GREATER, 0.1f);
//	glEnable(GL_ALPHA_TEST);
//	//
//	
//	glEnable( GL_POINT_SPRITE_OES );
//	glTexEnvi( GL_POINT_SPRITE_OES, GL_COORD_REPLACE_OES, GL_TRUE );
//	
//	glEnableClientState( GL_VERTEX_ARRAY );
//	glBindBuffer( GL_ARRAY_BUFFER, verticesID );
//	
//	glVertexPointer( 2, GL_FLOAT, sizeof(PointSprite), 0 );
//	
//	glEnableClientState( GL_POINT_SIZE_ARRAY_OES );  // <--------- problem with point size
//	glPointSizePointerOES( GL_FLOAT, sizeof(PointSprite), (GLvoid*)(sizeof(GL_FLOAT)*2));
//	
//	glEnableClientState(GL_COLOR_ARRAY);
//	glBindBuffer(GL_ARRAY_BUFFER, colorsID);
//	glColorPointer(4, GL_FLOAT, 0, 0);
//	glDrawArrays(GL_POINTS, 0, particleIndex);		// <--------- blows up here
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	
//	glDisableClientState(GL_POINT_SIZE_ARRAY_OES);
//	glDisableClientState(GL_COLOR_ARRAY);
//	glDisableClientState(GL_POINT_SPRITE_OES);
//	glDisableClientState(GL_POINT_SIZE_ARRAY_OES);
//	glDisableClientState(GL_COLOR_ARRAY);
//	glDisable(GL_POINT_SPRITE_OES);
//	glColor4f(1, 1, 1, 1);	
	

}















