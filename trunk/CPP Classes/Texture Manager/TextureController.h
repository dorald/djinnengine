/*
 *  TextureController.h
 *  DjinnEngine
 *
 *  Created by Craig Giles on 7/5/09.
 *
 */

#ifndef TEXTURECONTROLLER_H
#define TEXTURECONTROLLER_H

#include "Texture2D.h"

#include <string>
#include <map>

#define Textures TextureController::getInstance()

class TextureController
{
public:
	//	obtain the instance of the TextureController
	static TextureController* getInstance();
	
	//	Attempts to obtain a texture by asset name. If the texture exists,
	//	a reference to the texture will be passed back. If the texture does
	//	not exist, it will be loaded and then passed back.
	Texture2D* getTexture( const std::string &asset );
	
	//	Flush all textures from memory. This should only be called on situations
	//	such as a level change or on shutdown.
	void flushTextures();
	
	//	remove a texture from the texture map, and deallocate any memory
	//	associated with the texture.
	bool releaseTexture( const std::string &asset );
	
	//	Get the ID of the currently bound texture
	GLuint getBoundTexture() const { return currentTexture; }
	
	//	Bind the given texture to OpenGL
	void bindTexture( const GLuint name );
	
protected:
	TextureController();
	TextureController( const TextureController& copy ) { }
	~TextureController();
	
	static TextureController* instance;
	
private:
	typedef std::map<std::string, Texture2D*> textureMap;
	textureMap textures;
	GLuint currentTexture;
};

#endif
