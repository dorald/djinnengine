/*
 *  SoundController.cpp
 *  DjinnEngine
 *
 *  Created by Craig Giles on 6/3/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "SoundController.h"

#pragma mark
#pragma mark Singleton Pattern
#pragma mark ----------
//	Soundcontroller instance pointer
SoundController* SoundController::instance = 0;

SoundController* SoundController::getInstance()
{
	//	If no instance has been created, create one
	if ( instance == 0 )
		instance = new SoundController;
	
	//	otherwise, just return the instance
	return instance;
}

#pragma mark
#pragma mark Constructor(s) / Destructor
#pragma mark ----------

SoundController::SoundController() : MAX_SOURCES( 32 )
{
	device = NULL;
	context = NULL;
	sources = NULL;
	
	device = alcOpenDevice( NULL );
	
	if( !device )
	{
		Log::write("SoundController::default device not found");
		exit(1);
	}
	
	// Create an OpenAL Context
	context = alcCreateContext( device, NULL );	
	alcMakeContextCurrent(context);
	
	sources = new ALuint[ MAX_SOURCES ];
	alGenSources( MAX_SOURCES, sources ); 
}

SoundController::~SoundController()
{
	soundBank::iterator it;

	for ( it = soundEffects.begin(); it != soundEffects.end(); it++ )
		soundEffects.erase( it );
	
	alSourceStopv(MAX_SOURCES, sources);	
    alDeleteSources(MAX_SOURCES, sources);
	
	alcMakeContextCurrent( NULL );
    alcDestroyContext( context );
    alcCloseDevice( device );
}

#pragma mark
#pragma mark Music
#pragma mark ----------

#warning Implement all music functions for sound engine (OGG Format)
//void SoundController::setMusicVolume( const ALfloat theVolume )
//{
//}
//void SoundController::loadMusic( const std::string &key, const std::string &asset, const std::string &extension )
//{
//}
//
//void SoundController::playMusic( const std::string &key, const int timesToRepeat )
//{
//}

#pragma mark
#pragma mark Sound Effects
#pragma mark ----------

void SoundController::loadSound( const std::string &asset, const ALuint frequency )
{	
	bool loaded = true;
	AudioFileID audioID;
	std::string path = FileUtil::getInstance()->getPathToFile( asset, "caf" );
	CFURLRef theURL = CFURLCreateFromFileSystemRepresentation(kCFAllocatorDefault, (UInt8*)path.c_str(), strlen(path.c_str()), false);
	if (theURL == NULL)
	{
		loaded = false;
	}
	
#if TARGET_OS_IPHONE
	OSStatus result = AudioFileOpenURL(theURL, kAudioFileReadPermission, 0, &audioID);
#else
	OSStatus result = AudioFileOpenURL(theURL, fsRdPerm, 0, &audioID);
#endif
	CFRelease( theURL );
	loaded = false;

	//	now we need to get the file size
	UInt32 fileSize = getAudioSize( audioID );
	
	// this is where the audio data will live for the moment
	unsigned char outData[ fileSize ];
	
	// this where we actually get the bytes from the file and put them
	// into the data buffer
	result = noErr;
	result = AudioFileReadBytes( audioID, false, 0, &fileSize, outData);
	AudioFileClose( audioID ); //close the file
	
	if (result != 0) 
		Log::write("cannot load effect");
	
	ALuint bufferID;
	// grab a buffer ID from openAL
	alGenBuffers(1, &bufferID);
	
	
	// jam the audio data into the new buffer
	alBufferData( bufferID, AL_FORMAT_STEREO16, outData, fileSize, 44100); 
	
	// save the buffer so I can release it later
	soundEffects[ asset ] = bufferID;
}

ALuint SoundController::playSound( const std::string &asset, const ALfloat gain, const ALfloat pitch, const Vector2 &location, const bool loop )
{
	ALenum err = alGetError(); // clear the error code
	
	// Find the buffer linked to the key which has been passed in
	soundBank::iterator it = soundEffects.find( asset );
	ALuint bufferID = it->second;
	
	// Find an available source i.e. it is currently not playing anything
	int index = getAvailableSource();
	
	//Attach the buffer we have looked up to the source we have just found
	alSourcei(sources[ index ], AL_BUFFER, 0);
	alSourcei(sources[ index ], AL_BUFFER, bufferID);
	
	// Set the pitch and gain of the source
	alSourcef(sources[ index ], AL_PITCH, pitch);
	alSourcef(sources[ index ], AL_GAIN, gain);
	
	// Set the looping value
	if( loop ) 
		alSourcei( sources[ index ], AL_LOOPING, AL_TRUE );
	else 
		alSourcei( sources[ index ], AL_LOOPING, AL_FALSE );

	
	// Check to see if there were any errors
	err = alGetError();
	if( err != AL_NO_ERROR ) 
	{
		Log::write("ERROR SoundManager");
		return 0;
	}
	
	// Now play the sound
	alSourcePlay( sources[ index ] );
	
	// Return the source ID so that loops can be stopped etc
	return sources[ index ];	
}

#pragma mark
#pragma mark Helper Functions
#pragma mark ----------

int SoundController::getAvailableSource()
{
	// Holder for the current state of the current source
	ALint sourceState;
	
	// Find a source which is not being used at the moment
	for( int i = 0; i < MAX_SOURCES; ++i )
	{
		alGetSourcei(sources[i], AL_SOURCE_STATE, &sourceState );
		if ( sourceState != AL_PLAYING )
			return i;
	}
	
	// If all the sources are being used we look for the first non looping source
	// and use the source associated with that
	ALint looping;
	for( int i = 0; i < MAX_SOURCES; ++i )
	{
		alGetSourcei(sources[i], AL_LOOPING, &looping );
		if( !looping )
		{
			ALuint sourceID = sources[ i ];
			alSourceStop( sourceID );
			return i;
		}
	}

	
	// If there are no looping sources to be found then just use the first sounrce and use that
	ALuint sourceID = sources[ 0 ];
	alSourceStop(sourceID);
	return  0 ;
	
}

UInt32 SoundController::getAudioSize( const AudioFileID &fileID )
{
	UInt64 dataSize = 0;
	UInt32 propSize = sizeof(UInt64);
	
	OSStatus result = AudioFileGetProperty( fileID, kAudioFilePropertyAudioDataByteCount, &propSize, &dataSize);
	
	if(result != 0) 
	{
		Log::write("Can not get the audio size");
	}
	
	return (UInt32)dataSize;
}

















