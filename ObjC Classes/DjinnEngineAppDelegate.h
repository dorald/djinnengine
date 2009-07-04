//
//  DjinnEngineAppDelegate.h
//  DjinnEngine
//
//  Created by Craig Giles on 7/2/09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import <UIKit/UIKit.h>
#include <string>

#include "FileUtil.h"

@class EAGLView;

@interface DjinnEngineAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    EAGLView *glView;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet EAGLView *glView;

@end

