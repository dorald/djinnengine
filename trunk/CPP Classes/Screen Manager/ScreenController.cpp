
#include "ScreenController.h"

//
//	forward declaration headers
//
#include "GameScreen.h"
ScreenController* ScreenController::instance = 0;

ScreenController* ScreenController::getInstance()
{
	if ( instance == 0 )
		instance = new ScreenController();
	
	return instance;
}

#pragma mark Constructor(s) / Destructor

ScreenController::ScreenController() :
	LANDSCAPE_MODE(true),
	TRANSITION_ON_TIME(.70f),
	TRANSITION_OFF_TIME(.20f)
{
	setupView();
	loadContent();
}

ScreenController::~ScreenController()
{
	delete blankTexture;
	delete input;
	delete screens;
	delete screensToUpdate;
}

#pragma mark Setup view and Load Content

//
//	This function sets up OpenGL for how we wish to draw objects to the 
//	screen by editing the specific "modes" such as the projection
//	and model view matricies. 
//
void ScreenController::setupView()
{	
	//	Modify the Projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	//
	//	Orthof projection is used for 2d games. This sets the coordinates to
	//	(0, 0) at the top left corner of the screen, and as you move downward
	//	your y value will increase. As you move to the right, your x value will
	//	increase.
	//	(left, right, bottom, top, near, far)
	//
	//	If the game is going to be played in landscape mode, enable
	//	it via the bool switch at the top of the GLViewController.m file.
	if ( LANDSCAPE_MODE )
	{
		viewport = Rectangle(0, 0, 480, 320);
		glViewport(0, 0, viewport.height, viewport.width);
		glRotatef(-90, 0, 0, 1);
        glOrthof(0, viewport.width, viewport.height, 0, -10.0, 10.0);  
		
	}
	else	//	Game is to be played in portrait
	{
		viewport = Rectangle(0, 0, 320, 480);
		glViewport(0, 0, viewport.width, viewport.height);
		glOrthof(0.0, viewport.width, viewport.height, 0.0, -1.0, 1.0);	
	}
	
	
	//
	//	Setup Model view matrix
	//	Load graphics settings
	//
	glMatrixMode(GL_MODELVIEW);
	
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);	
	
	//	needed to draw textures using Texture2D
	glEnable(GL_TEXTURE_2D);	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	
	//	enables alpha for transparent textures
	//	I forget where I got these commands, iDevGames.net I think
	glAlphaFunc(GL_GREATER, 0.1f);
	glEnable(GL_ALPHA_TEST);
	
	//
	//	Setup clear color (cornflower blue'ish)
	//	Call me crazy, but I got so used to this color developing
	//	for XNA. This is a little nod to the Microsoft Development
	//	Team :)
	//
	glLoadIdentity(); 
	glClearColor(.39f, 0.58f, 0.920f, 0.0f); 
}

//
//	Now that the view has been initialized and OpenGL settings have been
//	modified, it is time to load enough content of the game to get started.
//
void ScreenController::loadContent()
{	
	//
	//	Seed the random number generator so we do not need to seed it 
	//	again
	//
	RNG::seedRandomNumberGenerator();
	
	//
	//	Since I am using dynamically allocated lists, we need to allocate
	//	memory for the lists. 
	//
	screens = new list<GameScreen *>;
	screensToUpdate = new list<GameScreen *>;
	
	//
	//	cycle through each screen in the screens list and tell them to
	//	load their content
	//	
	for ( screen = screens->begin(); screen != screens->end(); ++screen)
	{
		(*screen)->loadContent();
	}
	
	//
	//	Allocate memory for the input manager and the blankTexture used
	//	to fade the screen in / out.
	//
	input = new InputController( LANDSCAPE_MODE );
	blankTexture = new Texture2D( "blankTexture", PNG );

	//
	//	Once we are initialized, set the bool values to appropriate values. 
	//
	isInitialized = true;
	traceEnabled = false;
	
	//
	//	Adds a title screen to the game stack. This will be taken out 
	//	later, and right now is only used for debugging purposes. It
	//	will be replaced with your splash screen or game introduction
	//	screen.
	//
	
	//	<< TODO::create and initialize title screen >>
	GameScreen *newScreen = new TitleScreen( "TitleScreen", this );
	addScreen( newScreen );
}

#pragma mark Update / Draw Screens

//
//	Cycles through each game screen in the screens vector and calls
//	their update method. If it is determined that the screen is
//	on top and able to receive input information, pass that information
//	to the screen in order for it to handle its own input.
//
void ScreenController::updateView(float deltaTime)
{
	bool otherScreenHasFocus = false;
	bool coveredByOtherScreen = false;
	
	//	Read the touch input
	input->update( deltaTime );
	
	//	make a copy of hte master screen list, to avoid confusion if
	//	the process of updating one screens adds or removes others.
	screensToUpdate->clear();
	for ( screen = screens->begin(); screen != screens->end(); ++screen)
		screensToUpdate->push_back( (*screen) );
	
	//	since we are updating the screens "top to bottom" we need to
	//	reverse the order of the screens
	screensToUpdate->reverse();	
	
	
	//	Cycle through each screen that needs to be updated, and perform
	//	any updates needed
	for (screen = screensToUpdate->begin(); screen != screensToUpdate->end(); ++screen)
	{
		//update the screen
		(*screen)->update( deltaTime, otherScreenHasFocus, coveredByOtherScreen );
		
		if ( (*screen)->getScreenState() == TransitionOn ||
			 (*screen)->getScreenState() == Active )
		{
			//	if this is the first active screen we came across,
			//	give it a chance to handle input.
			(*screen)->handleInput( input );
			otherScreenHasFocus = true;
		}
		
		//	if this is an active non-popup, inform any subsequent 
		//	screens that they are covered by it
		if ( !(*screen)->getIsPopup() )
			coveredByOtherScreen = true;
	}
	
	//	do we need to print the debug trace?
	if ( traceEnabled )
		traceScreens();
}

//
//	draws a list of the active screens for debug purposes
//
void ScreenController::traceScreens() const
{
#warning Implement this function (ScreenController::traceScreens)
}

//
//	Cycles through each game screen in the screens vector and calls
//	their draw method. Since every game screen needs to be drawn and 
//	it is possible to draw some screens on top of other screens,
//	the screens are drawn from "bottom to top."
//
void ScreenController::drawView(float deltaTime) const
{	
	list<GameScreen *>::iterator drawScreen;
	
	for ( drawScreen = screens->begin(); drawScreen != screens->end(); ++drawScreen )
		(*drawScreen)->draw( deltaTime );
}

#pragma mark Adding / Removing a screen from the controller

//
//	when adding a screen to the screen manager, we will be
//	setting some default values for the screen and then
//	calling the screens "loadContent" function. Once everything
//	is loaded, the screen will be added to the screens list.
//
void ScreenController::addScreen ( GameScreen *gameScreen )
{	
	gameScreen->setViewport( viewport );
	gameScreen->setTransitionOnTime( TRANSITION_ON_TIME );
	gameScreen->setTransitionOffTime( TRANSITION_OFF_TIME );
	gameScreen->setScreenState( TransitionOn );
	gameScreen->setTransitionPosition( 1 );
	gameScreen->loadContent();
	
	screens->push_back( gameScreen );
}

//
//	Removes a screen from the game screens vector and cleans up
//	any memory associated with the screen by calling the screens
//	unloadContent function, and deleting the pointer associated
//	with the screen. This function will instantly kill a screen
//	so it is recommended that if the user wishes a screen to exit
//	gracefully, the client should call the screens "exit()" function.
//
void ScreenController::killScreen( GameScreen *gameScreen )
{
	//	unload the screens data
	gameScreen->unloadContent();
	
	//	remove the screen from the screens array
	screens->remove( gameScreen );
	screensToUpdate->remove( gameScreen );
	
	//	delete the screens memory
	delete gameScreen;
}

#pragma mark Transition Functions

//
//	Helper method to draw a translecent black fullscreen sprite, used
//	for fading screens in and out, and for darkening the background
//	behind pop up screens.
//
void ScreenController::fadeBackBufferToBlack( float alpha ) const
{
	glColor4f(alpha,alpha,alpha,alpha);
	blankTexture->drawInRect( viewport );
	glColor4f(1, 1, 1, 1);	
}
























