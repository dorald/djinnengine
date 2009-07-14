/*
 *  Character.h
 *  DjinnEngine
 *
 *  Created by Craig Giles on 5/30/09.
 *
 */

//	Character Class:
//	The character class is the start of the game classes, but I still put it in
//	in the engine for a few reasons.. All games will need characters, and thus
//	i figured that it was still viable to put in the engine.
//
//	The character class is a child of the Actor base class, and is a visual
//	representation of an object on screen. The character class was envisioned
//	as a base class for various other classes in teh game, some offshoots could
//	be interactive characters such as NPCs, Enemies, or even the Player.. other
//	classes that may use this object as its base or as-is would be things like
//	treasure chests, in-game objects (Desks, lamps, etc) and the like.
//
//	Character Class Object Serialization Guide:
//	The character class requires the first few lines of its .dat file to be
//	the same as the Actor class, for the most part because it offloads
//	the unSerialization of the actors data to the actor class itself. Once
//	the actor class has been unserialized, it will take over from that point
//
//	Character Serialization Example .dat File:
//	Asset Name:						NINJA
//	Name:							NINJA
//	Position X:						200
//	Position Y:						150
//	Size X:							64
//	Size Y:							64
//	Alpha:							1
//	# Animations:					1
//	list of all animation names:	RUNNINGLEFT
//	NOTE: these are 'asset names'	RUNNINGRIGHT
//									IDLE
//									ATTACKLEFT

#ifndef CHARACTER_H
#define CHARACTER_H

#include "Actor.h"
#include "Vector2.h"
#include "Rectangle.h"
#include "AnimationController.h"
#include "ActorController.h"

#include <string>
#include <iostream>

using std::string;
using std::istream;
using std::ostream;

class Character : public Actor
{
	friend istream &operator >>( istream &inStream, Character &value );
	friend ostream &operator <<( ostream &outStream, Character &value );
	
public:
	//	Constructor / Destructor for character class. Since the Character
	//	is also a base class, it should have virtual destructor to ensure
	//	the destruction of all children.
	Character();
	virtual ~Character();	
	
	//	Since A character is a visual object on the game screen, they have
	//	a position in the game world (inherited from the Actor class) 
	//	a destination in the game world (where they're going to) and
	//	a velocity (how fast they're getting there). For immobile objects,
	//	the velocity will be Vector2::ZERO() and the destination will be
	//	and always be the same as the current location.
	bool setDestination( const Vector2 &value );
	Vector2* getDestination() const { return destination; }
	
	//	The velocity is how fast a character is moving towards their
	//	destination. 
	bool setVelocity( const Vector2 &value );
	Vector2* getVelocity() const { return velocity; }
	
	
	//	used in the "setIdentity" function to create a base name for
	//	this class. Should be overridden in any derived classes
	virtual string getClassName() { return "Character"; }
	
	//	get and set the number of animations used by this character
	//	on screen.
	void setNumberOfAnimations( const int value );
	int getNumberOfAnimations() const { return numberOfAnimations; }
	
	//	get and set the animation asset names used to animate this
	//	character on screen.
	void setAnimationAssetNames( const string *value );
	string* getAnimationAssetNames() const { return animationAssetNames; }
	
	//	Load available animations using the animation controller, and
	//	play the default animation so the Character has something to show
	//	to screen (IE: the game doesn't crash from a null exception :D )
	void initializeAnimationController();

	virtual void update( const float deltaTime);
	virtual void draw( const float deltaTime );
	
protected:
	Character( const Character &copy );
	Character &operator =( const Character &copy );
	
	AnimationController *animation;
	Rectangle *spriteBox;
	Vector2 *destination;
	Vector2 *velocity;
	
	int numberOfAnimations;
	string* animationAssetNames;

	void updateSpriteBox();
};

#endif
