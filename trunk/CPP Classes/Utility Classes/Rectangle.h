//
//	Rectangle utility
//	Djinn Engine
//

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Vector2.h"

class Rectangle
{
public:
	//
	//	Constructor(s) / Destructor
	//
	Rectangle(int x = 0, int y = 0, int width = 0, int height = 0);
	~Rectangle() { };

	//	Returns a copy of a rectangle with its members initialized to 1 
	//	or 0 depending which function is called. 
	static Rectangle ONE() { return Rectangle(1, 1, 1, 1); }
	static Rectangle ZERO() { return Rectangle(0, 0, 0, 0); }
	
	//	Data members left public, since the rectangle is more like
	//	a struct with helper functions, and these data members are
	//	accessed often enough that it didn't make sense to encapsulate
	//	them. Error checking with the width and height is left up
	//	to the client.
	int x;
	int y;
	int width;
	int height;
	
	//
	//	operator overloads
	//
	bool operator ==( const Rectangle &value ) const;
	bool operator !=( const Rectangle &value ) const;
	
	//	Checks the rectangle and determines if the specified point
	//	is located within the rectangle's body.
	bool containsPoint( const Vector2 *value );
};

#endif