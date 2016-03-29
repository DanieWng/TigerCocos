//
//  WebView.h
//  TheSolarSystem
//
//  Created by Wang zhen on 10/27/14.
//  Copyright (c) 2014 Y Factory. All rights reserved.
//

#ifndef __TheSolarSystem__WebView__
#define __TheSolarSystem__WebView__

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>
#import <UIKit/UIKit.h>


@interface WebView : UIViewController<UIWebViewDelegate, UIAlertViewDelegate>
{
    
    UIViewController *currentModalViewController;
    
    UIWebView *webView;
    
    UIToolbar *toolBar;
    
    UIButton  *backButton;
    
    UIView* mview;
    
}

- (void) setLayerWebView : (const char*) url;

- (void) backClicked : (id)sender;

@end

#endif /* defined(__TheSolarSystem__WebView__) */
