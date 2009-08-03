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
#include "MathUtil.h"

#include <stdexcept>
#include <string>
#include <map>

using std::runtime_error;

#define MAX_VERTICES 100000
#define Textures TextureController::getInstance()

class TextureControllerException : public runtime_error
{
public:
	TextureControllerException( std::string ex ) :
		runtime_error( ex ) { }
};


struct Vertex
{
	short v[2];
	unsigned color;
	float uv[2];
};

struct VertexInfo
{
	Vertex vertex[MAX_VERTICES];
	int _vertexCount;
};


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
	
	//	Begin and End are used to alert the controller that the client wishes to 
	//	render a bunch of textures to screen. Begin is called with various parameters,
	//	all textures wishing to be rendered to screen are passed into the controller
	//	via the 'draw' functions, and then end is called. Once end is called, the
	//	textures are rendered to screen.
	void begin( const bool blendAdditive = true );
	void end();
	
	//	Various draw functions that castcade into eachother. If you know that you only
	//	want to draw a texture onto a rectangle, calling the first draw function will
	//	call the most detailed draw function with default values. 
	void draw( const Texture2D& texture, const Rectangle& destination );
	void draw( const Texture2D& texture, const Rectangle& destination, const Color& color );
	void draw( const Texture2D& texture, const Rectangle& destination, const Rectangle& source, 
			  const Color& color );
	void draw( const Texture2D& texture, const Rectangle& destination, const Rectangle& source,
			  const Color& color, const GLfloat depth );
	
	//	This function is not yet implemented. 
//	void drawString( const string& text, const string& font, 
//									   const Vector2& location, const Color& color,
//									   const GLfloat scale );
	 
protected:
	TextureController();
	TextureController( const TextureController& copy ) { }
	~TextureController();
	
	static TextureController* instance;
	
	void clearRenderState();
	void renderToScreen();
	
	//	This function is actually doing the 'batching' for us. It will store all vertex /
	//	color / GLuint information needed to draw the sprites, and when end() is called
	//	it will be thrown to OpenGL for rendering
	void addVertex( GLuint glid, float x, float y, float uvx, float uvy, unsigned color );

private:
	typedef std::map<std::string, Texture2D*> textureMap;
	textureMap textures;
	typedef std::map<GLuint, VertexInfo> VertexMap;
	VertexMap vertices;
	
	GLuint currentTexture;
	
	bool blendAdditive;
	bool beginCalled;
};

#endif
