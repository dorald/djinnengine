/*
 *  InputState.h
 *  Djinn Engine
 *
 *  Created by Craig Giles on 2/18/09.
 *
 */

#ifndef INPUTSTATE_H
#define INPUTSTATE_H

#include "Vector2.h"

struct InputState
{
	//	The location of the screen that is the focus location
	Vector2 touchLocation;
	
	//	is the screen currently being touched?
	bool isBeingTouched;
	
	//	overloaded operator that sets one state equal to another.
	InputState &InputState::operator =( const InputState &state );
};

#endif