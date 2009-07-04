//
//  DjinnEngineAppDelegate.m
//  DjinnEngine
//
//  Created by Craig Giles on 7/2/09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import "DjinnEngineAppDelegate.h"
#import "EAGLView.h"

@implementation DjinnEngineAppDelegate

@synthesize window;
@synthesize glView;

- (void)applicationDidFinishLaunching:(UIApplication *)application {
    
	glView.animationInterval = 1.0 / 60.0;
	[glView startAnimation];
	
	application.statusBarOrientation = UIInterfaceOrientationLandscapeRight;
	
	//	Hide the status bar
	application.statusBarHidden = YES;
	
	//	the paths to the iPhone Documents Directory and Application directory
	char iphoneDocDirectory[PATH_MAX];
	
	// get the documents directory, where we will write configs and save games
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];
	[documentsDirectory getCString: iphoneDocDirectory 
						 maxLength: sizeof( iphoneDocDirectory ) - 1
						  encoding: NSASCIIStringEncoding ];
	
	std::string CPP_iphoneDocDirectory( iphoneDocDirectory );
	CPP_iphoneDocDirectory.append( 1, '/' );
	Files->setDocumentsDirectory( CPP_iphoneDocDirectory );	
}


- (void)applicationWillResignActive:(UIApplication *)application {
	glView.animationInterval = 1.0 / 5.0;
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
	glView.animationInterval = 1.0 / 60.0;
}


- (void)dealloc {
	[window release];
	[glView release];
	[super dealloc];
}

@end
