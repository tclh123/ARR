//
//  vAlloc.c
//  ARReadingHD
//
//  Created by tclh123 on 13-2-14.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

ARRVec * arrVecAlloc(float x, float y)
{
    ARRVec *vec = (ARRVec*)malloc(sizeof(ARRVec));
    if(vec == NULL) return NULL;
    
    vec->x = x;
    vec->y = y;
    
    return vec;
}

ARRVec* arrVecAllocMinus(const ARRVec* a, const ARRVec* b)
{
    return arrVecAlloc(a->x-b->x, a->y-b->y);
}