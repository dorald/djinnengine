//
//	Random Number Generator
//	Djinn Engine
//

#ifndef RNG_H
#define RNG_H

#include "Vector2.h"
#include <cstdlib>
#include <ctime>

const int UNDEFINED_NUMBER = -1;

class RNG
{
public:
	//	Seeds the random number generator with a value X (provided by
	//	the client) or if no value is provided, time(0) is used.
	static void seedRandomNumberGenerator( float seed = -1 );
	
	//	The following two functions will generate a random int either
	//	between 0 and a number specified by the user, or between two
	//	numbers specified by the user.
	static int getRandomInt( int max = RAND_MAX );
	static int getRandomInt( int min, int max );
	
	//	The following two functions will generate a random float either
	//	between 0 and a number specified by the user, or between two
	//	numbers specified by the user.
	static float getRandomFloat( float max = RAND_MAX );
	static float getRandomFloat( float min, float max );
	
	//	The following two functions will generate a random double either
	//	between 0 and a number specified by the user, or between two
	//	numbers specified by the user.
	static double getRandomDouble( double max = RAND_MAX );
	static double getRandomDouble( double min, double max );
	
	//	The following function will generate a random point where
	//	the x and y coords are between the two numbers provided by 
	//	the user.
	static Vector2 getRandomVector2( float min, float max );
	
private:
	//	Set the constructor to private, we do not want any instances of
	//	the random number generator to be created.
	RNG();
	
	//
	//	Private helper methods
	//
	static float randfloat();
	static double randdouble();
	
	//	
	//	Has the RNG been seeded yet?
	//
	static bool seeded;
};

#endif