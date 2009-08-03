/*
 *  Texture2D.cpp
 *  DjinnEngine
 *
 *  Created by Craig Giles on 7/5/09.
 *
 */

#include "Texture2D.h"
#include "TextureController.h"

#pragma mark
#pragma mark Constructor(s) / Destructor
#pragma mark ---------

Texture2D::Texture2D( const std::string &asset )
{
	loadFile(asset);
	vertices = new GLfloat[12];
	texCoords = new GLfloat[12];
}
Texture2D::~Texture2D()
{
	delete [] vertices;
	delete [] texCoords;
	
	glDeleteTextures(1, &_glId);
}

#pragma mark
#pragma mark Reference Counting
#pragma mark ---------

void Texture2D::addReference()
{
	++_referenceCount;
}
void Texture2D::releaseReference()
{
	--_referenceCount;
	
	if (_referenceCount == 0)
		Textures->releaseTexture(assetName);
}

void Texture2D::bindTexture() 
{ 
	Textures->bindTexture( _glId ); 
}

#pragma mark
#pragma mark Load Texture from file.
#pragma mark ---------

void Texture2D::loadFile( const std::string& asset )
{
	CGImageRef			image;
	CGDataProviderRef	provider;
	CFStringRef			name;
	CFURLRef			url;
	CFBundleRef			mainBundle = CFBundleGetMainBundle();
	CGAffineTransform	transform = CGAffineTransformIdentity;
	bool				sizeToFit = false;
	int					i;
	
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
	Vector2 imageSize(CGImageGetWidth(image), CGImageGetHeight(image));
	_width = imageSize.x;
	_height = imageSize.y;
	
	//	adjust the images width and height to be powers of 2
	if( (_width != 1) && (_width & (_width - 1)) ) 
		_width = MathUtil::nextPowerOfTwo( _width );

	
	if( (_height != 1) && (_height & (_height - 1)) ) 
		_height = MathUtil::nextPowerOfTwo( _height );

	
	//	scale down an image greater than the max texture size
	while((_width > kMaxTextureSize) || (_height > kMaxTextureSize)) 
	{
		_width /= 2;
		_height /= 2;
		transform = CGAffineTransformScale(transform, 0.5, 0.5);
		imageSize.x *= 0.5;
		imageSize.y *= 0.5;
	}
	
	//	create a device dependant color space
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    
	//	get the bitmap data into RGBA format with the help of Core Graphics
	void *imageData = malloc( _height * _width * 4 );
    CGContextRef context = CGBitmapContextCreate( imageData, _width, _height, 8, 4 * _width, colorSpace, 
												 kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big );
	
	//	flip the coordinates of the image before you feed it into OpenGL
	//	Since I changed the coord system, this makes the image appear right side
	//	up and not upside down.
//	CGContextTranslateCTM (context, 0, _height);
//	CGContextScaleCTM (context, 1.0, -1.0);
	
    CGColorSpaceRelease( colorSpace );
    CGContextClearRect( context, CGRectMake( 0, 0, _width, _height ) );
    CGContextTranslateCTM( context, 0, _height - _height );
    CGContextDrawImage( context, CGRectMake( 0, 0, _width, _height ), image );
	
	//ensure our data is accurate
	_maxS = imageSize.x / (float)_width;
	_maxT = imageSize.y / (float)_height;
	
	//	pass the image data into OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	
    CGContextRelease(context);	
    free(imageData);
}
