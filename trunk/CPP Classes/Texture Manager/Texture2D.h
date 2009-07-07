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

#include <QuartzCore/QuartzCore.h>	//used to load texture into memory
#include <string>

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
	GLuint getId() const { return _glId; }
	GLuint getWidth() const { return _width; }
	GLuint getHeight() const { return _height; }
	
	void addReference();
	void releaseReference();
	void bindTexture();
	void draw( const Rectangle& rectangle );
	
protected:
	//	Only the TextureController class should be able to instanciate
	//	a texture
	Texture2D( const std::string &asset );
	~Texture2D();
	
private:
	//	texture Data
	GLuint _glId;			//OpenGL's ID for this texture
	GLuint _width;			
	GLuint _height;
	int _referenceCount;
	std::string assetName;	//Name of file this texture was loaded from
	
	void loadFile( const std::string& asset );
	
};

#endif
