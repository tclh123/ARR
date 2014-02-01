//
//  arrSegmentInit.c
//  ARR_OSX
//
//  Created by tclh123 on 13-2-17.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#include <stdio.h>
#include "ar.h"

void arrSegmentInit(ARRSegment* seg)
{
    seg->remove = FALSE;
    seg->start_corner = FALSE;
    seg->end_corner = FALSE;
    seg->num = 0;
}