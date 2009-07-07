/*
 *  MathUtil.cpp
 *  Djinn Engine
 *
 *  Created by Craig Giles on 4/24/09.
 *
 */

#include "MathUtil.h"

const float MathUtil::E = 2.718282f;
const float MathUtil::Log10E = 0.4342945f;
const float MathUtil::Log2E = 1.442695f;
const float MathUtil::Pi = 3.141593f;
const float MathUtil::PiOver2 = 1.570796f;
const float MathUtil::PiOver4 = 0.7853982f;
const float MathUtil::TwoPi = 6.283185f;
const float MathUtil::MaxFloat = 3.402823E+38f;
const float MathUtil::MinFloat = -3.402823E+38f;
const float MathUtil::Epsilon = 0.000001f;

float MathUtil::RadiansToDegrees(float radians)
{
	//	the formula for converting radians to degrees is
	//	D / 180 = R / Pi
	//	Solve for D = radians * (180 / Pi) 
	//	180 / Pi = 57.29578
	//	final equation is: Degrees = radians * 57.29578
	return (radians * 57.29578f);
}

float MathUtil::DegreesToRadians(float degrees)
{
	//	the formula for converting degrees to radians is
	//	D / 180 = R / Pi
	//	Solve for R = degrees * (Pi / 180) 
	//	Pi / 180 = 0.01745329
	//	final equation is: Radians = degrees * 0.01745329
	return (degrees * 0.01745329f);
}
