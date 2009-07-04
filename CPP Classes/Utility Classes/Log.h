/*
 *  Log.h
 *  DjinnEngine
 *
 *  Created by Craig Giles on 6/1/09.
 *
 */

//	Log:
//	This really needs to be written better, because right now all it does is write
//	something to the application bundle documents directory.. which does pretty
//	much nothing if we're debugging. I had the idea of emailing the log to the
//	developer on application crash, and dump the stack trace into the log... but
//	never really figured out how to doi t. This class will be revamped in the future.

#ifndef LOG_H
#define LOG_H

#include "FileUtil.h"

#include <string>
#include <fstream>

using std::ofstream;
using std::string;

class Log
{
public:
	static void write( const string value );
};

#endif