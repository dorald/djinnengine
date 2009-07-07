/*
 *  TextureController.cpp
 *  DjinnEngine
 *
 *  Created by Craig Giles on 7/5/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "TextureController.h"

#pragma mark Constructor(s) / Destructor

TextureController* TextureController::instance = 0;

TextureController* TextureController::getInstance()
{
	if (instance == 0)
		instance = new TextureController();
	
	return instance;
}

TextureController::TextureController()
{
}

TextureController::~TextureController()
{
}

#pragma mark Texture Management

Texture2D* TextureController::getTexture( const std::string &asset )
{	
	//	Look inside the texture map for the texture.. if it already
	//	exists, return it. If it can't find it, we'll have to create it.
	textureMap::const_iterator it = textures.find( asset );
	if (it != textures.end()) 
		return it->second;
	
	//	create the texture, add it to the texture map, and return 
	//	its reference.
	Texture2D* newTexture = new Texture2D(asset);
	textures[asset] = newTexture;
	return newTexture;
}

bool TextureController::releaseTexture( const std::string &asset )
{
	bool found = false;
	textureMap::iterator it = textures.find(asset);
	
	//	if the texture was found, and is not NULL, delete it and
	//	erase it from the texture map.
	if (it != textures.end())
	{
		found = true;
		if (it->second)
			delete it->second;
		textures.erase(it);
	}
	
	return found;
}

void TextureController::flushTextures()
{
	textureMap::iterator it = textures.begin();
	for ( /*no init*/; it != textures.end(); it++)
	{
		if (it->second)
			delete it->second;
		textures.erase(it);
	}
}

//	Bind the given texture to OpenGL
void TextureController::bindTexture( const GLuint name )
{
	if (name != getBoundTexture() )
	{
		currentTexture = name;
		glBindTexture(GL_TEXTURE_2D, name);
	}
}



