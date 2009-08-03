/*
 *  MathUtil.h
 *  Djinn Engine
 *
 *  Created by Craig Giles on 4/24/09.
 *
 */

//	NOTE: Heavily influenced by the AngelEngine:
//	http://code.google.com/p/angel-engine/

#ifndef MATHUTIL_H
#define MATHUTIL_H

#include "Vector2.h"

class MathUtil
{
public:

	//	Euler's number: http://en.wikipedia.org/wiki/E_(mathematical_constant)
	//	"the unique real number such that the area above the x-axis and below 
	//	the curve y=1/x for 1 ≤ x ≤ e is exactly 1"
	static const float E;

	//	Base-10 logarithm of Euler's number
	static const float Log10E;

	//	Base-2 logarithm of Euler's number
	static const float Log2E;

	// Pi: http://en.wikipedia.org/wiki/Pi
	static const float Pi;

	//	Pi divided by 2
	static const float PiOver2;

	//	Pi divided by 4
	static const float PiOver4;

	//	2 times Pi
	static const float TwoPi;

	//	The maximum value that can be represented by a 32-bit floating point
	//	number
	static const float MaxFloat;
	
	//	The minimum value that can be represented by a 32-bit floating point
	//	number
	static const float MinFloat;
	
	//	A very tiny number, useful for floating point comparisons (10e-6)
	static const float Epsilon;
	
	//	A template absolute value function that can handle any class where
	//	a comparison between the class and 0 makes sense. Mostly used
	//	for floats and ints
	//	Takes in a value
	//	returns the absolute value of said value
	template< typename T >
	static T Abs( const T &val )
	{
		return val >= 0 ? val : -val;
	}
	
	//	A template function that returns the greater of two values. Works with
	//	any class that impliments an overloaded > operator
	//	- Takes in two values
	//	- returns the greater of those two values
	template< typename T >
	static T Max( const T &value1, const T &value2)
	{
		return value1 > value2 ? value1 : value2;
	}
	
	//	A template function that returns the lesser of two values. Works with
	//	any class that impliments an overloaded < operator
	//	- takes in two values
	//	- returns the lesser of those two values
	template< typename T >
	static T Min( const T &value1, const T &value2)
	{
		return value1 < value2 ? value1 : value2;
	}
	
	//	Find the distance between two values. Works for any class that
	//	has a subtraction operator and will work with the Abs() function
	//	- takes in two values
	//	- returns the distance between them
	template< typename T >
	static T Distance( const T &value1, const T &value2)
	{
		return Abs(value1 - value2);
	}
	
	//	Linearly interpolates between two values. If two known points are given
	//	by the coordinates (X0, Y0) and (X1, Y1), the Lerp is the straight
	//	line between these two points.
	//	
	//	Works for any class that define addition, subtraction, and multiplication
	//	(by a float) operators.
	//
	//	http://en.wikipedia.org/wiki/Lerp_(computing)
	//	
	//	Value1 = Starting value
	//	Value2 = Ending value
	//	Amount = the amount to lerp (from 0.0 to 1.0)
	//	returns the interpolated value
	template<typename T>
	static T Lerp( const T &value1, const T &value2, const float amount)
	{
		return T(value1 + ((T)(value2 - value1) * amount ));
	}
	
	//	Clamps an integer to a specified range
	//
	//	Value = The integer in question
	//	min = the minimum of the range
	//	max = the maximum of the range
	//	returns the clamped value
	static int Clamp( const int value, const int min, const int max)
	{
		return Max( min, Min(max, value) );
	}
	
	//	Clamps a float to a specified range
	//
	//	Value = The float in question
	//	min = the minimum of the range
	//	max = the maximum of the range
	//	returns the clamped value
	static float Clamp(const float value, const float min, const float max)
	{
		return Max( min, Min(max, value) );
	}
	
	//	Clamps a double to a specified range
	//
	//	Value = The double in question
	//	min = the minimum of the range
	//	max = the maximum of the range
	//	returns the clamped value
	static double Clamp( const double value, const double min, const double max)
	{
		return Max( min, Min(max, value) );
	}
	
	//	Converts degrees to radians
	//
	//	degrees = floating point angle in degrees
	//	returns floating point angle in radians
	static float DegreesToRadians( const float degrees );
	
	//	Converts radians to degrees
	//
	//	radians = floating point angle in radians
	//	returns floating point angle in degrees
	static float RadiansToDegrees( const float radians );
	
	//	Finds the next Power of Two
	static int nextPowerOfTwo(int v)
	{
		v--;
		v |= v >> 1;
		v |= v >> 2;
		v |= v >> 4;
		v |= v >> 8;
		v |= v >> 16;
		v++;
		return v;
	};
};

#endif














