//
//  WebView.cpp
//  TheSolarSystem
//
//  Created by Wang zhen on 10/27/14.
//  Copyright (c) 2014 Y Factory. All rights reserved.
//
#import "WebView.h"
#import "cocos2d.h"
#import "platform/ios/CCEAGLView-ios.h"

@implementation WebView

- (id) init
{
    self = [super init];
    
    if (self)
    {
        
    }
    
    return  self;
}

- (void) dealloc
{
    [toolBar release];
    [webView release];
    [backButton release];
    [currentModalViewController release];
    [super dealloc];
}

////// 5.0 under
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    
    NSLog(@"shouldRotate");
    return YES;
}
//
///// 6.0
- (BOOL)shouldAutorotate
{
    NSLog(@"should rotate");
    return YES;
}
//
/// support direction
- (NSUInteger) supportedInterfaceOrientations
{
    NSLog(@"support orientation");
    
    if (self.supportedInterfaceOrientations == UIInterfaceOrientationMaskLandscape)
    {
        NSLog(@"land");
    }else if(self.supportedInterfaceOrientations == UIInterfaceOrientationMaskPortrait)
    {
        NSLog(@"portrait");
    }
    
    return UIInterfaceOrientationMaskAll;
}

/// rotate began
- (void)willAnimateRotationToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration
{
    [super willRotateToInterfaceOrientation:toInterfaceOrientation duration:duration];
    
    CGRect winsize = [[UIScreen mainScreen] bounds];
    
    [webView setFrame:CGRectMake(0, 0, winsize.size.height, winsize.size.width)];
    
    NSLog(@"call rotate");
}

- (void) setLayerWebView:(const char *)url
{
    CGRect winsize = [[UIScreen mainScreen] bounds];
    UIImage *toolbarbg = [UIImage imageNamed:@"main/info_bar.png"];
    UIImage *backbtnbg = [UIImage imageNamed:@"main/info_back.png"];
    
    NSLog(@"width[%f] height[%f]", winsize.size.width, winsize.size.height);
    
    float webview_width, webview_height;
    
    if (winsize.size.width > winsize.size.height)
    {
        webview_width = winsize.size.width;
        webview_height = winsize.size.height;
    }else
    {
        webview_width = winsize.size.height;
        webview_height = winsize.size.width;
    }

    webView = [[UIWebView alloc] initWithFrame: CGRectMake(0,
                                                           toolbarbg.size.height-4,
                                                           webview_width,
                                                           webview_height)];
    
    webView.delegate = self;
    
    NSString *urlBase = [NSString stringWithCString:url encoding:NSUTF8StringEncoding];
    
    [webView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:urlBase]]];
    
    [webView setUserInteractionEnabled:YES];
    
    toolBar = [[UIToolbar alloc] initWithFrame:CGRectMake(0, 0, webview_width, 48)];
    
    [toolBar setBackgroundImage:toolbarbg forToolbarPosition:UIBarPositionAny barMetrics:UIBarMetricsDefault];
    
    backButton = [UIButton buttonWithType:UIButtonTypeCustom];
    backButton.bounds = CGRectMake(0, 0, backbtnbg.size.width, backbtnbg.size.height);
    [backButton setImage:backbtnbg forState:UIControlStateNormal];
    [backButton addTarget:self action:@selector(backClicked:) forControlEvents:UIControlEventTouchUpInside];
    
    CGRect rect = backButton.frame;
    rect.origin.x = 5.0f;
    rect.origin.y = 4.0f;
    backButton.frame = rect;
    
    [toolBar addSubview:backButton];
    
    [self createViewController];
    [currentModalViewController.view addSubview:toolBar];
    [currentModalViewController.view addSubview:webView];

    auto view = cocos2d::Director::getInstance()->getOpenGLView();
    auto eaglview = (CCEAGLView *) view->getEAGLView();
    [eaglview addSubview:currentModalViewController.view];
    
}


- (void) webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
    if ([error code] != -999 && error != NULL) { //error -999 happens when the user clicks on something before it's done loading.
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Network Error" message:@"Unable to load the page. Please keep network connection."
                                                       delegate:self cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
        [alert show];
        [alert release];
    }
}

- (void) backClicked:(id)sender
{
    [webView removeFromSuperview];
    webView = NULL;
    
    [toolBar removeFromSuperview];
    toolBar = NULL;

    [self releaseViewController];
}

- (void) createViewController
{
//    UIWindow *window = [[UIApplication sharedApplication] keyWindow];
    currentModalViewController = [UIViewController alloc];
//    [window addSubview:currentModalViewController.view];
}

- (void) releaseViewController
{
    [currentModalViewController.view removeFromSuperview];
}

@end


























