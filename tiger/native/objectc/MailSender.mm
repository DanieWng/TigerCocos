//
//  MailSender.cpp
//  TheSolarSystem
//
//  Created by Wang zhen on 10/27/14.
//  Copyright (c) 2014 Y Factory. All rights reserved.
//

#import "MailSender.h"
//#import "CCEAGLView.h"

@implementation MailSender

- (void)sendMailUsingInAppMailer:(const char *)toAddress title:(const char *)subject body:(const char *)s
{
    if ([MFMailComposeViewController canSendMail])
    {
        MFMailComposeViewController *mailer = [[MFMailComposeViewController alloc] init];
        
        mailer.mailComposeDelegate = self;
        
        NSString *emailSubject = [[NSString alloc] initWithUTF8String:subject ];
        
        [mailer setSubject: emailSubject ];
        
        NSString *emailBody = [[NSString alloc] initWithUTF8String:s];
        
        [mailer setMessageBody:emailBody isHTML:YES];
        
        NSArray *toRecipients = [NSArray arrayWithObjects:[NSString stringWithFormat:@"%s", toAddress], nil];
        
        [mailer setToRecipients:toRecipients];
        
        // only for iPad
//        mailer.modalPresentationStyle = UIModalPresentationPageSheet;
       
        [self createViewController];
        
        [currentModalViewController presentViewController:mailer animated:YES completion:^{}];
        
        [emailSubject release];
        [emailBody release];
    }
    else
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Failure"
                                                        message:@"Your device doesn't support the composer sheet"
                                                       delegate:nil
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles: nil];
        [alert show];
        [alert release];
    }
    
}

- (void)sendMailUsingInAppMailer:(const char *)toAddress title:(const char *)subject body:(const char *)s image:(const char *)filename
{
    if ([MFMailComposeViewController canSendMail])
    {
        MFMailComposeViewController *mailer = [[MFMailComposeViewController alloc] init];
        
        mailer.mailComposeDelegate = self;
        
        NSString *emailSubject = [[NSString alloc] initWithUTF8String:subject ];
        
        [mailer setSubject: emailSubject ];
        
        NSString *emailBody = [[NSString alloc] initWithUTF8String:s];
        
        [mailer setMessageBody:emailBody isHTML:YES];
        
        NSArray *toRecipients = [NSArray arrayWithObjects:[NSString stringWithFormat:@"%s", toAddress], nil];
        
        [mailer setToRecipients:toRecipients];
        
        NSArray  *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentDirectory = [paths objectAtIndex:0];
        NSString *getImagePath = [documentDirectory stringByAppendingPathComponent: [NSString stringWithFormat:@"%s", filename]];
        
        NSLog(@"file path: %s", [getImagePath UTF8String]);
        
        UIImage *image     = [UIImage imageWithContentsOfFile:getImagePath];
        NSData  *imageData = UIImagePNGRepresentation(image);
        [mailer addAttachmentData:imageData mimeType:@"image/png" fileName:[NSString stringWithFormat:@"%s", filename]];
        
        [self createViewController];
        [currentModalViewController presentViewController:mailer animated:YES completion:^{}];
    
        [emailBody release];
        [emailSubject release];
        
    }
    else
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Failure"
                                                        message:@"Your device doesn't support the composer sheet"
                                                       delegate:nil
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles:nil];
        [alert show];
        [alert release];
    }
}

- (void)mailComposeController:(MFMailComposeViewController*)controller didFinishWithResult:(MFMailComposeResult)result error:(NSError*)error
{
//    const char *message;
//    switch (result)
//    {
//        case MFMailComposeResultCancelled:
//            message = "Mail cancelled";
//            break;
//        case MFMailComposeResultSaved:
//            message = "Mail saved";
//            break;
//        case MFMailComposeResultSent:
//            message = "Mail send";
//            break;
//        case MFMailComposeResultFailed:
//            message = "Mail failed";
//            break;
//        default:
//            message = "Mail cancelled";
//            break;
//    }
    
    [currentModalViewController dismissViewControllerAnimated:YES completion:^{
        [self releaseViewController];
    }];

}

- (void) createViewController
{
    UIWindow *window = [[UIApplication sharedApplication] keyWindow];
    currentModalViewController = [UIViewController alloc];
    [window addSubview:currentModalViewController.view];
}

- (void) releaseViewController
{
    [currentModalViewController.view removeFromSuperview];
    [currentModalViewController release];
}

@end

