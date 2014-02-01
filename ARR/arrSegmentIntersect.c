//
//  arrSegmentIntersect.c
//  ARReadingHD
//
//  Created by tclh123 on 13-2-14.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#include <stdio.h>

#include "matrix.h"

ARRVec arrSegmentIntersect(ARRSegment *seg, ARRSegment *seg2)
{
	float denom = ( (seg2->end.position.y - seg2->start.position.y)
                    * (seg->end.position.x - seg->start.position.x) )
                    -
                    ( (seg2->end.position.x - seg2->start.position.x)
                     * (seg->end.position.y - seg->start.position.y) );
	
    float nume_a = ( (seg2->end.position.x - seg2->start.position.x)
                     * (seg->start.position.y - seg2->start.position.y) )
                    -
                    ( (seg2->end.position.y - seg2->start.position.y)
                     * (seg->start.position.x - seg2->start.position.x));
    
	float ua = nume_a / denom;
	
	float x = seg->start.position.x + ua * (seg->end.position.x - seg->start.position.x);
	float y = seg->start.position.y + ua * (seg->end.position.y - seg->start.position.y);

	//ARRVec *intersection = arrVecAlloc(x, y);
    ARRVec intersection;
    intersection.x = x;
    intersection.y = y;
    
	return intersection;
}