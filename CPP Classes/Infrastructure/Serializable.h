/*
 *  Serializable.h
 *  DjinnEngine
 *
 *  Created by Craig Giles on 3/29/09.
 *
 */

//	Serializable:
//	ANY class that can be serialized needs to have an asset. 
//	An asset is basically the name of the file in the application bundle,
//	minus the extension. Right now for most of the files, i have been using
//	.dat files.. so for example, if loading the animation "RUNNINGLEFT.dat"
//	then the asset name would be RUNNINGLEFT.
//
//	NOTE: ALL CLASSES that are serializable should have the following two functions:
//	friend std::istream& operator >>( std::istream &inStream, 'object' &value );
//	friend std::ostream& operator <<( std::ostream &outStream, 'object' &value );

#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include "FileUtil.h"

#include <string>

using std::string;

class Serializable
{
public:	
	void setAsset( const string &value ) { asset = value; }
	string getAsset() const { return asset; }
	
protected:
	string asset;
};

#endif