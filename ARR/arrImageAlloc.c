//
//  arrImageAlloc.c
//  ARReadingHD
//
//  Created by tclh123 on 13-2-14.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "ar.h"

/*
ARRImage* arrImageAlloc(int width, int height)
{
    ARRImage *image = (ARRImage*)malloc(sizeof(ARRImage));
    if(image == NULL) return NULL;
    
    image->data = (ARRByte*)malloc(sizeof(ARRByte) * width * height);   // * 4 , 要看几通道的
    if(image->data == NULL) { free(image); return NULL; }
    
    image->width = width;
    image->height = height;
    
    return image;
}
*/