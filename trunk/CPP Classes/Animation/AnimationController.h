/*
 *  AnimationController.h
 *  DjinnEngine
 *
 *  Created by Craig Giles on 4/6/09.
 *
 */

//	Animation Controller:
//	This is a class designed to handle the update and rendering of
//	a visable sprite with animations. My first idea for an animation system was
//	to store all active animations in a "playlist" where any new animation, if it
//	didn't loop or pause at the end, would be added to a stack. Once that animation
//	was finished, it would be pop'd off the stack and the previous animation would
//	resume. I may end up re-vamping the system to take advantage of something along
//	those lines, but for now, I've settled with the current / previous / default
//	animation pointers.
//
//	As it stands now, the AnimationController does NOT have an overload = correctly
//	implemented, so if there is ever a situation where "Character1 = Character2",
//	bad things may happen. I was thinking of when it would be appropriate to do such
//	a thing, and why the client wouldn't simply just un-serialize a character, but
//	when I get to it I may just implement it and if it gets used.. it will get used.
//
//	A note about loading vs playing of an animation: When Loading an animation, new
//	memory will be allocated and the animation will be put into a map with the
//	animation name as its key. If its the first animation to be placed in the map,
//	that animation will also be the 'defaultAnimation' for the character. PLAYING
//	an animation, will NOT create new memory, in fact it will simply just point the
//	currentAnimation pointer to the instance in the map, and reset all values for
//	the animation such that it starts from the beginning. The big difference is
//	the memory allcoation, loading creates memory, while playing does not.

#ifndef ANIMATIONCONTROLLER_H
#define ANIMATIONCONTROLLER_H

#include "Texture2D.h"
#include "Animation.h"
#include "Rectangle.h"

#include <string>
#include <map>
#include <vector>

using std::vector;
using std::string;
using std::map;

class AnimationController
{
public:
	//	default constructor sets all the values to standard values, and
	//	initializes the animations to NULL. The constructor that takes in
	//	the defaultAssetName will load in the default animation and initialize
	//	it to be associated with the defaultAnimation pointer.
	AnimationController();
	AnimationController( const string &defaultAssetName );
	~AnimationController();
	
	//	Loads an animation into memory, and reads the animation data from
	//	the asset file hosted in the resources directory. This function will
	//	allocate memory for the animation, populate the animation with the 
	//	appropriate data, and store the animation into a map file waiting
	//	to be played. 
	//
	//	NOTE: If this is the first animation to be loaded, it will automatically
	//	be set as the characters "default animation" Therefore, in the animation
	//	serialization .dat file, the first animation should ALWAYS be set to the
	//	intended default animation.
	void loadAnimation( const string &asset );
	
	//	find the value in the map, and deallocate memory associated
	//	with the iterator. Once the memory has been dealocated, erase
	//	the value from the map.
	void removeAnimation( const string &name );
	
	//	Checks to see if the current frame count is greater than the ending frame,
	//	and if it is; the animation is complete and a 'true' statement is returned.
	bool isAnimationComplete() { return frame > endingFrame; }
	
	//	Checks to see if the animation is currently being played, and if not, set
	//	the animation to the active (current) animation. 
	void playAnimation( const string &name );
	
	//	play the default animation
	void playDefaultAnimation() { playAnimation( defaultAnimation ); }
	
	//	Pausing the animation will keep it from updating. It will stay on the same
	//	frame until the animation is un-paused.
	void togglePause();
	
	//	Stop the current animation and begin playing the previously played animation.
	//	For example, if your character was running, and then began to swing their
	//	sword.. when your attack animation stops, it will continue the run animation.
	//	When this function is called, the currently played animation is NOT deleted or
	//	deallocated, it is mearly stopped.
	void stopCurrentAnimation();
	
	//	Jump to a specific frame in the current animation. This function will clamp the
	//	value being passed into it between 0 and the ending frame of the animation.
	//	if a number greater than the ending frame is passed in, the animation will
	//	jump to the ending frame.
	void jumpToFrame( const int value );
	
	//	update the animation interval and determine if the animation should switch
	//	to the next frame. If so, 'incrementFrame()' is called and the timer is reset.
	void update( const float deltaTime );
	
	//	Draw the current animation to screen inside the specified spritebox.
	void draw( const float deltaTime, const Rectangle &spriteBox );
	
private:
	//	we'll make the sprite controller private for now. In order to make a copy of this
	//	object, you should load whatever is housing the sprite contorller from data file.
	AnimationController& operator =( const AnimationController &copy );
	AnimationController( const AnimationController &copy );
	
	Animation* currentAnimation;
	Animation* previousAnimation;
	Animation* defaultAnimation;
	
	map<string, Animation*> availableAnimations;

	int frame;
	int endingFrame;
	
	float currentInterval;
	float frameInterval;
	
	bool paused;
	
	void playAnimation( Animation* value );
	bool isAnimationPlaying( const string &name );
	void incrementFrame();
	void resetAnimationVariables();
};

#endif