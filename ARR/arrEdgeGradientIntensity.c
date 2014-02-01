//
//  arrEdgeGradientIntensity.c
//  ARReadingHD
//
//  Created by tclh123 on 13-2-16.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#include <stdio.h>
#include <math.h>

#include "ar.h"

ARRVec arrEdgeGradientIntensity(ARREdgeDetector *detector, int x, int y)
{
    int gx = arrImageGetPixelColor(detector->image, x-1, y-1, 0);
    gx += 2 * arrImageGetPixelColor(detector->image, x, y-1, 0);
    gx += arrImageGetPixelColor(detector->image, x+1, y-1, 0);
    gx -= arrImageGetPixelColor(detector->image, x-1, y+1, 0);
    gx -= 2 * arrImageGetPixelColor(detector->image, x, y+1, 0);
    gx -= arrImageGetPixelColor(detector->image, x+1, y+1, 0);
    
    int gy = arrImageGetPixelColor(detector->image, x-1, y-1, 0);
    gy += 2 * arrImageGetPixelColor(detector->image, x-1, y, 0);
    gy += arrImageGetPixelColor(detector->image, x-1, y+1, 0);
    gy -= arrImageGetPixelColor(detector->image, x+1, y-1, 0);
    gy -= 2 * arrImageGetPixelColor(detector->image, x+1, y, 0);
    gy -= arrImageGetPixelColor(detector->image, x+1, y+1, 0);
    
    // TODO: Vector2f.get_normalized() ?
//    double length = sqrt((double)gx*gx + (double)gy*gy);
//    ARRVec vec;
//    vec.x = (double)gy / length;
//    vec.y = (double)gx / length;
    
    ARRVec vec;
    vec.x = gy;
    vec.y = gx;
    arrVecNormalize(&vec);
    
    return vec;
}