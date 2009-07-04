

#ifndef DJINNENGINE_H
#define DJINNENGINE_H

//
//	Actors
//
#include "ActorController.h"
#include "Actor.h"					//non-character base class (IE: Traps)
#include "Character.h"				//character base class (NPCs, Vendors, etc)

//
//	Animation
//
#include "AnimationController.h"	//Animated Sprite Controller
#include "Animation.h"				//Individiual Animation

//
//	Input Management
//
#include "InputController.h"		//Input Controller for touch controls
#include "InputState.h"				//Individual state for each input

//
//	Screen Management
//
#include "ScreenController.h"		//Screen controller for game flow
#include "GameScreen.h"				//Base class for individual screens

//
//	Sound Management	
//
#include "SoundController.h"		//OpenAL Sound Controller

//
//	Utility Classes
//
#include "Serializable.h"			//object serialization
#include "OpenGlCommon.h"			//OpenGL Headers
#include "RNG.h"					//Random Number Generator
#include "Texture2D.h"				//OpenGL Texture
#include "Rectangle.h"				//Rectangle (x y width height)
#include "FileUtil.h"				//File Handling
#include "Log.h"					//Log handling (log files)
#include "MathUtil.h"				//Math Helper Functions
#include "Vector2.h"				//Vector ( x y ) location

#endif
