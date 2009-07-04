/*
 *  InputState.h
 *  Djinn Engine
 *
 *  Created by Craig Giles on 2/18/09.
 *
 */

#include "InputState.h"

InputState &InputState::operator =(const InputState &state)
{
	touchLocation = state.touchLocation;
	isBeingTouched = state.isBeingTouched;
	
	return *this;
}