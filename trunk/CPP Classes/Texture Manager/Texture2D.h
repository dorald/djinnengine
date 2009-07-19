/*
 *  Texture2D.h
 *  DjinnEngine
 *
 *  Created by Craig Giles on 7/5/09.
 *
 */
//	the following will allow you to tile a texture.. put in a draw function?
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

#ifndef TEXTURE2D_H
#define TEXTURE2D_H


#include "OpenGlCommon.h"
#include "Rectangle.h"
#include "Color.h"

#include <QuartzCore/QuartzCore.h>	//used to load texture into memory
#include <string>

const int kMaxTextureSize = 1024;

typedef enum
{		
		PNG = 0,
		JPEG = 1,
} ImageType;

class TextureController;
class Texture2D
{
	//	Make the texture controller a friend of this texture class.
	friend class TextureController;
	
public:
	//	Get the GL id code used to identify 
	GLuint getId() const { return _glId; }
	
	//	get the width and height of the texture in pixels
	GLuint getWidth() const { return _width; }
	GLuint getHeight() const { return _height; }
	
	//	Max S and Max T are the basic representations of X and Y in our
	//	Texture Coordinate System. T is bottom to top while S is Left to 
	//	Right. If a texture is 512 x 512, but the actual renderable portion
	//	is only 500 x 510, the maxT would be 500, while the max S would be 510
	//	represented in dec form between 0 and 1. 
	GLfloat getMaxS() const { return _maxS; }
	GLfloat getMaxT() const { return _maxT; }

	//	reference counters for the texture. When an object needs to use a texture
	//	that has already been created, it will add a reference. When they no longer
	//	need it, it should release the reference. Once the reference counter has
	//	reached 0, the texture will deallocate any memory associated with it, and
	//	remove itself from the texture map.
	void addReference();
	void releaseReference();
	
	//	Tells the texture controller to bind the texture using glBindTexture() function
	void bindTexture();
	
	//	Render the texture to screen using a variety of parameters.
	void draw( const Rectangle& rectangle );
	void draw( const Rectangle& rectangle, const Color& color );
	void draw( const Rectangle& rectangle, const Color& color, const bool blendAdditive );
	
protected:
	//	Only the TextureController class should be able to instanciate
	//	a texture object
	Texture2D( const std::string &asset );
	~Texture2D();
	
private:
	//	texture Data
	GLuint _glId;			//OpenGL's ID for this texture
	GLuint _width;			
	GLuint _height;
	
	GLfloat _maxT;			//	verticle
	GLfloat _maxS;			//	horizontal
	
	int _referenceCount;
	std::string assetName;	//Name of file this texture was loaded from
	
	void loadFile( const std::string& asset );
	
};

#endif
