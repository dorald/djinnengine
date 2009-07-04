/*
 *  Rectangle.cpp
 *  Djinn Engine
 *
 *  Created by Craig Giles on 5/1/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Rectangle.h"

//
//	Constructor(s) / Destructor
//
Rectangle::Rectangle( int x, int y, int width, int height )
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

//
//	operator overloads
//
bool Rectangle::operator ==( const Rectangle &value )
{
	if ( x == value.x &&
		y == value.y &&
		width == value.width &&
		height == value.height )
	{
		return true;
	}
	
	return false;
}

bool Rectangle::operator !=( const Rectangle &value )
{
	return !( *this == value );
}

//	Checks the rectangle and determines if the specified point
//	is located within the rectangle's body.
bool Rectangle::containsPoint( const Vector2 *value )
{
	if ( value->x > x &&
		value->x < x + width &&
		value->y > y &&
		value->y < y + height)
	{
		return true;
	}
	
	return false;
}