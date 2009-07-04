/*
 *  SoundController.h
 *  DjinnEngine
 *
 *  Created by Craig Giles on 6/3/09.
 *
 */

//	Sound Controller Class:
//	The Sound Controller is another class that I used Mark's blog as a staring point.
//	It uses OpenAL for sound effects, and currently does that rather well... however
//	in a future update I will be looking into adding OGG support. Mark's tutorials
//	used Objective-C classes to play music, and since this is a C++ engine, I needed
//	to figure out a way to stream music using OpenAL, and there are quite a few
//	tutorials for that online using OGG format, so that is the direction i will be headed
//	with the sound engine moving on.

#ifndef SOUNDCONTROLLER_H
#define SOUNDCONTROLLER_H

#define Sounds SoundController::getInstance()

#include "FileUtil.h"
#include "Log.h"
#include "Vector2.h"

#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <string>
#include <map>
#include <AudioToolbox/AudioToolbox.h>

class SoundController
{
public:
	SoundController();
	~SoundController();
	
	static SoundController* getInstance();

	void setBackgroundMusicVolume( const ALfloat theVolume );
	
	void loadSoundWithKey( const std::string &key, const std::string &asset, const std::string &extension, const ALuint frequency );
	void loadBackgroundMusicWithKey( const std::string &key, const std::string &asset, const std::string &extension );
	
	void playMusicWithKey( const std::string &key, const int timesToRepeat );
	ALuint playSoundWithKey( const std::string &key, const ALfloat gain, const ALfloat pitch, const Vector2 &location, const bool loop );
	
protected:
	static SoundController* instance;
	const int MAX_SOURCES;
	
	ALfloat musicVolume;
	
	// OpenAL context for playing sounds
	ALCcontext *context;
	
	// The device we are going to use to play sounds
	ALCdevice *device;
	
	// Array to store the OpenAL buffers we create to store sounds we want to play
	ALuint *sources;
	std::map<string, ALuint> soundLibrary;
	std::map<string, ALuint> musicLibrary;

	UInt32 getAudioSize( const AudioFileID &fileID );
	int getAvailableSource();
};



#endif