//
//  arrSegmentIsCompatible.c
//  ARReadingHD
//
//  Created by tclh123 on 13-2-14.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#include <stdio.h>

#include "matrix.h"

Bool arrSegmentIsCompatible(ARRSegment *seg, ARRSegment *seg2)
{
    //return  slope * cmp.slope > 0.92f; //0.38f; //cosf( 67.5f / 2 pi )
    
    return arrVecInnerProduct(&seg->slope, &seg2->slope) > 0.92f;
}