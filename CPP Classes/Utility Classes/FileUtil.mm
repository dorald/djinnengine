/*
 *  FileUtil.cpp
 *  DjinnEngine
 *
 *  Created by Craig Giles on 5/25/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "FileUtil.h"

FileUtil* FileUtil::instance = 0;
string FileUtil::FileExtension = "dat";

FileUtil *FileUtil::getInstance()
{
	if ( !instance )
		instance = new FileUtil();
	
	return instance;
}

#pragma mark Properties.

void FileUtil::setDocumentsDirectory( const string value )
{
	if ( documentsDir.length() == 0 ); 
	documentsDir = value; 
}

void FileUtil::setAppDirectory( const string value )
{
	if ( applicationDir.length() == 0 ); 
		applicationDir = value; 
}

#pragma mark Functions.

FILE* FileUtil::openFile( const string &assetName, const string &extension )
{
	string path = getPathToFile( assetName, extension );
	FILE* file = fopen( path.c_str(), "rb"); 
	
	return file;
}

string FileUtil::getPathToFile( const string &assetName, const string &extension )
{
	char path[1024];
	
	//	allocate a new NSString with the asset name and extension names.
	NSString *asset = [[NSString alloc] initWithCString:assetName.c_str()];
	NSString *ext = [[NSString alloc] initWithCString:extension.c_str()];
	
	//	Get the path for the file and store it in a CFStringRef
	CFStringRef pathRef = (CFStringRef)[[NSBundle mainBundle] pathForResource:asset ofType:ext];
	
	//	Convert the path to a char* and store it in path array
	CFStringGetCString( pathRef, path, 1024, kCFStringEncodingUTF8);
	
	//	Release memeory allocated 
	[asset release];
	[ext release];
	
	return string(path);
}

string FileUtil::getDocumentsPath( const string &asset )
{
	char iphoneDocDirectory[1024];
	
	// get the documents directory, where we will write configs and save games
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];
	[documentsDirectory getCString: iphoneDocDirectory 
						 maxLength: sizeof( iphoneDocDirectory ) - 1
						  encoding: NSASCIIStringEncoding ];
	
	std::string path( iphoneDocDirectory );
	path += '/' + asset + '.' + FileUtil::FileExtension;
	
	return path;
}