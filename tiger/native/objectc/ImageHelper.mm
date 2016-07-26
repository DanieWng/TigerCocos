//
//  ImageHelper.m
//  FactoryAppAirplane
//
//  Created by Wang zhen on 4/8/16.
//
//

#import "ImageHelper.h"

@implementation ImageHelper

- (void) moveImageToCameraRoll:(NSString *)file{
    UIImage *img = [UIImage imageWithContentsOfFile:file];
    
    UIImageWriteToSavedPhotosAlbum(img,
                                   self,
                                   @selector(image:didFinishSavingWithError:contextInfo:),
                                   nil);
}

- (void) image:(UIImage *)image didFinishSavingWithError:(NSError *)error contextInfo:(void *)contextInfo{
    // Was there an error?
    if (error != NULL)
    {
        // Show error message...
        NSLog(@"Move Photo in CameraRoll - FAILD - %@", error);
    }
    else  // No errors
    {
        // Show message image successfully saved
        NSLog(@"Move Photo in CameraRoll - SUCESSED");
    }
}

@end


