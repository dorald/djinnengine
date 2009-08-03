/*
 *  TextureController.cpp
 *  DjinnEngine
 *
 *  Created by Craig Giles on 7/5/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "TextureController.h"

#pragma mark
#pragma mark Constructor(s) / Destructor
#pragma mark ---------

TextureController* TextureController::instance = 0;

TextureController* TextureController::getInstance()
{
	if (instance == 0)
		instance = new TextureController();
	
	return instance;
}

TextureController::TextureController() 
{
	beginCalled = false;
}

TextureController::~TextureController()
{
	//clean up particle vert (now a map)delete [] particleVert;
	vertices.clear();
}

#pragma mark
#pragma mark Texture Management
#pragma mark ---------

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
	
	//	if there are any vertices in the VertexMap currently using
	//	the deleted texture, we should remove them from the vertex map.
	//	NOTE:: this has not been tested
#warning test this
	VertexMap::iterator vertIterator = vertices.begin();
	for (/* */; vertIterator != vertices.end(); it++)
	{
		if(vertIterator->first == it->second->getId())
			break;
	}
	
	if (vertIterator != vertices.end())
		vertices.erase( vertIterator );
		
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
	
	vertices.clear();
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

#pragma mark
#pragma mark Begin / End functions (Un-Implemented)
#pragma mark ---------

void TextureController::begin( const bool blendAdditive )
{
	this->blendAdditive = blendAdditive;
	beginCalled = true;
}

void TextureController::end()
{
	if (!beginCalled)
		throw TextureControllerException("Begin must be called before end");

	renderToScreen();
	clearRenderState();
}

void TextureController::clearRenderState()
{
	vertices.clear();
	beginCalled = false;
}

void TextureController::renderToScreen()
{
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	
	//	Texture Blending fuctions
	if ( blendAdditive )
	{
		glEnable(GL_BLEND);	
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	
	//	needed to draw textures using Texture2D
	glEnable(GL_TEXTURE_2D);		
	
	//	enables alpha for transparent textures
	//	I forget where I got these commands, iDevGames.net I think
	glAlphaFunc(GL_GREATER, 0.1f);
	glEnable(GL_ALPHA_TEST);
	
	//	Enable the various arrays used to draw texture to screen
	glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
    glLoadIdentity();
	
	//	loop through all of the elements of the map and draw the vertices
	VertexMap::iterator it = vertices.begin();
	for (/* none */; it != vertices.end(); it++)
	{
		//	easy access to our data
		VertexInfo *vertexInfo = &it->second;
		Vertex *vert = vertexInfo->vertex;
		
		//	bind the texture for the following vertices
		bindTexture( (*it).first );
		
		//	throw everything to OpenGL
		glVertexPointer(2, GL_SHORT, sizeof(Vertex), &vert->v);
		glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &vert->uv);
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), &vert->color);
		glDrawArrays(GL_TRIANGLES, 0, vertexInfo->_vertexCount);
		
		//	reset this batches vertex count
		vertexInfo->_vertexCount = 0;
	}
	
	//	disable all the stuff we enabled eariler
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisable( GL_BLEND );
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_ALPHA_TEST );
	
	glPopMatrix();
}

#pragma mark
#pragma mark Draw functions 
#pragma mark ---------

void TextureController::draw( const Texture2D& texture, const Rectangle& destination )
{
	draw( texture, destination, Rectangle::ZERO(), Color(1, 0, 0, 1), 1.0f);
}

void TextureController::draw( const Texture2D& texture, const Rectangle& destination, 
							 const Color& color )
{
	draw( texture, destination, Rectangle::ZERO(), color, 1.0f);
}
void TextureController::draw( const Texture2D& texture, const Rectangle& destination, 
							 const Rectangle& source, const Color& color )
{
	draw( texture, destination, source, color, 1.0f);
}
void TextureController::draw( const Texture2D& texture, const Rectangle& destination, 
							 const Rectangle& source, const Color& color, const GLfloat depth )
{	
	//
	//	TODO:: need to implement depth somehow
	//
	
	GLuint glid = texture.getId();
		
	//	if we don't have any vertices with the texture being drawn, create a
	//	vertex map for it.
	VertexMap::iterator it = vertices.find(glid);
	if (it == vertices.end())
		vertices[glid]._vertexCount = 0;		
	
	//	find all of the vertices we'll need for this sprite
	float topLeftX = destination.x;
	float topLeftY = destination.y;	
	float topRightX = destination.x + destination.width;
	float topRightY = destination.y;	
	float bottomLeftX = destination.x;
	float bottomLeftY = destination.y + destination.height;	
	float bottomRightX = destination.x + destination.width;
	float bottomRightY = destination.y + destination.height;

	// Texture atlas
	float minUV[2];
	float maxUV[2];
	
	//	if the source rectangle of ZERO was passed in, it means the client want to just
	//	draw the texture as is.. otherwise, the client wishes to draw a portion of
	//	the rectangle
	if (source == Rectangle::ZERO())
	{
		float maxS = texture.getMaxS(); 
		float maxT = texture.getMaxT();
		float minS = 0;
		float minT = 0;
	
		minUV[0] = minS;
		minUV[1] = minT;
		maxUV[0] = maxS;
		maxUV[1] = maxT;
	}
	else
	{
		float minS = source.x / texture.getWidth();
		float minT = source.y / texture.getHeight();
		float maxS = source.width / texture.getWidth(); 
		float maxT = source.height / texture.getHeight();

		minUV[0] = minS;
		minUV[1] = minT;
		maxUV[0] = maxS;
		maxUV[1] = maxT;
	}
	
	//	Convert the colors into bytes
	unsigned char red = color.red * 255.0f;
	unsigned char green = color.green * 255.0f;
	unsigned char blue = color.blue * 255.0f;
	unsigned char shortAlpha = color.alpha * 255.0f;
	
	//	pack all of the color data bytes into an unsigned int
	unsigned _color = (shortAlpha << 24) | (blue << 16) | (green << 8) | (red << 0);
	
	// Triangle #1
	addVertex(glid, topLeftX, topLeftY, minUV[0], minUV[1], _color);
	addVertex(glid, topRightX, topRightY, maxUV[0], minUV[1], _color);
	addVertex(glid, bottomLeftX, bottomLeftY, minUV[0], maxUV[1], _color);
	
	// Triangle #2
	addVertex(glid, topRightX, topRightY, maxUV[0], minUV[1], _color);
	addVertex(glid, bottomLeftX, bottomLeftY, minUV[0], maxUV[1], _color);
	addVertex(glid, bottomRightX, bottomRightY, maxUV[0], maxUV[1], _color);
}

void TextureController::addVertex(GLuint glid, float x, float y, float uvx, float uvy, unsigned color)
{
	VertexInfo *vertexInfo = &vertices[glid];
	Vertex *vert = &vertexInfo->vertex[vertexInfo->_vertexCount];
    vert->v[0] = x;
    vert->v[1] = y;
    vert->uv[0] = uvx;
    vert->uv[1] = uvy;
    vert->color = color;
	vertexInfo->_vertexCount++;
}

#pragma mark
#pragma mark DrawString functions (Un-Implemented)
#pragma mark ---------
//
//void TextureController::drawString( const std::string& text, const std::string& font, 
//								   const Vector2& location, const Color& color,
//								   const GLfloat scale )
//{
//	//	bind the font bitmap
//	//	loop through the string for however long the string is
//	//		render the character to screen. Since the texture will be the same for
//	//		the entire screen, we only need to bind it once and jump around to
//	//		the position from the bitmap of the font.
//	//
//	//		My guess is, the best way to do this is to have a map with the
//	//		unicode of the character to be drawn, and the vector2 offset where
//	//		it can be found on the bitmap.
//}
//









