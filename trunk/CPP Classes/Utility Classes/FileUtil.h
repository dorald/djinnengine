/*
 *  FileUtil.h
 *  DjinnEngine
 *
 *  Created by Craig Giles on 5/25/09.
 *
 */

//	File Utility:
//	When creating this class, I needed a way to obtain information from files
//	within the application bundle... and for the life of me I could not get it
//	working with C++ calls... To that end, I was forced to use an Obj-C++ class.
//	
//	For the most part, the file utility class (called via the Files->functionName())
//	is mostly used to serialize and unserialize objects within the game. You'll notice
//	in various places, the Files->unSerialize( assetName, object ); is called.. the
//	object is unSerialized, and the game can continue.

#ifndef FILEUTIL_H
#define FILEUTIL_H

#define Files FileUtil::getInstance() 

#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <QuartzCore/QuartzCore.h>	//used to load texture into memory

#include <sstream>
#include <fstream>

using std::string;

enum FileType {
	AnimationType = 0,
};

class FileUtil
{
public:
	FileUtil() { }
	~FileUtil() { }
	
	static string FileExtension;
	static FileUtil *getInstance();
	
	string getDocumentsDirectory() { return documentsDir; }	
	string getAppDirectory() { return applicationDir; }	
	
	void setDocumentsDirectory( const string value );
	void setAppDirectory( const string value );
	
	//	Uses Obj-C classes to open a file using fopen and return the
	//	FILE*. If the file was opened successfully the FILE* will return
	//	a place in memory. If the file was not opened successfully, 
	//	it will be pointed to NULL. 
	//
	//	NOTE: I am considering making this a utility function, since
	//	now that I've written the 'serialize' and 'unSerialize' funcitons,
	//	I am not sure the client needs access to the file pointer.
	FILE* openFile( const string &asset, const string &extension );
	
	//	Uses Obj-C classes to locate the path to a file within the bundle, convert
	//	that path to a string object, and send that string back to the caller.
	string getPathToFile( const string &assetName, const string &extension );
	
	
	template< typename T >
	void serialize( const string &asset, T& value )
	{
		std::ofstream outFile;
		std::stringstream oStream;
				
		string path = getDocumentsPath( asset );		
		outFile.open( path.c_str() , std::ios::binary );		
		oStream << value;		
		outFile.write( oStream.str().c_str() , oStream.str().length() );
		
		outFile.close();
	}
	
	template< typename T >
	void unSerialize( const string &asset, T &value )
	{
		std::stringstream stream;	//stream to store the data file
		char buffer[1024];			//Buffer used to read data file line by line
		
		//	Open the file and return the file pointer
		FILE* fp = openFile( asset, FileUtil::FileExtension );
		
		//	if the file pointer is null, the file was not opened.
		if ( fp == NULL )
			return;
		
		//	Read teh file line by line, filling the buffer, then store that buffer
		//	into a stringstream object. 
		while( fgets( buffer, 1024, fp ) )
			stream << buffer;
		
		//	Create a new animation object and extract the stream's data into
		//	the animation.
		stream >> value;
		
		fclose( fp );
	}
	
private:
	string getDocumentsPath( const string &asset );
	
	string documentsDir;
	string applicationDir;
	static FileUtil* instance;
};

#endif

