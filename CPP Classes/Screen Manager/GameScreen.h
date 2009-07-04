//
//	Base GameScreen class
//	Djinn Engine
//

//	GameScreen Class:
//	A game screen is something like a "view" in the Model / View / Controller 
//	setup. Everything that happens in the game that has to be rendered to screen,
//	will be rendered in a game screen. The game screen is an abstract base class,
//	meaning you can not directly create "GameScreen *screen = new GameScreen.
//	A GameScreen must be inherited to be used.
//
//	Game Screens can be used in a variety of ways, but I have found that the
//	best way to use them is to split them up as much as you can. For example;
//	Lets say you have a MainGameScreen, and you press the 'start' button to bring
//	up a menu. That will be a "MenuScreen." In that menu, you bring up your inventory,
//	that will be an "InventoryScreen." All game screens will be managed by the 
//	ScreenController class.


#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <string>

#include "Rectangle.h"
#include "InputController.h"

using std::string;

//	Enum to describe the screens transition state
enum ScreenState {
	TransitionOn = 0,
	Active,
	TransitionOff,
	Hidden
};

//	forward declarations
class ScreenController;

class GameScreen
	{
	public:

		//	The constructor for the GameScreen doesn't do anything special.. All it
		//	really does is set this screens name to the name parameter, and sets
		//	a pointer to be able to find the ScreenController for anything it needs.
		GameScreen( const std::string &name, ScreenController *sm );
		GameScreen( const GameScreen &copy );
		GameScreen &operator=( const GameScreen &copy );
		virtual ~GameScreen();	//ensures destruction of any children classes
		
		//	Gets the pointer to the screen manager handling the update
		//	and draw function calss to this screen.
		//	<<TODO: Screen manager will be converted to a singleton,
		//		therefor this can be replaced by a 'ScreenController::getInstance()'
		//		function call.>>
		ScreenController *getScreenController() const { return screenController; }
		
		//	Get the name of this screen.
		//	IE: "Title Screen" "Gameplay Screen" "High Scores"
		const string &getName() const { return name; }
		
		//	Get the viewport used to draw the screen to. 
		const Rectangle getViewport() const { return viewport; }
		void setViewport(const Rectangle &value);
		
		//	Is the screen a popup screen like a message box, or does
		//	it cover the entire viewport? There is no setter for popup, 
		//	since it can only be one or the other. isPopup is set in 
		//	the constructor.
		const bool getIsPopup() const { return isPopup; }
		
		//	The length of time this screen needs to fade in before
		//	its fully active
		const float getTransitionOnTime() const { return transitionOnTime; }
		void setTransitionOnTime(const float &value);
		
		//	The length of time this screen needs to fade out before
		//	it can be deactivated and removed from the screen manager
		const float getTransitionOffTime() const { return transitionOffTime; }
		void setTransitionOffTime(const float &value);
		
		//	The current position of the screens transition. 
		//	1.0 = Fully On
		//	0.0 = Fully Off
		const float getTransitionPosition() const { return transitionPosition; }
		void setTransitionPosition(const int &value);
		
		//	How transparent is this screen. Mainly used when transitioning
		//	on and off, although some screens may utilize this for special
		//	effects (Such as the paused screen)
		const float getTransitionAlpha() const { return transitionAlpha; }
		
		//	Gets the current screen state.
		//		TransitionOn - Screen is still transitioning on and is not fully active
		//		Active	- Screen has finished transitioning, and is fully active
		//		TransitionOff - Screen is still transitioning and should not be removed
		//		Hidden - Screen is finished transitioning off, and can be removed
		const ScreenState getScreenState() const { return currentScreenState; }
		void setScreenState(const ScreenState &value);
		
		//	overloaded operators that determine if this screen is ==, or != another.
		bool operator==(const GameScreen &value) const;
		bool operator!=(const GameScreen &value) const;
				
		//	Loads all content associated with the game screen. This would
		//	include textures, game models, fonts, levels, etc.
		virtual void loadContent();
		
		//	Unloads and deallocates all content associated with this game screen
		virtual void unloadContent();
		
		//	Handle the current input state. This is a pure virtual function and needs
		//	to be overridden in any derived classes, since the base GameScreen class
		//	has no need to handle input.
		virtual void handleInput( const InputController &input ) = 0;
		
		//	This function will update the transition between gameScreens, while any
		//	game screen that inherits this function will override the function to
		//	handle the game logic for the screen. Once that game screen's update function is
		//	complete, it will call this update function to transition between screens if needed.
		virtual void update( float deltaTime, bool otherScreenHasFocus, bool coveredByOtherScreen );
		
		//	This function will handle the fade in / fade out aspect of the gameScreen.
		//	Any game screen that overrides this function will handle the drawing of
		//	that game screen, then callt his function in order to handle any fade in/fade out
		//	effects
		virtual void draw( float deltaTime );

		//	Calling this function will tell the screen to transition out. Once the screen
		//	has finished transitioning out, the screen will tell the ScreenController to remove 
		//	the screen from the update lists, and deallocate any memory associated with the screen.
		void exitScreen();
		
	protected:
		
		string name;
		ScreenController *screenController;
		Rectangle viewport;
		
		bool hasBeenUnloaded;
		bool isPopup;
		
		float transitionOnTime;
		float transitionOffTime;
		float transitionPosition;
		float transitionAlpha;
		ScreenState currentScreenState;
		
		bool isExiting;
		bool isActive;
		bool otherScreenHasFocus;
		
		bool updateTransition(float deltaTime, float transition, int direction);
	};

#endif