/*
 *  ActorController.cpp
 *  Djinn Engine
 *
 *  Created by Craig Giles on 4/3/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
	
#include "ActorController.h"

#pragma mark
#pragma mark Singleton Function 
#pragma mark ----------

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

#pragma mark
#pragma mark Control Functions
#pragma mark ---------- 

const Actor* ActorController::getActorWithId( const string value )
{
	actorMap::iterator it = actors.find( value );
	
	//	return the results of our search (Either the Actor or NULL)
	//	if actor is not found, NULL is returned. Otherwise, the pointer
	//	to the actor is returned.
	if ( it == actors.end() )
		return NULL;
	
	return it->second;
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
		//	IE: Dragon_32 ... 1 through N
		iteratedName << identity << '_' <<  counter++;
		
	} while ( getActorWithId( iteratedName.str() ) != NULL );
	
	//	store the iteratedName into identity string
	identity = iteratedName.str();
	
	//	If the actors identity has not been set
	//	set their identity and add them to the character map.
	//	if the identity HAS been set, false will be returned
	//	and the character is already on the character map.
	if ( actor->setIdentity( identity ) )
		actors[ identity ] = actor;	
}

void ActorController::removeActor( const string &identity )
{
	actorMap::iterator it = actors.find( identity );
	delete it->second;
	it->second = NULL;
	actors.erase( it );
}

void ActorController::removeActor( const Actor &actor )
{
	removeActor( actor.getIdentity() );
}

#pragma mark
#pragma mark Update / Draw
#pragma mark ----------

void ActorController::update( const float deltaTime )
{
	actorMap::iterator it = actors.begin();
	for( /* none */ ; it != actors.end(); it++)
	{
		//	call the actors update function
		it->second->update(deltaTime);
		
		//	TODO::perform any other updates the actor may need.
		//		- Check Collisions
		//		- etc etc
	}
}

void ActorController::draw( const float deltaTime )
{
	actorMap::iterator it = actors.begin();
	for( /* none */ ; it != actors.end(); it++)
	{
		//	call the actors update function
		it->second->draw( deltaTime );
	}
}
