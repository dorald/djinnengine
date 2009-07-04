//
//	Texture utility
//	Djinn Engine
//

//	Texture2D
//	This is a modified version of the Texture2D class provided by Apple in the
//	Crash Landing application. Eventually, I would like to go through and 
//	re-write the class to be more efficient and eliminate any memory leaks that
//	maybe present, but as of this time it works and does what I need it to do.
//	
//	The Texture2D provided by apple worked with Objective-C, and I wanted to use
//	a C++ wrapper for it... but then I realized that most of the code was simply
//	C Style code and just changed a few things around to make it a C++ class.


#ifndef TEXTURE_H
#define TEXTURE_H

//
//	Include statements
//
#include <QuartzCore/QuartzCore.h>	//used to load texture into memory
#include "OpenGlCommon.h"
#include <string>
using std::string;
#include "Rectangle.h"

//
//	Max texture size allowed for a texture.
//
const int kMaxTextureSize = 1024;

//
//	Pixel format used by the texture
//
enum Texture2DPixelFormat
{
	kTexture2DPixelFormat_Automatic = 0,
	kTexture2DPixelFormat_RGBA8888,
	kTexture2DPixelFormat_RGB565,
	kTexture2DPixelFormat_A8,
};

enum ImageType
{
	PNG = 0,
	JPG,
};

enum TextAlignmentType
{
	AlignmentLeft = 0,
	AlignmentRight,
	AlignmentCenter,
};

class Texture2D
{
public:	
	//	Constructor(s) / Destructor
	//	Default constructor. A default texture2d object should never need to be created
	//	except in the situation where you have an array
	//	Example: Texture2D *frames = new Texture2D[4];
	Texture2D() {}
	
	//	initialize with the file name and image type.
	//	Texture2D( "fileName", PNG );
	Texture2D(const string &filename, ImageType imageType);
	
	//	Initialize with the display text, dimensions, alignment type, font name, and font size
	//	Texture2D( "Display This Text", Size(32, 32), AlignmentCenter, "Arial", 18 );
	Texture2D( const string &text, const Size &dimensions, const TextAlignmentType &alignment, const string &fontName, const float &fontSize );
	
	//	Copy constructor, used to copy one texture2D object to another
	//	Texture2D newFrame = oldFrame;
	Texture2D( const Texture2D &copy );
	
	//	Destructor, ensures all memory is being released properly and handles
	//	the teardown of this Texture2D object
	~Texture2D();
	
	//	Overloaded operator used to copy one Texture object into another.
	Texture2D &operator =( const Texture2D &copy );
	
	//	Functions used to gain access to the private data of the Texture2D class.
	//	Everything returned by value, and no functions are available to set
	//	the data, since the data should only be set on creation. If the values
	//	need to be changed, a new object should be constructed and the old destroyed
	//	since it means something has gone very very wrong with the old object.
	GLuint getName() const { return _name; }
	CGSize getSize() const { return _size; }
	int getWidth() const { return _width; }
	int getHeight() const { return _height; }
	Texture2DPixelFormat getFormat() const { return _format; }
	GLfloat getMaxS() const { return _maxS; }
	GLfloat getMaxT() const { return _maxT; }
	
	//	Tells the texture to draw itself using the specified rectangle
	//	as a canvas / spritebox.
	void drawInRect( const Rectangle &drawRect );
	
private:
	//
	//	Private Methods
	//
	void initWithData( const void* data, Texture2DPixelFormat pixelFormat, int width, int height, CGSize size );
	void initWithImage( const CGImageRef &incImage );
	CGImageRef createImageObject( const CGDataProviderRef &provider, ImageType imageType );
	
	//
	//	Member Data
	//
	GLuint						_name;
	CGSize						_size;
	int							_width,
								_height;
	Texture2DPixelFormat		_format;
	GLfloat						_maxS,
								_maxT;
	

};

#endif