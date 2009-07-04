
#include "Texture2D.h"

Texture2D::Texture2D(const string &filename, ImageType imageType) 
{
	//
	//	CGImageRef and several functions used in this portion of the Texture file
	//	are specific to the mac platform. When doing cross platform applications,
	//	an external library such as libpng will need to be used to load the texture
	//	into memory.
	//
	CGImageRef image;
	CGDataProviderRef provider;
	CFStringRef name;
	CFURLRef url;
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	
	// Get the URL to the bundle resource.
	name = CFStringCreateWithCString (NULL, filename.c_str(), kCFStringEncodingUTF8);
	
	if ( imageType == JPG )
	{
		url = CFBundleCopyResourceURL(mainBundle, name, CFSTR("jpg"), NULL);
	}
	else if ( imageType == PNG )
	{
		url = CFBundleCopyResourceURL(mainBundle, name, CFSTR("png"), NULL);
	}
	else
	{
		printf("Invalid Data Type");
	}
	
	CFRelease(name);
	
	// Create the data provider object
	provider = CGDataProviderCreateWithURL (url);
	CFRelease (url);
	
	// Create the image object from that provider.
	image = createImageObject(provider, imageType);
	CGDataProviderRelease (provider);
	
	initWithImage(image);
}

//
//	This constructor will eventually be used to initialize the Texture2D with a string 
//	and use that string a a texture. I need to research ways of doing this, this
//	might not even be the right way of doing that. 
//
Texture2D::Texture2D( const string &text, 
					 const Size &dimensions, 
					 const TextAlignmentType &alignment, 
					 const string &fontName, 
					 const float &fontSize )
//	: kMaxTextureSize(1024)
{
#warning TODO:: Initialize OpenGL and bind a the given text to a texture
}

Texture2D::Texture2D( const Texture2D &copy )
{
	*this = copy;
}

Texture2D::~Texture2D()
{
}

Texture2D &Texture2D::operator =( const Texture2D &copy )
{
	//self assignment check
	if ( this == &copy )
		return *this;
	
	this->_name = copy.getName();
	this->_size = copy.getSize();
	this->_width = copy.getWidth();
	this->_height = copy.getHeight();
	this->_format = copy.getFormat();
	this->_maxS = copy.getMaxS();
	this->_maxT = copy.getMaxT();
	
	return *this;
}

CGImageRef Texture2D::createImageObject(const CGDataProviderRef &provider, ImageType imageType)
{
	if ( imageType == JPG )
	{
		return CGImageCreateWithJPEGDataProvider(provider, NULL, true, kCGRenderingIntentDefault);
	}
	else	//image is PNG
	{
		return CGImageCreateWithPNGDataProvider(provider, NULL, true, kCGRenderingIntentDefault);
	}
}

void Texture2D::initWithData(const void* data, Texture2DPixelFormat pixelFormat, int width, int height, CGSize size)
{
	GLint					saveName;
	
	glGenTextures(1, &_name);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &saveName);
	glBindTexture(GL_TEXTURE_2D, _name);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	switch(pixelFormat) 
	{				
		case kTexture2DPixelFormat_RGBA8888:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;
		case kTexture2DPixelFormat_RGB565:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
			break;
		case kTexture2DPixelFormat_A8:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
			break;
		default:
			printf("Invalid Format");
	}
	
	glBindTexture(GL_TEXTURE_2D, saveName);
		
	_size = size;
	_width = width;
	_height = height;
	_format = pixelFormat;
	_maxS = size.width / (float)width;
	_maxT = size.height / (float)height;
			
}

void Texture2D::initWithImage(const CGImageRef &incImage)
{
	int						width,
							height,
							i;
	CGContextRef			context = NULL;
	void*					data = NULL;;
	CGColorSpaceRef			colorSpace;
	void*					tempData;
	unsigned int*			inPixel32;
	unsigned short*			outPixel16;
	bool					hasAlpha;
	CGImageAlphaInfo		info;
	CGAffineTransform		transform;
	CGSize					imageSize;
	Texture2DPixelFormat    pixelFormat;
	CGImageRef				image;
	bool					sizeToFit = false;
	
	image = incImage;
	
	if(image == NULL) {
		printf("image is null");
		return;
	}
	
	
	info = CGImageGetAlphaInfo(image);
	hasAlpha = ((info == kCGImageAlphaPremultipliedLast) || 
				(info == kCGImageAlphaPremultipliedFirst) || 
				(info == kCGImageAlphaLast) || 
				(info == kCGImageAlphaFirst) 
					? true : false);
	
	if( CGImageGetColorSpace(image) ) 
	{
		if(hasAlpha)
			pixelFormat = kTexture2DPixelFormat_RGBA8888;
		else
			pixelFormat = kTexture2DPixelFormat_RGB565;
	} 
	else  //NOTE: No colorspace means a mask image
		pixelFormat = kTexture2DPixelFormat_A8;
	
	
	imageSize = CGSizeMake(CGImageGetWidth(image), CGImageGetHeight(image));
	transform = CGAffineTransformIdentity;
	width = imageSize.width;
	
	//
	//	Resize the width of the image to be a power of 2
	//
	if( (width != 1) && (width & (width - 1)) ) 
	{
		i = 1;
		
		while((sizeToFit ? 2 * i : i) < width)
			i *= 2;
		
		width = i;
	}
	
	height = imageSize.height;
	
	//
	//	resize the height of the image to be a power of 2
	//
	if( (height != 1) && (height & (height - 1)) ) 
	{
		i = 1;
		while((sizeToFit ? 2 * i : i) < height)
			i *= 2;
		height = i;
	}
	while((width > kMaxTextureSize) || (height > kMaxTextureSize)) 
	{
		width /= 2;
		height /= 2;
		transform = CGAffineTransformScale(transform, 0.5, 0.5);
		imageSize.width *= 0.5;
		imageSize.height *= 0.5;
	}
	
	switch(pixelFormat) {		
		case kTexture2DPixelFormat_RGBA8888:
			colorSpace = CGColorSpaceCreateDeviceRGB();
			data = malloc(height * width * 4);
			context = CGBitmapContextCreate(data, width, height, 8, 4 * width, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
			CGColorSpaceRelease(colorSpace);
			break;
		case kTexture2DPixelFormat_RGB565:
			colorSpace = CGColorSpaceCreateDeviceRGB();
			data = malloc(height * width * 4);
			context = CGBitmapContextCreate(data, width, height, 8, 4 * width, colorSpace, kCGImageAlphaNoneSkipLast | kCGBitmapByteOrder32Big);
			CGColorSpaceRelease(colorSpace);
			break;
			
		case kTexture2DPixelFormat_A8:
			data = malloc(height * width);
			context = CGBitmapContextCreate(data, width, height, 8, width, NULL, kCGImageAlphaOnly);
			break;				
		default:
			printf("Invalid pixel format");
	}
	
	
	CGContextClearRect(context, CGRectMake(0, 0, width, height));
	CGContextTranslateCTM(context, 0, height - imageSize.height);
	
	if(!CGAffineTransformIsIdentity(transform))
		CGContextConcatCTM(context, transform);
	CGContextDrawImage(context, CGRectMake(0, 0, CGImageGetWidth(image), CGImageGetHeight(image)), image);
	
	//Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGGBBBBB"
	if(pixelFormat == kTexture2DPixelFormat_RGB565) 
	{
		tempData = malloc(height * width * 2);
		inPixel32 = (unsigned int*)data;
		outPixel16 = (unsigned short*)tempData;
		
		for(i = 0; i < width * height; ++i, ++inPixel32)
			*outPixel16++ = ((((*inPixel32 >> 0) & 0xFF) >> 3) << 11) | ((((*inPixel32 >> 8) & 0xFF) >> 2) << 5) | ((((*inPixel32 >> 16) & 0xFF) >> 3) << 0);
		
		free(data);
		data = tempData;
	}
	
	initWithData(data, pixelFormat, width, height, imageSize);
	
	CGContextRelease(context);
	free(data);
}


//
//	Drawing methods
//	
void Texture2D::drawInRect(const Rectangle &drawRect)
{
	CGRect rect = CGRectMake(drawRect.x, drawRect.y, drawRect.width, drawRect.height);
	
	GLfloat	 coordinates[] = 
	{  
		0,		_maxT,
		_maxS,	_maxT,
		0,		0,
		_maxS,	0  
	};
	
	//		GLfloat	vertices[] = 
	//		{	
	//			rect.origin.x,							rect.origin.y,							0.0,
	//			rect.origin.x + rect.size.width,		rect.origin.y,							0.0,
	//			rect.origin.x,							rect.origin.y + rect.size.height,		0.0,
	//			rect.origin.x + rect.size.width,		rect.origin.y + rect.size.height,		0.0 
	//		};
	//	
	//
	//	Modified by Craig Giles to flip the texture upside down while using my coord system
	//
	GLfloat	vertices[] = 
	{
		rect.origin.x,							rect.origin.y + rect.size.height,		0.0,
		rect.origin.x + rect.size.width,		rect.origin.y + rect.size.height,		0.0, 
		rect.origin.x,							rect.origin.y,							0.0,
		rect.origin.x + rect.size.width,		rect.origin.y,							0.0
		
	};
	
	//added
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);	
	glEnable(GL_TEXTURE_2D);	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glAlphaFunc(GL_GREATER, 0.1f);
	glEnable(GL_ALPHA_TEST);
	
	glColorPointer(4, GL_FLOAT, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// /added
	
	glBindTexture(GL_TEXTURE_2D, _name);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, coordinates);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

