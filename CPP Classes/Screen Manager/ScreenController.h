//	Screen Controller Class:
//
//	The Screen Controller is a complex screen management system. It was first taken
//	from the GameState Management System in XNA, and later changed to meet my needs,
//	however the implementation remains similar to the XNA system.
//
//	The Screen Controller will manage a set of screens, and display (in order) and
//	update the screens that need updating, while also filtering input to the screens
//	that have control. For example, lets say you have a game started (MainGameScreen)
//	and in that game you have the Option Menu open (OptionMenuScreen) and within that
//	option menu, there is a popup asking "Are you sure you want to change so.and.so"
//	(ConfirmationMenuScreen) ... well the screen manager will be managing three screens,
//	and possibly filtering input to the active screen which should be the Confirmation
//	window. All of the screens will need to be updated (animations in the main game screen
//	for example) and chances are, only 2 of the 3 screens will need to be drawn (since
//	the options menu will possibly take up the entire screen, but the confirmation
//	menu only requires a portion of the screen... therefore the main game is completly
//	hidden and does not need to be drawn, since it can't be seen anyway)
//
//	The Screen controller takes care of all of these details, all you have to do
//	is take advantage of them.

#ifndef SCREENCONTROLLER_H
#define SCREENCONTROLLER_H

#define Screens ScreenController::getInstance()

#define DEBUG

//
//	OpenGL headers
//
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

//
//	utility headers
//
#include "RNG.h"
#include "Texture2D.h"
#include "Rectangle.h"
#include "InputController.h"
#include <list>
using std::list;

#include "TitleScreen.h"

//
//	Unit Test Headers
//
#include "TestGame.h"

//
//	forward declarations
//
class GameScreen;

class ScreenController
{
public:
	static ScreenController* getInstance();
	
	// --------------------------------------------------
	//	Constructor(s) / Destructor
	// --------------------------------------------------
	ScreenController();
	~ScreenController();
	
	// --------------------------------------------------
	//	Public Variables
	// --------------------------------------------------
	const bool LANDSCAPE_MODE;
	const float TRANSITION_ON_TIME;
	const float TRANSITION_OFF_TIME;
	
	// --------------------------------------------------
	//	mutators
	// --------------------------------------------------
	InputController *getInputController() const { return input; }
	Rectangle getViewport() const { return viewport; }

	//	The screen manager is the only class available to change
	//	the game screens array, so it must be passed as a const
	const list<GameScreen *> *getGameScreens() const { return screens; }
		
	// --------------------------------------------------
	//	Public methods
	// --------------------------------------------------
		
	//	This function sets up OpenGL for how we wish to draw objects to the 
	//	screen by editing the specific "modes" such as the projection
	//	and model view matricies. 
	void setupView();
		
	//	This function will load any content needed to get the game started 
	//	(IE: Splash screens, title screens, specific sounds, etc) The majority
	//	of the game will be loaded when the player enters the game, but this
	//	function will load what is needed to get the player started.
	void loadContent();
		
	//	Add a screen to the screens stack, letting the ScreenController run update
	//	and draw calls when needed.
	void addScreen( GameScreen *screen );
		
	//	This function will immediatly kill a screen from the screens array, and
	//	unload all content associated with the screen, and deallocate the memory
	//	used for the screen. It is recommended using the gameScreens exit function
	//	which respects transition times.
	void killScreen( GameScreen *gameScreen );
		
	//	Cycles through each game screen in the screens vector and calls
	//	their update method. If it is determined that the screen is
	//	on top and able to receive input information, pass that information
	//	to the screen in order for it to handle its own input.
	void updateView( float deltaTime );
		
	//	Cycles through each game screen in the screens vector and calls
	//	their draw method. Since every game screen needs to be drawn and 
	//	it is possible to draw some screens on top of other screens,
	//	the screens are drawn from "bottom to top."
	void drawView( float deltaTime ) const;
		
	//	Renders the screens list containing all of the screen names onto the
	//	game screen. This is used primarly for debug purposes, when needing
	//	to know if screens are added and removed properly, and in which order
	//	they appear in the lists.
	void traceScreens() const;
		
	//	Helper method to draw a translecent black fullscreen sprite, used
	//	for fading screens in and out, and for darkening the background
	//	behind pop up screens.
	void fadeBackBufferToBlack( float alpha ) const;
		
private:
	static ScreenController* instance;
	Texture2D *blankTexture;
	InputController *input;
	list<GameScreen *>::iterator screen;
		
	Rectangle viewport;
	
	list<GameScreen *> *screens;
	list<GameScreen *> *screensToUpdate;
		
	bool isInitialized;
	bool traceEnabled;	
};

#endif