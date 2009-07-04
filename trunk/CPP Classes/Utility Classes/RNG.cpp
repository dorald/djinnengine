
#include "RNG.h"

bool RNG::seeded = false;

void RNG::seedRandomNumberGenerator( float seed ) 
{	
	//	if the seed is left blank, the default value of -1
	//	will be passed in and we should use time. If the client
	//	provides a seed for us to use, use that seed instead. 
	if ( seed == UNDEFINED_NUMBER )
		srand( time(0) );
	else
		srand( seed );
	
	seeded = true;
}

//
//	get a random number between 0 and max
//
int RNG::getRandomInt( int max )
{
	if (!seeded) seedRandomNumberGenerator();
	return getRandomInt( 0, max );
}

//
//	get a random number between min and max
//
int RNG::getRandomInt( int min , int max )
{
	if (!seeded) seedRandomNumberGenerator();
	return rand() % (max - min) + min;
}

//
//	get a random float between 0 and max
//
float RNG::getRandomFloat( float max )
{
	if (!seeded) seedRandomNumberGenerator();
	return randfloat() * max;
}

//
//	get a random float between min and max
//
float RNG::getRandomFloat( float min , float max )
{
	if (!seeded) seedRandomNumberGenerator();
	if ( min > max ) 
    { 
        return randfloat() * (min-max) + max;     
    } 
    else 
    { 
        return randfloat() * (max-min) + min; 
    }  
}

//
//	get a random double between 0 and max
//
double RNG::getRandomDouble( double max )
{
	if (!seeded) seedRandomNumberGenerator();
	return randdouble() * max;
}

//
//	get a random double between min and max
//
double RNG::getRandomDouble( double min , double max )
{
	if (!seeded) seedRandomNumberGenerator();
	if ( min > max ) 
    { 
        return randdouble() * (min-max) + max;     
    } 
    else 
    { 
        return randdouble() * (max-min) + min; 
    } 
}

//
//	get a random vector2 where x and y are between min and max
//
Vector2 RNG::getRandomVector2( float min, float max )
{
	return Vector2(
		getRandomFloat( min, max ),
		getRandomFloat( min, max ) );
}

//
//	private member functions for getting random floats and doubles
//
float RNG::randfloat()
{
	return rand() / ( float(RAND_MAX) + 1 ); 
}

double RNG::randdouble()
{
	return rand() / ( double(RAND_MAX) + 1 );
}
