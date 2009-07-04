/*
 *  InputController.h
 *  Djinn Engine
 *
 *  Created by Craig Giles on 2/18/09.
 *
 */

//	Input Controller:
//	I have not yet converted this to use the singleton. Many would remember
//	the tutorial I did on this class, and it has been vastly unchanged since
//	then (with the exception that its now in C++ and not Objective-C)
//	It may be worth while to change it to a singleton, but with the way 
//	the screen manager works, this class does its job and works just fine.

#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H

#include "InputState.h"
#include <stdlib.h>

class InputController
{
public:
	//
	//	Constructor(s) / Destructor
	//
	InputController( bool isLandscape );
	InputController( const InputController &copy );
	~InputController();
	
	//	This function is called from the view of the application, and will hold
	//	the query state until the input manager's update function is called.
	//	Once called, the currentState will be in sync with the queryState,
	//	and the process will start all over again.
	void updateQueryState( bool isBeingTouched, const Vector2 &touchLocation );
	
	//	This update function gets called once per frame. It will set the
	//	previous inputState to the current inputState, and update the current
	//	inputState to be whatever was included in the query state. 
	void update( float deltaTime );
	
private:
	bool isLandscape;
	InputState *currentState;
	InputState *previousState;
	InputState *queryState;
	
	void convertCoordinatesToLandscape() const;
};

#endif