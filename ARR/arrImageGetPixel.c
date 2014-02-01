//
//  arrImageGetPixel.c
//  ARReadingHD
//
//  Created by tclh123 on 13-2-14.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#include <stdio.h>

#include "ar.h"

ARRByte arrImageGetPixel(ARRImage *image, int x, int y, int channel) {
    x = x>0?x:0;
    y = y>0?y:0;
    
    x = x<image->width?x:image->width-1;
    y = y<image->height?y:image->height-1;
    
    int offset = ((x + (y*image->width)) * 3)+channel;
    return image->data[offset];
}

//??
float arrImageGetPixelColor(ARRImage *image, int x, int y, int channel) {  // 就是取整个？ channel = 0 ？
    ARRByte pixel = arrImageGetPixel(image, x, y, channel);
    return (float)pixel;
}