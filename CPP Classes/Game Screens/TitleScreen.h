/*
 *  TitleScreen.cpp
 *  CppBase
 *
 *  Created by Craig Giles on 3/6/09.
 *
 */

//	TitleScreen
//	This is an example of how a game screen will be implemented in the engine.
//	the title screen right now is not a real title screen, but mearly a view
//	created by me to test the various aspects of the game. Eventually
//	it will be changed to a true title screen, and also other screens 
//	can be modeled from this one (IE: Loading Screen, Gameplay Screen,
//	Inventory Screen, etc etc etc)

#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include "GameScreen.h"
#include <string>

class TitleScreen : public GameScreen	
{
public:
	//	Constructor / Destructor(s)
	//	If the transition on and off by the title screen is different than
	//	the default values, it should be set in the constructor
	TitleScreen( const std::string &name, ScreenController *sm );
	~TitleScreen();
	
	//	Load and unloads all content associated with the title screen. 
	//	Includes, any sound files or effects, textures, etc.
	void loadContent();
	void unloadContent();
	
	//	Uses the pointer to the input manager to handle any input
	//	specific to this screen
	virtual void handleInput( const InputController &input );
	
	//	Called once per frame, updates all logic associated with this game screen.
	//	this could inclue animations with textures, music and sound effect playback,
	//	logic checking with any in-game objects, etc.
	void update( float deltaTime, bool otherScreenHasFocus, bool coveredByOtherScreen );
	
	//	Called once per frame, this function will render any visable actors to screen.
	void draw(float deltaTime);
};

#endif
