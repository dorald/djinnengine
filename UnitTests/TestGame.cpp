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
	
	
//	Textures->getTexture( "RunningLeft01" ); 
//	
//	
	//actor
	testActor = Actors->loadActor<Character>( "CHARACTER" );
	
	//load a particle effect. This should create a 'definition' of the particle effect for
	//	which all other effects of the same type will be copied.
	Particles->loadParticleEffect( "CAMPFIRE" );
//	
//	//	Starts a particle effect at the specific spot, with the specific variance, and
//	//	duration. This should use the "CAMPFIRE" effect as a template, make a new copy
//	//	of the effect, and start that effect in the location provided.
	Particles->startParticleEffect( "CAMPFIRE", Vector2( 100, 200 ), Vector2::ZERO(), 5 );
	Particles->startParticleEffect( "CAMPFIRE", Vector2( 120, 190 ), Vector2::ZERO(), 10 );
	Particles->startParticleEffect( "CAMPFIRE", Vector2( 140, 180 ), Vector2::ZERO(), 15 );
	Particles->startParticleEffect( "CAMPFIRE", Vector2( 160, 170 ), Vector2::ZERO(), 20 );
	
	Particles->startParticleEffect( "CAMPFIRE", Vector2( 300, 200 ), Vector2::ZERO(), -1 );
	Particles->startParticleEffect( "CAMPFIRE", Vector2( 280, 190 ), Vector2::ZERO(), -1 );
	Particles->startParticleEffect( "CAMPFIRE", Vector2( 260, 180 ), Vector2::ZERO(), -1 );
	Particles->startParticleEffect( "CAMPFIRE", Vector2( 240, 170 ), Vector2::ZERO(), -1 );
	
	
	Sounds->loadSound( "photon", 22050 );
	Sounds->playSound( "photon", .5, .5, Vector2::ZERO(), true );


	
	tex = Textures->getTexture( "RunningLeft01" );
	tex->addReference();
	
	
	
	
	initialized = true;						 
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
	Actors->draw( deltaTime );
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

void TestGame::testActorClass()
{
	//	IMPORTANT:
	//	In the game, the ActorController should be the only class that instanciates 
	//	an actor object. For example; you would call 
	//	Actors->loadActor( assetName, actorPointer );
	Actor *act = new Actor();
	
	act->setAlpha( .50f );
	
	//	identities are unique and assigned when the object is constructed.. Calling this function
	//	should not change the identity
	act->setIdentity( "newIdentity" ); 	
	act->setName( "Werewolf ");
	act->setPosition( Vector2::ZERO() );
	act->setSize( Vector2(32, 32) );
	act->update(.003f);
	
	//	One actors Update and Draw method will be called, since there is 
	//	one actor in the 'actors' map
	Actors->update( .003f );
	Actors->draw( .003f );

	delete act;
	act = NULL;
	
	//	Update and Draw still called, but now there are no actors
	//	in the 'actors' map
	Actors->update( .003f );
	Actors->draw( .003f );
	
}

void TestGame::testAnimationClass()
{
	Texture2D *testTexture;
	
	//	An array of Texture2D Pointers
	Texture2D* *frames = new Texture2D*[4];
	string *frameNames = new string[4];
	
	frameNames[0] = "RunningLeft01";
	frameNames[1] = "RunningLeft02";
	frameNames[2] = "RunningLeft01";
	frameNames[3] = "RunningLeft02";
	
	frames[0] = Textures->getTexture( "RunningLeft01" );
	frames[0]->addReference();
	frames[1] = Textures->getTexture( "RunningLeft02" );
	frames[1]->addReference();
	frames[2] = Textures->getTexture( "RunningLeft01" );
	frames[2]->addReference();
	frames[3] = Textures->getTexture( "RunningLeft02" );
	frames[3]->addReference();
	

	
	Animation *testAnim = new Animation();			//works
	
	testAnim->setAsset( "RUNNINGLEFT" );
	testAnim->setName( "RunningLeft" );//working
	testString = testAnim->getName();	//working
	
	testAnim->loadAnimation( frameNames, PNG, 4 );
	//anim->loadAnimation( frames, 4 );
	
	testAnim->setIsLoop( true );
	testBool = testAnim->getIsLoop();
	
	testAnim->setPauseAtEnd( false );
	testBool = testAnim->getPauseAtEnd();
	
	testAnim->setFrameInterval( .35f );
	testFloat = testAnim->getFrameInterval();
		
	testAnim->setIsLoop( false );
	testAnim->setPauseAtEnd( false );

	Animation *anim2 = new Animation;
	*anim2 = *testAnim;
	
	delete testAnim;
	testAnim = NULL;
	
	testTexture->releaseReference();
	
	delete [] frames;
	delete [] frameNames;
	
	anim2->setIsLoop( true );
	Files->serialize( "RUNNINGLEFT", *anim2 );
	
	
	Animation* testSerialize = new Animation;
	Files->unSerialize( "RUNNINGLEFT", *testSerialize );
	testSerialize->isAnimationLoaded();
}

void TestGame::testAnimationController()
{
	AnimationController *sprite = new AnimationController( "RUNNINGLEFT" );
	Rectangle *testSpriteBox = new Rectangle( 100, 300, 32, 32 );
	
	sprite->loadAnimation( "RUNNINGLEFT" );
	sprite->playAnimation( "RUNNINGLEFT" );
	
	sprite->togglePause();
	sprite->stopCurrentAnimation();
	sprite->jumpToFrame( 3 );
	
	sprite->update( .0015f );
	sprite->draw( .0015f, *testSpriteBox );
	
	sprite->removeAnimation( "RUNNINGLEFT" );
	
	delete testSpriteBox;
	delete sprite;
}

void TestGame::testCharacterClass()
{
}	

void TestGame::testParticleEngine()
{
//	Particles->addEffect( "CAMPFIRE" );
//	Particles->
}




















