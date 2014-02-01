//
//  arrMarkerCopyChain.c
//  ARReadingHD
//
//  Created by tclh123 on 13-2-17.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#include <stdio.h>
#include "ar.h"

void arrMarkerCopyChain(ARRMarker *marker, ARRSegment *chain, int num)
{
    int i;
    for (i=0; i<num; i++) {
        marker->chain[i] = chain[i];
    }
    marker->num = num;
}