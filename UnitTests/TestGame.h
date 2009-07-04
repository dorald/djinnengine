/*
 *	I have been using the TestGame class as a static unit test in order to learn
 *	how to do unit testing. The idea was to create the tests, write the functions
 *	for whatever I was testing to ensure they pass, then write the functions
 *	to do what I intended for the function to do. This way it was easier to create
 *	the implementation, and if I changed anything I can just uncomment the test
 *	and re-test.
 *	
 *  Djinn Engine
 *
 *  Created by Craig Giles on 3/6/09.
 *
 */

#ifndef TESTGAME_H
#define TESTGAME_H

//	Use this to gain access to the UnitTests provided by the static test client;
#define UnitTests TestGame::getInstance()

#include "Actor.h"
#include "FileUtil.h"
#include "ActorController.h"
#include "Texture2D.h"
#include "MathUtil.h"
#include "Animation.h"
#include "AnimationController.h"
#include "Character.h"
#include "SoundController.h"
#include "ParticleController.h"
#include "ParticleEmitter.h"

#include <string>

using std::string;

class TestGame
	{
	public:
		//	Constructor and Destructor don't really do anything special
		//	since this is a singleton class. Only one instance of a
		//	TestGame object can ever be created.
		TestGame() { initialized = false; }
		~TestGame() { }
				
		//	In order to interact with the TestGame object, you must
		//	obtain its instance.
		static TestGame *getInstance();
		
		//	This function will initialize testing of whatever needs to be tested.
		//	It will be nothing but a list of function calls to the test functions
		//	IE: testMathUtilities(). 
		void startTesting();
		
		//	This is the test game loop. If i wanted to test anything a class does
		//	within a game loop, I would use this loop as an easy way to do just that
		void testLoadContent();
		void unloadContent();
		void testUpdate( const float deltaTime );
		void testDraw( const float deltaTime );
		
	private:
		static TestGame* instance;
		bool initialized;
		
		//	Test variables that can be re-used if needed.
		bool testBool;
		int testInt;
		float testFloat;
		string testString;		
		
		//	Tests the math utilities located in the MathUtil.cpp class.
		void testMathUtilities();
		
		//	Tests the Actor and ActorController class
		void testActorClass();
		Character *testActor;
		
		//	Tests the creation, update, and draw of an animation and the 
		//	eventual execution of the AnimationController class.
		void testAnimationClass();
		void testAnimationController();
		AnimationController *anim;
		
		void testCharacterClass();	
		
		void testParticleEngine();
	};

#endif