
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

#pragma mark
#pragma mark Constructor(s) / Destructor
#pragma mark ----------
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
	blankTexture->releaseReference();
	delete input;
	delete screens;
	delete screensToUpdate;
}

#pragma mark 
#pragma mark Setup view and Load Content
#pragma mark ----------
//	This function sets up OpenGL for how we wish to draw objects to the 
//	screen by editing the specific "modes" such as the projection
//	and model view matricies. 
void ScreenController::setupView()
{	
	//	Modify the Projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	//	Orthof projection is used for 2d games. This sets the coordinates to
	//	(0, 0) at the top left corner of the screen, and as you move downward
	//	your y value will increase. As you move to the right, your x value will
	//	increase.
	//	(left, right, bottom, top, near, far)
	//
	//	If the game is going to be played in landscape mode, enable
	//	it via the bool switch at the top of the ScreenController file.
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
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 

	//	Setup clear color (cornflower blue'ish)
	//	Call me crazy, but I got so used to this color developing
	//	for XNA. This is a little nod to the Microsoft Development
	//	Team :)
	glClearColor(.39f, 0.58f, 0.920f, 0.0f); 
}

void ScreenController::loadContent()
{
	screens = new list<GameScreen *>;
	screensToUpdate = new list<GameScreen *>;

	//	Cycle through all screens and instruct them to load their content
	for ( screen = screens->begin(); screen != screens->end(); ++screen)
		(*screen)->loadContent();

	
	//	Allocate memory for the input manager and the blankTexture used
	//	to fade the screen in / out.
	input = new InputController( LANDSCAPE_MODE );
	blankTexture = Textures->getTexture( "blankTexture" );
	blankTexture->addReference();

	isInitialized = true;
	traceEnabled = false;

#warning TitleScreen to the Screen Controller
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

#pragma mark
#pragma mark Update / Draw Screens
#pragma mark ----------
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


void ScreenController::traceScreens() const
{
#warning Implement this function (ScreenController::traceScreens)
}

void ScreenController::drawView(float deltaTime) const
{	
	list<GameScreen *>::iterator drawScreen;
	
	for ( drawScreen = screens->begin(); drawScreen != screens->end(); ++drawScreen )
		(*drawScreen)->draw( deltaTime );
}

#pragma mark 
#pragma mark Adding / Removing a screen from the controller
#pragma mark ----------

void ScreenController::addScreen ( GameScreen *gameScreen )
{	
	//	when adding a screen to the screen manager, we will be
	//	setting some default values for the screen and then
	//	calling the screens "loadContent" function. Once everything
	//	is loaded, the screen will be added to the screens list.
	gameScreen->setViewport( viewport );
	gameScreen->setTransitionOnTime( TRANSITION_ON_TIME );
	gameScreen->setTransitionOffTime( TRANSITION_OFF_TIME );
	gameScreen->setScreenState( TransitionOn );
	gameScreen->setTransitionPosition( 1 );
	gameScreen->loadContent();
	
	screens->push_back( gameScreen );
}

void ScreenController::killScreen( GameScreen *gameScreen )
{
	//	Removes a screen from the game screens vector and cleans up
	//	any memory associated with the screen by calling the screens
	//	unloadContent function, and deleting the pointer associated
	//	with the screen. This function will instantly kill a screen
	//	so it is recommended that if the user wishes a screen to exit
	//	gracefully, the client should call the screens "exit()" function.
	gameScreen->unloadContent();
	screens->remove( gameScreen );
	screensToUpdate->remove( gameScreen );
	
	delete gameScreen;
}

#pragma mark 
#pragma mark Transition Functions
#pragma mark ----------

void ScreenController::fadeBackBufferToBlack( float alpha ) const
{
#warning re-work this system
	//	Helper method to draw a translecent black fullscreen sprite, used
	//	for fading screens in and out, and for darkening the background
	//	behind pop up screens.
	blankTexture->draw(viewport, Color(alpha, alpha, alpha, alpha));
}
























