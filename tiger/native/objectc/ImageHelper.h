//
//  ImageHelper.h
//  FactoryAppAirplane
//
//  Created by Wang zhen on 4/8/16.
//
//

#ifndef ImageHelper_h
#define ImageHelper_h

#import <Foundation/Foundation.h>

@interface ImageHelper : NSObject
{
}

- (void) moveImageToCameraRoll:(NSString *) file;

- (void)image:(UIImage *)image didFinishSavingWithError:(NSError *)error
  contextInfo:(void *)contextInfo;

@end

#endif /* ImageHelper_h */
