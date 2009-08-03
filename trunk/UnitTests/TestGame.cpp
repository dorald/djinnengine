/*
 *  TestGame.cpp
 *  
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

#include "TestGame.h"

TestGame* TestGame::instance = 0;// initialize pointer

TestGame *TestGame::getInstance()
{
	if ( !instance )
		instance = new TestGame();
	
	return instance;
}

void TestGame::startTesting()
{
	if ( initialized )
		return;
	
	//	The idea behind testing, is writing these tests first. Once you write the test
	//	and write the function calls based on how the object should work, you then write
	//	the 
	
	//testMathUtilities();		//working
	//testActorClass();			//working
	//testAnimationClass();		//working
	//testAnimationController();
	//testCharacterClass();
	testParticleEngine();
}


// ---------------------------------------------------------------------------------------------------------
//	TestGame game loop. For use when testing things that need
//	update and draw calls after the class is constructed and working
//
void TestGame::testLoadContent()
{
	if ( initialized )
		return;
	
	testActors();
	testParticleEngine();
	//testSounds();
	
	initialized = true;						 
}

void TestGame::testSounds()
{
	Sounds->loadSound( "photon", 22050 );
	Sounds->playSound( "photon", .5, .5, Vector2::ZERO(), true );	
}

void TestGame::testParticleEngine()
{
	//load a particle effect. This should create a 'definition' of the particle effect for
	//	which all other effects of the same type will be copied.
	Particles->loadParticleEffect( "CAMPFIRE" );
	
	//	Starts a particle effect at the specific spot, with the specific variance, and
	//	duration. This should use the "CAMPFIRE" effect as a template, make a new copy
	//	of the effect, and start that effect in the location provided.
//	Particles->startParticleEffect( "CAMPFIRE", Vector2( 100, 200 ), Vector2::ZERO(), 5 );
//	Particles->startParticleEffect( "CAMPFIRE", Vector2( 120, 190 ), Vector2::ZERO(), 10 );
//	Particles->startParticleEffect( "CAMPFIRE", Vector2( 140, 180 ), Vector2::ZERO(), 15 );
//	Particles->startParticleEffect( "CAMPFIRE", Vector2( 160, 170 ), Vector2::ZERO(), 20 );
//	
//	Particles->startParticleEffect( "CAMPFIRE", Vector2( 300, 200 ), Vector2::ZERO(), -1 );
//	Particles->startParticleEffect( "CAMPFIRE", Vector2( 280, 190 ), Vector2::ZERO(), -1 );
//	Particles->startParticleEffect( "CAMPFIRE", Vector2( 260, 180 ), Vector2::ZERO(), -1 );
	Particles->startParticleEffect( "CAMPFIRE", Vector2( 240, 170 ), Vector2::ZERO(), -1 );
}

void TestGame::testActors()
{
	//actor
	testActor = Actors->loadActor<Character>( "CHARACTER" );
	testActor = Actors->loadActor<Character>( "ACTOR" );
	testActor = Actors->loadActor<Character>( "ACTOR2" );
	testActor = Actors->loadActor<Character>( "ACTOR3" );
	
	tex = Textures->getTexture( "RunningLeft01" );
	tex->addReference();
}

void TestGame::unloadContent()
{ 
	//Actors->removeActor( testActor );
}

void TestGame::testUpdate( const float deltaTime )
{
	Actors->update( deltaTime );
	Particles->update( deltaTime );
}

void TestGame::testDraw( const float deltaTime )
{	
	Textures->begin();
	Actors->draw( deltaTime );
	Textures->end();
	
	Particles->draw(deltaTime);
}

//
//	End TestGame game loop.
// ---------------------------------------------------------------------------------------------------------

void TestGame::testMathUtilities()
{
	int result = 0;
	
	result = MathUtil::Abs<int>( -30 );			//should = 30
	result = MathUtil::Max<int>( 50, -30 );		//should = 50
	result = MathUtil::Min<int>( 50, -30 );		//should = -30
	result = MathUtil::Distance( 35, 103 );		//should = 68
	result = MathUtil::Lerp(100 , 200, 0.5f);	//should = 150
	result = MathUtil::Clamp( result, 0, 5 );	//should = 5
		
	result = 0;
}


















