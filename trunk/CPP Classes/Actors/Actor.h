/*
 *  Actor.h
 *  Djinn Engine
 *
 *  Created by Craig Giles on 4/2/09.
 *
 */

//	Actor Class:
//	An Actor is a class that can be used as the basis for all characters and objects
//	within the game. "Actor," the way it was visioned, can be used by any game object
//	without a graphic (IE: Traps, Ambient Sounds, etc) but currently the functionality
//	for that does not exist. I have not made this class abstract, although I may in 
//	the future, as there is no reason an Actor should be created directly.
//
//	Actors are the data classes that will be controlled via the Actor Controller, and
//	serialized in the same fasion of "ASSETNAME.dat" in the application bundle.
//
//	Current Serialization Format Example:
//	Asset Name:				NINJA
//	Name:					NINJA
//	Position X:				200
//	Position Y:				150
//	Size X:					64
//	Size Y:					64
//	Alpha:					1

#ifndef ACTOR_H
#define ACTOR_H

#include "MathUtil.h"
#include "FileUtil.h"
#include "Animation.h"
#include "Vector2.h"
#include "Serializable.h"
#include "FileUtil.h"

#include <algorithm>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>

using std::string;

//
//	Forward Declarations
//
class ActorController;

class Actor : public Serializable
{
public:	
	//	Overloaded >> and << operators used for serialization and deserialization
	friend std::istream& operator >>( std::istream &inStream, Actor &value );
	friend std::ostream& operator <<( std::ostream &outStream, Actor &value );
	
	//	The constructor ensures default values to the Actors data
	//	and gets it ready to be loaded. The only major thing that happens
	//	in the constructor is it requests a unique ID from the ActorController
	Actor();
	
	//	The destructor ensures the Actor has been removed from the game world
	//	and purged from the list of unique names, freeing the name for
	//	another actor.
	virtual ~Actor();
	

	//	Get and set the id of the actor. The identity of the actor
	//	can only be set once, and that identity will stick with the
	//	actor through their lifetime in the program. The identity
	//	is a unique name associated with the actor, which uses the 
	//	class name as a base and a unique id number at the end to
	//	distinguish itself from other objects created from the same class.
	//	IE: Werewolf_31
	bool setIdentity( const string &value );
	string getIdentity() const { return identity; }
	
	//	Get and set the name of the actor. This is not a unique
	//	identifier, and can be the same as another actor in the 
	//	same game world.
	//	an example of a name is: Werewolf
	bool setName( const string &value );
	string getName() const { return name; }
	
	//	Get and set the position within the gameworld this actor is located.
	bool setPosition( const Vector2 &value );
	Vector2* getPosition() { return position; }
	
	//	Get and set the size of the actor in pixels.
	bool setSize( const Vector2 &value );
	Vector2* getSize() const { return size; }
	
	//	Get and set the alpha value of the actor, used when
	//	rendering the actor to screen.
	bool setAlpha( const float value );
	float getAlpha() const { return alpha; }
	
	//	used in the "setIdentity" function to create a base name for
	//	this class. Should be overridden in any derived classes
	virtual string getClassName() { return "Actor"; }
	
	//	Called once per frame, this function makes the necessary
	//	updates to the Actor. The base implementation just updates
	//	the animations and intervals, but a subclass override
	//	can perform whatever extra processing is required. 
	//	IMPORTANT:: if a derived class overrides this function, you should
	//	call the base class Update function.
	virtual void update( const float deltaTime );
	
protected:
	string identity;
	string name;
	Vector2 *position;
	Vector2 *size;
	float alpha;
};

#endif