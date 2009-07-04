/*
 *  Color.h
 *  DjinnEngine
 *
 *  Created by Craig Giles on 6/15/09.
 *
 */

#ifndef COLOR_H
#define COLOR_H

struct Color
{
	Color() { red = 0; green = 0; blue = 0; alpha = 0; }
	Color( float a, float b, float c, float d ) { red = a; green = b; blue = c; alpha = d; }
	float red;
	float green;
	float blue;
	float alpha;
};

#endif