//
//  arrSegmentIsEquel.c
//  ARReadingHD
//
//  Created by tclh123 on 13-2-14.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#include <stdio.h>
#include "matrix.h"

Bool arrSegmentIsEquel(ARRSegment *seg, ARRSegment *seg2)
{
    
    return (seg->start.position.x == seg2->start.position.x &&
            seg->start.position.y == seg2->start.position.y &&
            seg->end.position.x == seg2->end.position.x &&
            seg->end.position.y == seg2->end.position.y);
}