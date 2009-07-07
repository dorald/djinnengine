

#include "Animation.h"

//
//	Initialize Static members
//
string Animation::FileExtension = "dat";

#pragma mark Constructor(s) / Destructor
Animation::Animation() 
{
	asset = "";
	name = "";
	frames = NULL;
	imageNames = NULL;
	frameInterval = -1;
	isLoop = false;
	pauseAtEnd = false;
}

Animation::Animation( const Animation &copy )
{
	*this = copy;
}

Animation::~Animation()
{
	for ( int i = 0; i < numberOfFrames; ++i )
	{
		//delete frames[i]; //REMOVE: used before TextureController
		frames[i]->releaseReference();
		frames[i] = NULL;
	}
	
	delete [] frames;
}

#pragma mark Object Serialization >> and << functions

std::istream& operator >>( std::istream &inStream, Animation &value )
{	
	string asset;
	string name;
	int numberOfFrames;
	int tempImageType;
	ImageType imageType;
	string *imageNames;
	float frameInterval;
	bool isLoop;
	bool pauseAtEnd;
	
	inStream >> asset;
	inStream >> name;
	inStream >> numberOfFrames;
	inStream >> tempImageType;
	imageType = static_cast<ImageType> ( tempImageType );
	
	imageNames = new string[numberOfFrames];
	for ( int i = 0; i < numberOfFrames; ++i )
		inStream >> imageNames[i];
	
	inStream >> frameInterval;
	inStream >> isLoop;
	inStream >> pauseAtEnd;
	
	value.setAsset( asset );
	value.setName( name );
	value.setFrameInterval( frameInterval );
	value.setIsLoop( isLoop );
	value.setPauseAtEnd( pauseAtEnd );
	
	value.loadAnimation( imageNames, imageType, numberOfFrames );
	
	delete [] imageNames;
	
	
	return inStream;
}

std::ostream& operator <<( std::ostream &oStream, Animation &value )
{

	string asset = value.getAsset();
	string name = value.getName();
	int numberOfFrames = value.getNumberOfFrames();
	ImageType imageType = value.getImageType();
	string *imageNames = value.getImageAssets();
	
	float frameInterval = value.getFrameInterval();
	bool isLoop = value.getIsLoop();
	bool pauseAtEnd = value.getPauseAtEnd();
	
	oStream << asset << '\n';
	oStream << name << '\n';
	oStream << numberOfFrames << '\n';
	oStream << imageType << '\n';
	for ( int i = 0; i < numberOfFrames; ++i )
		oStream << imageNames[i] << '\n';
	
	oStream << frameInterval << '\n';
	oStream << isLoop << '\n';
	oStream << pauseAtEnd << '\n';
	
	return oStream;
}

#pragma mark Overloaded operators
Animation& Animation::operator =( const Animation &copy )
{
	//self assignment check
	if ( this == &copy )
		return *this;

	//	copy all of the data members to the new object
	this->name = copy.getName();
	this->numberOfFrames = copy.getNumberOfFrames();
	this->imageType = copy.getImageType();
	this->frameInterval = copy.getFrameInterval();
	this->isLoop = copy.getIsLoop();
	this->pauseAtEnd = copy.getPauseAtEnd();

	
	//	load the animations 
	loadAnimation( copy.getImageAssets(), imageType, numberOfFrames );
	
	return *this;
}
bool Animation::operator ==( const Animation &value ) const
{
	return ( this->getName() == value.getName() );
}

bool Animation::operator !=( const Animation &value ) const
{
	return !(*this == value);
}

Texture2D* Animation::operator []( const int value ) const 
{
	return getSpriteToDraw( value );
}

#pragma mark Properties

void Animation::setName( string value )
{
	//	Transform the string to uppercase
	std::transform(value.begin(), value.end(), value.begin(), ::toupper);
	
	//	compare and assign if needed
	if ( name != value )
		name = value;
}

#pragma mark Functions

Texture2D* Animation::getSpriteToDraw( const int value ) const
{
	if ( frames == NULL )
		return NULL;
	
	if ( value >= numberOfFrames || value < 0 )
		return frames[ numberOfFrames - 1 ];
	
	return frames[value];
}

void Animation::loadAnimation( const string *imageNames, const ImageType &type, const int numberOfFrames )
{
	//	set the image type
	imageType = type;
	
	//	allocate new memory for the image names
	this->imageNames = new string[ numberOfFrames ];
	
	//	create an array of images
	Texture2D **images = new Texture2D*[numberOfFrames];

	//	load each image with a texture from the list of names
	//	passed into the function
	for ( int i = 0; i < numberOfFrames; ++i )
	{
		this->imageNames[i] = imageNames[i];
		images[i] = Textures->getTexture( imageNames[i] );
		images[i]->addReference();
		//images[i] = new Texture2D( imageNames[i], type ); //used before TextureController
	}
	
	//	now that the images are loaded, load the animation
	loadAnimationFrames( images, numberOfFrames );
}

void Animation::loadAnimationFrames( Texture2D **images, const int numberOfFrames )
{
	//	set the number of frames for this animation
	setNumberOfFrames( numberOfFrames );	
	frames = images;
}

