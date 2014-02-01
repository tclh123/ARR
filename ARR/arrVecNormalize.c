//
//  arrVecNormalize.c
//  ARReadingHD
//
//  Created by tclh123 on 13-2-16.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include "matrix.h"

int arrVecNormalize(ARRVec *v)
{
    float length = arrVecLength(v);
//    v->x /= length;       // 别用除法，精度问题
//    v->y /= length;
    
    const float inv_length = 1.0f / length;
    v->x *= inv_length;
    v->y *= inv_length;
    
    return 0;
}

float arrVecLength(ARRVec *v)
{
    return (float)sqrt(v->x*v->x + v->y*v->y);
}