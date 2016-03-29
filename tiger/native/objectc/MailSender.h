//
//  MailSender.h
//  TheSolarSystem
//
//  Created by Wang zhen on 10/27/14.
//  Copyright (c) 2014 Y Factory. All rights reserved.
//

#ifndef __TheSolarSystem__MailSender__
#define __TheSolarSystem__MailSender__

#import <UIKit/UIKit.h>
#import <MessageUI/MessageUI.h>
#import <MessageUI/MFMailComposeViewController.h>

@interface MailSender : UIViewController<MFMailComposeViewControllerDelegate>
{
    UIViewController *currentModalViewController;
}

- (void) sendMailUsingInAppMailer:(const char*)toAddress title : (const char*)subject body: (const char*)s;

- (void) sendMailUsingInAppMailer:(const char *)toAddress title:(const char *)subject body:(const char *)s image: (const char*)filename;

- (void) releaseViewController;

- (void) createViewController;

@end

#endif /* defined(__TheSolarSystem__MailSender__) */
