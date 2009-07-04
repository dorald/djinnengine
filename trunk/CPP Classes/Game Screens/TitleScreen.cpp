/*
 *  TitleScreen.cpp
 *  CppBase
 *
 *  Created by Craig Giles on 3/6/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "TitleScreen.h"
#include "Actor.h"

TitleScreen::TitleScreen( const std::string &name, ScreenController *sm ) 
	: GameScreen( name, sm )
{
}

TitleScreen::~TitleScreen()
{
}

void TitleScreen::loadContent()
{
}
void TitleScreen::unloadContent()
{
}

void TitleScreen::handleInput( const InputController &input )
{
}

void TitleScreen::update(float deltaTime, bool otherScreenHasFocus, bool coveredByOtherScreen)
{
	
	
	GameScreen::update(deltaTime, otherScreenHasFocus, coveredByOtherScreen);
}
void TitleScreen::draw(float deltaTime)
{
}