//
//  vAdd.c
//  ARReadingHD
//
//  Created by tclh123 on 13-2-17.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#include <stdio.h>

#include "matrix.h"

int arrVecAdd(ARRVec *vec, const ARRVec *add)
{
    vec->x += add->x;
    vec->y += add->y;
    return 0;
}