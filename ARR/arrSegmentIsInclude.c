//
//  arrSegmentIsInclude.c
//  ARReadingHD
//
//  Created by tclh123 on 13-2-14.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#include <stdio.h>
#include <math.h>

#include "matrix.h"

Bool arrSegmentIsInclude(ARRSegment *seg, ARREdge *edge)
{
    if(!arrEdgeIsCompatible(&seg->start, edge)) return FALSE;
    
	// distance to line: (AB x AC)/|AB|
	// A = r1
	// B = r2
	// C = cmp
    
	// AB ( r2.x - r1.x, r2.y - r1.y )
	// AC ( cmp.x - r1.x, cmp.y - r1.y )
    
    float cross = (seg->end.position.x - seg->start.position.x)
                    * (edge->position.y - seg->start.position.y);
	cross -= (seg->end.position.y- seg->start.position.y)
             * ( edge->position.x-seg->start.position.x);
    
    float d1 = seg->start.position.x - seg->end.position.x;
    float d2 = seg->start.position.y - seg->end.position.y;
    
    float d_length = (float)sqrt(d1*d1 + d2*d2);  // ToDo: vLength(...)
    
    float distance = cross / d_length;
    
	return fabs(distance) < 0.75f;
}