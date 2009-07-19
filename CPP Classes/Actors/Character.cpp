/*
 *  Character.cpp
 *  DjinnEngine
 *
 *  Created by Craig Giles on 5/30/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Character.h"

#pragma mark
#pragma mark Serialization Operators
#pragma mark ----------

istream &operator >>( istream &inStream, Character &value )
{
	//	since the actor class is the Character's base class,
	//	we'll let the actor class load its data before we load
	//	the character classes data. This has to be formatted
	//	correctly in the file in order for this to work.
	Actor *base = &value;
	inStream >> *base;

	int numberOfAnimations;
	string *animationAssetNames;
	
	inStream >> numberOfAnimations;
	animationAssetNames = new string[ numberOfAnimations ];
	
	for ( int i = 0; i < numberOfAnimations; ++i )
		inStream >> animationAssetNames[ i ];
	
	value.setNumberOfAnimations( numberOfAnimations );
	value.setAnimationAssetNames( animationAssetNames );
	
	delete [] animationAssetNames;
	
	value.initializeAnimationController();
	
	//	return the stream
	return inStream;
	

}

ostream &operator <<( ostream &outStream, Character &value )
{
	return outStream;
}

#pragma mark
#pragma mark Constructor(s) / Destructor
#pragma mark ----------

Character::Character()
{
	animation = new AnimationController;
	spriteBox = new Rectangle( Rectangle::ZERO() );
	destination = new Vector2( Vector2::ZERO() );
	velocity = new Vector2( Vector2::ZERO() );
	numberOfAnimations = 0;
	animationAssetNames = NULL;
}

Character::~Character()
{
	delete animation;
	delete spriteBox;
	delete destination;
	delete velocity;
	delete animationAssetNames;
}

#pragma mark
#pragma mark Functions
#pragma mark ----------

bool Character::setDestination( const Vector2 &value )
{
	if ( destination == NULL )
		destination = new Vector2( value.x, value.y );
	else
	{
		destination->x = value.x;
		destination->y = value.y;
	}
	
	return true;
}

bool Character::setVelocity( const Vector2 &value )
{
	if ( velocity == NULL )
		velocity = new Vector2( value.x, value.y );
	else
	{
		velocity->x = value.x;
		velocity->y = value.y;
	}
	
	return true;
}

void Character::initializeAnimationController()
{
	for ( int i = 0; i < numberOfAnimations; ++i )
		animation->loadAnimation( animationAssetNames[ i ] );
	
	animation->playDefaultAnimation();
}

void Character::setNumberOfAnimations( const int value )
{
	if ( numberOfAnimations != value )
		numberOfAnimations = value;
}

void Character::setAnimationAssetNames( const string *value )
{
	delete animationAssetNames;
	animationAssetNames = NULL;
	
	animationAssetNames = new string[ numberOfAnimations ];
	
	for ( int i = 0; i < numberOfAnimations; ++i )
	{
		animationAssetNames[ i ] = value[ i ];
	}
	
}

#pragma mark
#pragma mark Update / Draw
#pragma mark ----------

void Character::updateSpriteBox()
{
	spriteBox->x = position->x;
	spriteBox->y = position->y;
	spriteBox->width = size->x;
	spriteBox->height = size->y;
}

void Character::update( const float deltaTime )
{
	//	Since the actor controller will handle all collision detection, really
	//	the only thing that needs to be updated by this character is its animation
	//	and sprite box.
	animation->update( deltaTime );	
	updateSpriteBox();
}

void Character::draw( const float deltaTime )
{
	animation->draw( deltaTime, *spriteBox );
}

















