/*
 *  Actor.cpp
 *  Djinn Engine
 *
 *  Created by Craig Giles on 4/2/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Actor.h"

//forward declaration
#include "ActorController.h"

Actor::Actor()
{
	position = NULL;
	size = NULL;
	alpha = 1.0f;
	name = "";
}

Actor::~Actor()
{
	delete position;
	delete size;
	position = NULL;
	size = NULL;
}

std::istream& operator >>( std::istream &inStream, Actor &value )
{
	//	declare data types for reading
	string asset;
	string name;
	int posX, posY;
	int sizeX, sizeY;
	float alpha;
	
	//	read in the data from the stream
	inStream >> asset;
	inStream >> name;
	inStream >> posX >> posY;
	inStream >> sizeX >> sizeY;
	inStream >> alpha;
	
	//	assign the data to the Actor object
	value.setAsset( asset );
	value.setName( name );
	value.setPosition( Vector2( posX, posY ) );
	value.setSize( Vector2( sizeX, sizeY ) );
	value.setAlpha( alpha );
	
	//	return the stream
	return inStream;
}

std::ostream& operator <<( std::ostream &outStream, Actor &value )
{
	string asset = value.getAsset();
	string name = value.getName();
	Vector2 *position = value.getPosition();
	Vector2 *size = value.getSize();
	float alpha = value.getAlpha();	
	
	outStream << asset << '\n';
	outStream << name << '\n';
	outStream << position->x << '\n';
	outStream << position->y << '\n';
	outStream << size->x << '\n';
	outStream << size->y << '\n';
	outStream << alpha << '\n';
	
	return outStream;
}

bool Actor::setIdentity( const string &value )
{
	//	if the identity has already been set, do not
	//	re-set the identity. Object must be destroyed
	//	and re-created to reset an identity. 
	if ( identity.length() > 0)
	{
		return false;
	}
	
	//	if the identity has not been set, set it
	//	and return true
	identity = value;
	return true;
}

bool Actor::setName( const string &value )
{
	if ( name != value )
		name = value;
	
	return true;
}

bool Actor::setPosition( const Vector2 &value )
{
	//	if the position has not been created,
	//	create it with the value
	if ( position == NULL )
		position = new Vector2( value.x, value.y );
	else
	{
		position->x = value.x;
		position->y = value.y;
	}
	
	//	setting parameter was successful
	return true;
}

bool Actor::setSize( const Vector2 &value )
{
	//	If the size has not been created,
	//	create it with the value
	if ( size == NULL )
		size = new Vector2( value.x, value.y );
	else
	{
		size->x = value.x;
		size->y = value.y;
	}
	
	//	setting parameter was successful
	return true;
}

bool Actor::setAlpha( const float value )
{
	//	if alpha is not the current value
	//	set alpha to the current value
	if ( alpha != value )
		alpha = value;
	
	return true;
}

