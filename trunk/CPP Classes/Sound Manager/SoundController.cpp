/*
 *  SoundController.cpp
 *  DjinnEngine
 *
 *  Created by Craig Giles on 6/3/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "SoundController.h"


//	Soundcontroller instance pointer
SoundController* SoundController::instance = 0;

SoundController* SoundController::getInstance()
{
	//	If no instance has been created, create one
	if ( instance == 0 )
	{
		instance = new SoundController;
	}
	
	//	otherwise, just return the instance
	return instance;
}

#pragma mark Constructor(s) / Destructor

SoundController::SoundController() : MAX_SOURCES( 32 )
{
	device = NULL;
	context = NULL;
	sources = NULL;
	
	musicVolume = 1.0f;	
	device = alcOpenDevice( NULL );
	
	if( !device )
	{
		Log::write("default device not found");
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
	std::map<string, ALuint>::iterator it;
	
	for ( it = musicLibrary.begin(); it != musicLibrary.end(); it++ )
		musicLibrary.erase( it );

	for ( it = soundLibrary.begin(); it != soundLibrary.end(); it++ )
		soundLibrary.erase( it );
	
	alSourceStopv(MAX_SOURCES, sources);	
    alDeleteSources(MAX_SOURCES, sources);
	
	alcMakeContextCurrent( NULL );
    alcDestroyContext( context );
    alcCloseDevice( device );
}

#pragma mark Music Functions

#warning Implement all music functions for sound engine (OGG Format)
void SoundController::setBackgroundMusicVolume( const ALfloat theVolume )
{
}
void SoundController::loadBackgroundMusicWithKey( const std::string &key, const std::string &asset, const std::string &extension )
{
}

void SoundController::playMusicWithKey( const std::string &key, const int timesToRepeat )
{
}

#pragma mark Sound Effects Functions.

void SoundController::loadSoundWithKey( const std::string &key, const std::string &asset, const std::string &extension, const ALuint frequency )
{	
	bool loaded = true;
	AudioFileID audioID;
	std::string path = FileUtil::getInstance()->getPathToFile( asset, extension );
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
	unsigned char outData[ fileSize ];// = sizeof( fileSize ); //malloc( fileSize );
	
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
	soundLibrary[ key ] = bufferID;
}

ALuint SoundController::playSoundWithKey( const std::string &key, const ALfloat gain, const ALfloat pitch, const Vector2 &location, const bool loop )
{
	ALenum err = alGetError(); // clear the error code
	
	// Find the buffer linked to the key which has been passed in
	std::map<string, ALuint>::iterator it = soundLibrary.find( key );
	ALuint bufferID = it->second;
	
	// Find an available source i.e. it is currently not playing anything
	int index = getAvailableSource();
	
	// Make sure that the source is clean by resetting the buffer assigned to the source
	// to 0
	alSourcei(sources[ index ], AL_BUFFER, 0);
	//Attach the buffer we have looked up to the source we have just found
	alSourcei(sources[ index ], AL_BUFFER, bufferID);
	
	// Set the pitch and gain of the source
	alSourcef(sources[ index ], AL_PITCH, pitch);
	alSourcef(sources[ index ], AL_GAIN, gain);
	
	// Set the looping value
	if( loop ) 
	{
		alSourcei( sources[ index ], AL_LOOPING, AL_TRUE );
	} else {
		alSourcei( sources[ index ], AL_LOOPING, AL_FALSE );
	}
	
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

#pragma mark Helper Functions

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

















