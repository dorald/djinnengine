/*
 *  Texture2D.cpp
 *  DjinnEngine
 *
 *  Created by Craig Giles on 7/5/09.
 *
 */

#include "Texture2D.h"
#include "TextureController.h"

#pragma mark Constructor(s) / Destructor

Texture2D::Texture2D( const std::string &asset )
{
	loadFile(asset);
}
Texture2D::~Texture2D()
{
}

#pragma mark Reference Counting

void Texture2D::addReference()
{
}
void Texture2D::releaseReference()
{
}

void Texture2D::bindTexture() 
{ 
	Textures->bindTexture( _glId ); 
}

#pragma mark Load Texture from file.

void Texture2D::loadFile( const std::string& asset )
{
	CGImageRef image;
	CGDataProviderRef provider;
	CFStringRef name;
	CFURLRef url;
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	
	// Get the URL to the bundle resource.
	name = CFStringCreateWithCString (NULL, asset.c_str(), kCFStringEncodingUTF8);
	url = CFBundleCopyResourceURL(mainBundle, name, CFSTR("png"), NULL);
	CFRelease(name);
	
	// Create the data provider object
	provider = CGDataProviderCreateWithURL (url);
	CFRelease (url);
	
	// Create the image object from that provider.
	image = CGImageCreateWithPNGDataProvider(provider, NULL, true, kCGRenderingIntentDefault);
	CGDataProviderRelease (provider);
	
	//	Generate a texture name
	glGenTextures(1, &_glId);
	glBindTexture(GL_TEXTURE_2D, _glId);
	
	//	configure the image to use linear interpolation when increasing or decreasing
	//	the texture in size to fit on a polygon.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//	get the width and height of the image
	_width = CGImageGetWidth(image);
	_height = CGImageGetHeight(image);
	
	//	create a device dependant color space
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    
	//	get the bitmap data into RGBA format with the help of Core Graphics
	void *imageData = malloc( _height * _width * 4 );
    CGContextRef context = CGBitmapContextCreate( imageData, _width, _height, 8, 4 * _width, colorSpace, 
												 kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big );
	
	//	flip the coordinates of the image before you feed it into OpenGL
	CGContextTranslateCTM (context, 0, _height);
	CGContextScaleCTM (context, 1.0, -1.0);
	
    CGColorSpaceRelease( colorSpace );
    CGContextClearRect( context, CGRectMake( 0, 0, _width, _height ) );
    CGContextTranslateCTM( context, 0, _height - _height );
    CGContextDrawImage( context, CGRectMake( 0, 0, _width, _height ), image );
	
	//	pass the image data into OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	
    CGContextRelease(context);	
    free(imageData);
}

void Texture2D::draw( const Rectangle& rectangle )
{
	glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);	
	
	GLfloat	vertices[] = 
	{	
		rectangle.x,						rectangle.y,						0.0,
		rectangle.x + rectangle.width,		rectangle.y,						0.0,
		rectangle.x,						rectangle.y + rectangle.height,		0.0,
		rectangle.x + rectangle.width,		rectangle.y + rectangle.height,		0.0 
	};

    GLfloat texCoords[] = {
        0.0, 1.0,
        1.0, 1.0,
        0.0, 0.0,
        1.0, 0.0
    };
    
    glLoadIdentity();
    
	Textures->bindTexture( _glId );
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable( GL_BLEND );
//	//
//	//
//	//
//	
//	//added
//	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
//	glEnable(GL_BLEND);	
//	glEnable(GL_TEXTURE_2D);	
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glAlphaFunc(GL_GREATER, 0.1f);
//	glEnable(GL_ALPHA_TEST);
//	
//	glColorPointer(4, GL_FLOAT, 0, 0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	// /added
//	
//	glBindTexture(GL_TEXTURE_2D, _name);
//	glVertexPointer(3, GL_FLOAT, 0, vertices);
//	glTexCoordPointer(2, GL_FLOAT, 0, coordinates);
//	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//	//
//	//
//	//
	
	// Save the current matrix to the stack
//	glPushMatrix();
//	
//	// Rotate around the Z axis by the angle defined for this image
//	glTranslatef(location.x, location.y, 0);
//	//glRotatef(-rotation, 0.0f, 0.0f, 1.0f);
//	glTranslatef(-location.x, -location.y, 0);
//	
//	// Set the glColor to apply alpha to the image.  This takes into account the gloabl alpha which is managed
//	// by the sharedDirector.  This allows us to fade all objects based on the global alpha
//	//glColor4f(_colorfilter.red, _colorfilter.green, _colorfilter.blue, _colorfilter.alpha * [_director globalAlpha]);
//	
//	// Set client states so that the Texture Coordinate Array will be used during rendering
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//	
//	// Enable Texture_2D
//	glEnable(GL_TEXTURE_2D);
//	
//	// Bind to the texture that is associated with this image.  This should only be done if the
//	// texture is not currently bound
//	Textures->bindTexture( _glId );
//	
//	
//	// Set up the VertexPointer to point to the vertices we have defined
//	glVertexPointer(2, GL_FLOAT, 0, &quadVertices);
//	
//	// Set up the TexCoordPointer to point to the texture coordinates we want to use
//	glTexCoordPointer(2, GL_FLOAT, 0, &texCoords);
//	
//	// Enable blending as we want the transparent parts of the image to be transparent
//	glEnable(GL_BLEND);
//	
//	// Setup how the images are to be blended when rendered.  The setup below is the most common
//	// config and handles transparency in images
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	
//	// Draw the vertices to the screen
//	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//	
//	// Now we are done drawing disable blending
//	glDisable(GL_BLEND);
//	
//	// Disable as necessary
//	glDisable(GL_TEXTURE_2D);
//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//	glDisableClientState(GL_VERTEX_ARRAY);
//	
//	// Restore the saved matrix from the stack
//	glPopMatrix();
}