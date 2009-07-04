
#include "InputController.h"

#pragma mark Constructor(s) / Destructor

 InputController::InputController( bool isLandscape )
{
	this->isLandscape = isLandscape;
	currentState = new InputState;
	previousState = new InputState;
	queryState = new InputState;
}

InputController::InputController( const InputController &copy )
{
	this->isLandscape = isLandscape;
	currentState = new InputState;
	previousState = new InputState;
	queryState = new InputState;
	
	currentState = copy.currentState;
	previousState = copy.previousState;
	queryState = copy.queryState;
}

InputController::~InputController()
{
	delete currentState;
	delete previousState;
	delete queryState;
}

#pragma mark Update Functions

//
//	Updates the input manager; Moves the current state into
//	the previous state, and updates the current state to be ready
//	to filter through any game screen.
//
void InputController::update(float deltaTime)
{
	//	set previous state to current state, and current to query state.
	previousState = currentState;
	currentState = queryState;
	
	//	converts the coordinate system if the game is in landscape mode
	convertCoordinatesToLandscape();
}

//
//	This function is called from the view of the application, and will hold
//	the query state until the input manager's update function is called.
//	Once called, the currentState will be in sync with the queryState,
//	and the process will start all over again.
//
void InputController::updateQueryState( bool isBeingTouched, const Vector2 &touchLocation )
{
	queryState->isBeingTouched = isBeingTouched;
	queryState->touchLocation = touchLocation;
}

#pragma mark Utility Functions

void InputController::convertCoordinatesToLandscape() const
{
	//	If we are not in landscape mode, don't convert anything
	if ( !isLandscape )
		return;
	
	//	Otherwise, we will need to take the current touch location
	//	swap the x and y values (since in landscape, the portrait
	//	coordinate system means x will point up / down instead of
	//	left / right) and move the y position to match our origin 
	//	point of (0,0) in the upper left corner. 
	int x = currentState->touchLocation.y;
	int y = (currentState->touchLocation.x - 320);
	
	//	make sure we take the absolute value of y, since if we didn't
	//	y would always be a negative number.
	y = abs(y);
	
	//	Since we were converting the current state, we need to update
	//	the current touch location
	currentState->touchLocation = Vector2( x, y );	
}