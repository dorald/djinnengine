/*
 *  ActorController.cpp
 *  Djinn Engine
 *
 *  Created by Craig Giles on 4/3/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ActorController.h"


ActorController* ActorController::instance = 0;// initialize pointer
ActorController::ActorController()
{
}

ActorController* ActorController::getInstance()
{
	//	If no instance has been created, create one
	if ( instance == 0 )
	{
		instance = new ActorController;
	}
	
	//	otherwise, just return the instance
	return instance;
}

const Actor* ActorController::getActorNamed( const string value )
{
	map<string, Actor*>::iterator it = actors.find( value );
	
	//	return the results of our search (Either the Actor or NULL)
	//	if actor is not found, NULL is returned. Otherwise, the pointer
	//	to the actor is returned.
	if ( it == actors.end() )
	{
		return NULL;
	}
	else
	{
		return it->second;
	}
}

void ActorController::setActorsIdentity( Actor *actor )
{
	int counter = 1;
	std::ostringstream iteratedName;
	string identity;
	
	//	get the class name of the actor. This will be used as the base
	//	for the actors identity.
	identity = actor->getClassName();
		
	//	repeat the following loop until a unique identity has been found
	do
	{
		//	reset the iteratedName
		iteratedName.str("");
		
		//	set the iteratedName to the actors className + the current counter.
		//	IE: Dragon_32
		iteratedName << identity << '_' <<  counter++;
		
	} while ( getActorNamed( iteratedName.str() ) != NULL );
	
	//	store the iteratedName into identity string
	identity = iteratedName.str();
	
	//	If the actors identity has not been set
	//	set their identity and add them to the character map.
	//	if the identity HAS been set, false will be returned
	//	and the character is already on the character map.
	if ( actor->setIdentity( identity ) )
		actors[ identity ] = actor;//add actor to the map
}

void ActorController::removeActor( const string &identity )
{
	map<string, Actor*>::iterator it = actors.find( identity );
	delete it->second;
	it->second = NULL;
	actors.erase( it );
}

void ActorController::removeActor( const Actor &actor )
{
	removeActor( actor.getIdentity() );
}

void ActorController::update( const float deltaTime )
{
#warning TODO:: Every actor that needs to be updated, call their update function.
}

void ActorController::draw( const float deltaTime )
{
#warning TODO:: Every actor that needs to be drawn, call their draw function.
}
