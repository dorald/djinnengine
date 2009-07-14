/*
 *  ActorController.h
 *  Djinn Engine
 *
 *  Created by Craig Giles on 4/3/09.
 *
 */

//	The Actor Controller is the Controller portion of the 'model view controller'
//	for all objects inheriting from the Actor base class. IE: Enemies, Characters, NPCs,
//	Traps, in-game objects, etc.
//
//	The Actor Controller is a singleton object, meaning only one instance can be created
//	at any given time. As with all singletons, a #define shortcut was programmed in so
//	the client would only have to type "Actors->" instead of "ActorController::getInstance()->"
//	every time they wish to access this controller.
//	
//	There is a little work needing to be done with this controller, but everything should be
//	commented well enough to get the jist of what was intended from the class. Sections
//	marked with #warning are nothing more than notes to myself of things that should be
//	taken care of.

#ifndef ACTORCONTROLLER_H
#define ACTORCONTROLLER_H

//	shortcut to singleton
#define Actors ActorController::getInstance()

#include "Actor.h"
#include "Character.h"

#include <string>
#include <sstream>
#include <map>

using std::string;
using std::map;

class ActorController
{
public:
	//	Gets the instance of the ActorController. Since only one instance
	//	can be created at any given time, the only way to access the
	//	controller is via this function
	static ActorController *getInstance();
	
	//	instanciates a new instance of an Actor class. Several notes about this:
	//	The reason its in the interface and not the implementation is because of
	//	the Templates. I am unfamiliar with templates enough to pre-compile what
	//	I need as an .obj file, but was told this sloppy method would work for
	//	what I needed.
	//
	//	The actor will be deserialized, and the ID will be returned in case the
	//	client wishes to hold onto that information (IE: If the player character
	//	is being deserialized)
	template< class T >
	std::string loadActor( const std::string &asset )
	{
		T* actor = new T;
		Files->unSerialize( asset, *actor );
		setActorsIdentity( actor );
		return actor->getIdentity();
	}
	
	//	Search through all active actors for a specific actor named X.
	//	If the actor is found, the pointer to that specific actor is returned,
	//	however if the actor is not found, NULL is returned.
	const Actor* getActorNamed( const string value );
	
	//	This function will set the actors identity to a unique name that
	//	the actor will carry through their lifetime. We will start with
	//	ClassName_1 and increase the number until a unique id is found.
	//	An example of a unique identity would be: Werewolf_35
	void setActorsIdentity( Actor *actor );
	
	//	Removes an actor from the map. This is to be called by the actors
	//	destructor, and not by any other class.
	void removeActor( const string &identity );
	void removeActor( const Actor &actor );
	
	//	Called once per frame, this function will call each actors Update
	//	function and allow each actor to perform their update logic.
	void update( const float deltaTime );
	
	//	Called once per frame, this function will call each actors Draw
	//	function, and allow each actor to be rendered to screen.
	void draw( const float deltaTime );
	
protected:
	//	All constructors are labeled as protected so no explicit instance
	//	can be instanciated.
	ActorController();
	ActorController( const ActorController &copy );
	ActorController& operator=( const ActorController &copy);
	
	//	All actors active in the game world.
	//	string = Actors identity name
	//	Actor* = The base class pointer to the actor.
	typedef map<string, Actor*> actorMap;
	actorMap actors;
	
private:
	static ActorController* instance;
		
};

#endif