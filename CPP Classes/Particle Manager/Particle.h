/*
 *  Particle.h
 *  DjinnEngine
 *
 *  Created by Craig Giles on 6/15/09.
 *
 */

//	Particle Class:
//	A particle is nothing more than an individual "sparkle" emitting from a
//	given particle emitter. 

#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector2.h"
#include "Color.h"

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

struct Particle
{
	Vector2 position;
	Vector2 direction;
	Color color;
	Color deltaColor;
	GLfloat size;
	GLfloat timeToLive;
};

struct PointSprite
{
	GLfloat x, y, size;
};

#endif