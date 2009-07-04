/*
 *  Vector2D.cpp
 *  Djinn Engine
 *
 *  Created by Craig Giles on 4/22/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Vector2.h"


#pragma mark Constructor(s) / Destructors

Vector2::Vector2()
{
	x = 0;
	y = 0;
}

Vector2::Vector2( const float value )
{
	x = value;
	y = value;
}

Vector2::Vector2( const float x, const float y )
{
	this->x = x;
	this->y = y;
}

Vector2::~Vector2()
{
	//	no Dynamic memory allocated
}


#pragma mark Operator Overloads

Vector2 &Vector2::operator = ( const Vector2 &value )
{
	this->x = value.x;
	this->y = value.y;
	
	return *this;
}


Vector2 Vector2::operator + ( const Vector2 &value ) const
{
	return Vector2(	x + value.x, y + value.y );
}

Vector2 Vector2::operator - ( const Vector2 &value ) const
{
	return Vector2(	x - value.x, y - value.y );
}

Vector2 Vector2::operator * ( const Vector2 &value ) const
{
	return Vector2(	x * value.x, y * value.y );
}

Vector2 Vector2::operator / ( const Vector2 &value ) const
{
	//	If the value of X or Y = 0, the fraction
	//	is undefined. Can not divide by 0, therefore
	//	just return the ZERO vector
	if ( value.x == 0 || value.y == 0 )
		return Vector2::ZERO();
	
	return Vector2(	x / value.x, y / value.y );
}


Vector2 &Vector2::operator += ( const Vector2 &value )
{
	*this = *this + value;	
	return *this;
}

Vector2 &Vector2::operator -= ( const Vector2 &value )
{
	*this = *this - value;	
	return *this;
}

Vector2 &Vector2::operator *= ( const Vector2 &value )
{
	*this = *this * value;
	return *this;
}

Vector2 &Vector2::operator /= ( const Vector2 &value )
{
	*this = *this / value;
	return *this;
}
