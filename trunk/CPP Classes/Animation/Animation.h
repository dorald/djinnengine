/*
 *  Animation.h
 *  Djinn Engine
 *
 *  Created by Craig Giles on 4/5/09.
 *
 */

//	Animation class
//	This is one of the first complex classes I designed for the engine.
//	The Animation class is fairly easy to grasp the concept of, but was 
//	quite difficult to design since I wasn't sure how I wanted to control
//	it. It is a Model portion of the Model / View / Controller in the Animation
//	series.
//
//	The functions and everything are fairly well commented, so if there is
//	any question on how things work or how they were intended to work, the
//	comments should lead you in the right direction.
//
//	Animation Object Serialization Guide:
//	Asset Name:					RUNNINGLEFT
//	Animation Name:				RUNNINGLEFT
//	# Frames:					4
//	Image Type Enum (0 = PNG):	0
//	List of image asset names:	RunningLeft01
//								RunningLeft02
//								RunningLeft03
//								RunningLeft04
//	Time Interval:				0.19
//	Loop bool:					1
//	Pause at end bool:			0

#ifndef ANIMATION_H
#define ANIMATION_H

#include "Serializable.h"
#include "TextureController.h"
#include "Texture2D.h"
#include "MathUtil.h"
#include "FileUtil.h"

#include <string>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <fstream>

using std::ostringstream;
using std::ofstream;
using std::ios_base;
using std::string;

class Animation : public Serializable
	{
	public:
		//	The file extension and applications folder used
		//	for animation data files.
		static string FileExtension;
		
		//	Overloaded >> operator used to populate the new animation 
		//	object being constructed.
		friend std::istream& operator >>( std::istream &inStream, Animation &value );
		friend std::ostream& operator <<( std::ostream &inStream, Animation &value );
		
		//	Constructor(s) / destructor
		Animation();
		Animation( const Animation &copy );
		~Animation();
		
		//	Assignment operator: creates new dynamic memory and stores
		//	the values of the copy in the 'this' object.
		Animation& operator =( const Animation &copy );
		
		//	Checks the name of both animations and returns true if they
		//	are the same. The names are all converted to uppercase so
		//	the check is case insensitive.
		bool operator ==( const Animation &value ) const;
		
		//	calls !( this == value )
		bool operator !=( const Animation &value ) const;
		
		//	Returns the Texture2D object at the specified value in the frames
		//	array. If the value is out of bounds of the array, an exception 
		//	will be thrown.
		Texture2D* operator []( const int value ) const;
		Texture2D* getSpriteToDraw( const int value ) const;
		
		//	Takes an array of strings housing the asset names of images,
		//	and loads a Texture2D into memory for each asset name. Once
		//	the Texture2D objects are loaded, the overloaded loadAnimation
		//	function is called, which sets the frames array = the newly
		//	created memory. 
		void loadAnimation( const string *imageNames, const ImageType &type, const int numberOfFrames );
		
		//	Returns true if this animation has frames associated to it. If the frames
		//	are set to NULL, the animation is not loaded and false is returned. 
		bool isAnimationLoaded() { return frames != NULL; }
		
		//	The name of the animation, hosted as a string in uppercase. When the name
		//	is set, it is passed by value in order to conver the string to uppercase,
		//	then set the name accordingly. 
		string getName() const { return name; } 
		void setName( string value );
		
		//	returns a copy of the pointer to string array hosting the image
		//	asset names array.
		string* getImageAssets() const { return imageNames; }
		
		//	The format of the images used for this animation, so we know how to load
		//	them into memory. IE: PNG, JPEG, etc.
		ImageType getImageType() const { return imageType; }
		void setImageType( const ImageType value ) { imageType = value; }
		
		//	Does this animation loop once the ending frame has been reached?
		//	Loop has a higher presidence than PauseAtEnd. If isLoop = true, but
		//	the animation is also set to pauseAtEnd = true, it will not pause
		//	instead it will loop.
		bool getIsLoop() const { return isLoop; }
		void setIsLoop( const bool value ) { isLoop = value; }
		
		//	Does this animation pause at the ending frame? 
		//	Loop has a higher presidence than PauseAtEnd. If isLoop = true, but
		//	the animation is also set to pauseAtEnd = true, it will not pause
		//	instead it will loop.
		bool getPauseAtEnd() const { return pauseAtEnd; }
		void setPauseAtEnd( const bool value ) { pauseAtEnd = value; }
		
		//	Gets the time interval between frames. The time interval is the
		//	amount of time the frame is shown before the next frame is drawn.
		float getFrameInterval() const { return frameInterval; }
		void setFrameInterval( const float value ) { frameInterval = value; }

		//	Gets the number of frames in this animation. From 1 to MaxFrames,
		//	therefore if you're using this as an index to your array, you should
		//	use frames[ numberOfFrames - 1 ]
		int getNumberOfFrames() const { return numberOfFrames; }
		void setNumberOfFrames( const int value ) { numberOfFrames = value; }

	private:
		string name;				//name of animation
		string *imageNames;			//array of image asset names used for textures
		ImageType imageType;		//What compression does the images use
		Texture2D* *frames;			//array of textures 
		int numberOfFrames;			//number of frames in this animation
		float frameInterval;		//time between frames
		bool isLoop;				//does animation loop
		bool pauseAtEnd;			//does animation pause at end
		
		//	Takes an array of pointers to Texture2D objects, and sets the frames
		//	array to point to the texture2D objects. This function DOES NOT allocate
		//	memory (unlike the loadAnimation function above) so while you can
		//  delete the array of Texture2D objects created to pass into this function,
		//	you should not delete the Texture2D objects themselves. 
		void loadAnimationFrames( Texture2D **images, const int numberOfFrames );
	};

#endif