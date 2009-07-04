/*
 *  Log.cpp
 *  DjinnEngine
 *
 *  Created by Craig Giles on 6/1/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Log.h"

void Log::write( const string value )
{
	string dir = FileUtil::getInstance()->getDocumentsDirectory();
	ofstream oStream;
	
	oStream.open( dir.c_str() );
	oStream << value;
	oStream.close();
}