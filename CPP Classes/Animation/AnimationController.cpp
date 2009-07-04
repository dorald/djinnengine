/*
 *  AnimationController.cpp
 *  DjinnEngine
 *
 *  Created by Craig Giles on 4/6/09.
 *
 */

#include "AnimationController.h"

#pragma mark Constructor(s) / Destructor

AnimationController::AnimationController()
{
	currentAnimation = NULL;
	previousAnimation = NULL;
	defaultAnimation = NULL;
	
	frame = 0;
	endingFrame = 0;
	currentInterval = 0;
	frameInterval = 0;
	paused = false;
}

AnimationController::AnimationController( const string &defaultAssetName )
{
	currentAnimation = NULL;
	previousAnimation = NULL;
	
	frame = 0;
	endingFrame = 0;
	currentInterval = 0;
	frameInterval = 0;
	paused = false;
	
	defaultAnimation = new Animation;
	Files->unSerialize( defaultAssetName, *defaultAnimation );
	availableAnimations[ defaultAnimation->getName() ] = defaultAnimation;
	
	//	Since the animation object was just created, the 'currentAnimation' will
	//	be NULL. We should play the default animation. 
	playAnimation( defaultAnimation );
}

AnimationController::AnimationController( const AnimationController &copy )
{
	*this = copy;
}

AnimationController::~AnimationController()
{
	//	Clean up memory associated with allocation of animation pointers
	//	and storing them into the map.
	map<string, Animation*>::iterator it;
	for ( it = availableAnimations.begin(); it != availableAnimations.end(); ++it )
	{
		delete it->second;
		it->second = NULL;
		availableAnimations.erase( it );
	}
	
	//	once the map has been cleaned up, delete the animation pointers for
	//	current, previous, and default animations... then set them to NULL.
	delete defaultAnimation;
	delete currentAnimation;
	delete previousAnimation;
	
	defaultAnimation = NULL;
	currentAnimation = NULL;
	previousAnimation = NULL;
}



AnimationController& AnimationController::operator =( const AnimationController &copy )
{
	if ( this == &copy )
		return *this;

#warning Since this function is private, no copies of the animation controller can be made. Should a character1 = character2 happen, this is kept here as a warning.
	
	return *this;
}

# pragma mark Functions

#pragma mark Loading and Removing an animation
void AnimationController::loadAnimation( const string &asset )
{
	//	Allocate memory for the new animation
	Animation *newAnimation = new Animation;
	
	//	Unserialize the animation and populate its data
	FileUtil::getInstance()->unSerialize( asset, *newAnimation );
	
	//	store the animation into the availableAnimations map file
	availableAnimations[ newAnimation->getName() ] = newAnimation;
	
	//	if the default animation has not been set, set it to
	//	this animation.
	if ( defaultAnimation == NULL )
		defaultAnimation = newAnimation;
}

void AnimationController::removeAnimation( const string &name )
{
	//	if the currently played animation is the one we're trying
	//	to remove, we should stop playing the current animation.
	if ( currentAnimation->getName() == name )
		stopCurrentAnimation();
	
	//	if the default animation is the animation we're trying
	//	to remove, we should set the default animation to NULL
	if ( defaultAnimation->getName() == name )
		defaultAnimation = NULL;

	//	Find the animation to be removed in the map
	map<string, Animation*>::iterator it;
	it = availableAnimations.find( name );
	
	//	deallocate memory that was allocated for the animation
	delete it->second;
	it->second = NULL;
	
	//	erase the animation from the map
	availableAnimations.erase( it );
}

#pragma mark Animation playback.
void AnimationController::playAnimation( const string &name )
{
	//	if the animation is currently playing, there is no
	//	need to load it again. Exit out of the function.
	if ( isAnimationPlaying( name ) )
		return;
	
	map<string, Animation*>::iterator it;
	it = availableAnimations.find( name );
	playAnimation( it->second );
}

void AnimationController::playAnimation( Animation* value )
{
	previousAnimation = currentAnimation;
	currentAnimation = value;
	
	resetAnimationVariables();
}

void AnimationController::togglePause()
{
	//	If its currently paused, un-pause.
	//	if its currently un-paused, pause.
	if ( paused == true )
		paused = false;
	else
		paused = true;
}

void AnimationController::stopCurrentAnimation()
{
	//	cycle the animations so the current animation
	//	is the previously active animation, and the
	//	previous animation is the current animation.
	Animation *tempAnimation = currentAnimation;
	currentAnimation = previousAnimation;
	previousAnimation = tempAnimation;
	
	//	If there is no currently playing animation after
	//	the swap takes place, load the default animation.
	if ( currentAnimation == NULL )
		playAnimation( defaultAnimation );
	
	resetAnimationVariables();
}

void AnimationController::resetAnimationVariables()
{
	paused = false;
	currentInterval = 0;
	frame = 0;
	frameInterval = currentAnimation->getFrameInterval();
	endingFrame = (currentAnimation->getNumberOfFrames() - 1);
}

void AnimationController::jumpToFrame( const int value )
{
	frame = MathUtil::Clamp( value , 0, endingFrame );
}

bool AnimationController::isAnimationPlaying( const string &name )
{
	if ( currentAnimation == NULL )
		return false;
	
	else if ( currentAnimation->getName() == name )
		return true;
	
	return false;
}

#pragma mark Update and Draw functions.

void AnimationController::update( const float deltaTime )
{
	//	if the animation is paused, do not update
	if ( paused )
		return;
	
	//	if the animation is not paused, lets update the animation.
	currentInterval += deltaTime;
	
	//	if current interval is greater than the frame interval,
	//	we need to move to the next frame of animation.
	if ( currentInterval >= frameInterval )
	{
		incrementFrame();
		
		//reset the clock
		currentInterval -= frameInterval;
	}
}

void AnimationController::draw( const float deltaTime, const Rectangle &spriteBox )
{	
	Texture2D* spriteToDraw = currentAnimation->getSpriteToDraw( frame );
	spriteToDraw->drawInRect( spriteBox );
}

void AnimationController::incrementFrame()
{
	
	frame++;
	
	//	if we've reached the end of the animation
	if ( isAnimationComplete() )
	{
		//if animation is loopable, loop it
		if ( currentAnimation->getIsLoop() )
			jumpToFrame( 0 );
		
		//otherwise, if it pauses at end, pause it
		else if ( currentAnimation->getPauseAtEnd() )
			frame = endingFrame;
		
		//otherwise, set currentFrame = endFrame + 1
		//	flagging it as "complete"
		else
			jumpToFrame( endingFrame + 1 );
		
	}
}