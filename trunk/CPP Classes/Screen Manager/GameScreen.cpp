
#include "GameScreen.h"

//
//	forward declaration headers
//
#include "ScreenController.h"

#pragma mark Constructor(s) / Destructor

GameScreen::GameScreen( const std::string &name, ScreenController *sm )
{
	this->name = name;
	this->screenController = sm;
	isExiting = false;
}

GameScreen::GameScreen( const GameScreen &copy )
{
	*this = copy;
}

GameScreen::~GameScreen()
{
}

//
//	operator overload for assignment operator;
//	since there is dynamic memory allocated with this game screen
//	it was recommended that an assignment operator should be
//	overloaded.
//
//	NOTE:: There should only ever exist one screen manager per
//	program, and every game screen shares the same instance.
//	for this reason, we are shallow copying the screen manager
//	instance rather than creating a new screen manager for every
//	game screen.
//
GameScreen &GameScreen::operator=( const GameScreen &copy )
{
	//	self assignment check
	if (this == &copy)
		return *this;
	
	name = copy.getName();
	screenController = copy.getScreenController();
	viewport = copy.getViewport();
	
	hasBeenUnloaded = false;
	isPopup = copy.getIsPopup();
	
	transitionOnTime = copy.getTransitionOnTime();
	transitionOffTime = copy.getTransitionOffTime();
	transitionPosition = copy.getTransitionPosition();
	transitionAlpha = copy.getTransitionAlpha();
	currentScreenState = copy.getScreenState();
	
	isExiting = false;
	isActive = true;
	otherScreenHasFocus = false;
	
	return *this;
}

#pragma mark Overloaded operators

bool GameScreen::operator==(const GameScreen &value) const
{
	return name == value.getName();
	
}

bool GameScreen::operator!=(const GameScreen &value) const
{
	return name != value.getName();
}

#pragma mark Load / Unload Content

void GameScreen::loadContent()
{
}

void GameScreen::unloadContent()
{
	hasBeenUnloaded = true;
}

#pragma mark Properties

void GameScreen::setViewport(const Rectangle &value)
{
	if (viewport != value)
		viewport = value;
}
void GameScreen::setTransitionOnTime(const float &value)
{
	if ( transitionOnTime != value )
		transitionOnTime = value;
}
void GameScreen::setTransitionOffTime(const float &value)
{
	if ( transitionOffTime != value )
		transitionOffTime = value;
}
void GameScreen::setScreenState(const ScreenState &value)
{
	if ( currentScreenState != value )
		currentScreenState = value;
}
void GameScreen::setTransitionPosition(const int &value)
{
	if ( transitionPosition != value )
		transitionPosition = value;
}

#pragma mark Update / Draw functions

void GameScreen::update( float deltaTime, bool otherScreenHasFocus, bool coveredByOtherScreen )
{
	this->otherScreenHasFocus = otherScreenHasFocus;
	
	if ( isExiting )
	{
		//if the screen is going to die, it should transition off
		currentScreenState = TransitionOff;
		
		if ( !updateTransition(deltaTime, transitionOffTime, 1) )
		{
			//when transition finishes, remove the screen
			screenController->killScreen( this );
		}
	}
	else if ( coveredByOtherScreen )
	{
		//if the screen is covered by another, it should transition off
		if ( updateTransition(deltaTime, transitionOffTime, 1) )
		{
			//scren is still transitioning
			currentScreenState = TransitionOff;
		}
		else
		{
			//transition has finished
			currentScreenState = Hidden;
		}
	}
	else
	{
		if ( updateTransition(deltaTime, transitionOnTime, -1) )
		{
			//still busy transitioning
			currentScreenState = TransitionOn;
		}
		else
		{
			//transition finished
			currentScreenState = Active;
		}
	}
	
}

//
//	Helper function to update the transition of the game screen
//	when it enters and exits from the game. It will keep track of
//	its current alpha value, which direction it is transitioning,
//	and update its fade in / fade out depending on the time it last
//	took to call this function.
//
bool GameScreen::updateTransition(float deltaTime, float transition, int direction)
{
	//	how much should we move by?
	float transitionDelta;
	
	if ( transition <= 0 )
		transitionDelta = 1;
	else
		transitionDelta = deltaTime / transition;
	
	//update the transition position
	transitionPosition += transitionDelta * direction;
	
	//did we reach the end of the transition?
	if ( direction < 0 && transitionPosition <= 0 ||
		 direction > 0 && transitionPosition >= 1 )
	{
		//clamp transition position to 0, 1, or value;
		if (transitionPosition >= 1)
			transitionPosition = 1;
		else if (transitionPosition <= 0)
			transitionPosition = 0;
		
		return false;
	}//end "end of transition"
	
	//otherwise, we are still busy transitioning
	return true;
}

void GameScreen::draw(float deltaTime)
{
	//	TODO:: Do I need to do anything with this? or just make it a pure virtual?
}

void GameScreen::exitScreen()
{
#warning Need to implement this function (GameScreen::exitScreen)
}
