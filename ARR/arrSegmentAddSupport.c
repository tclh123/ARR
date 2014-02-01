//
//  arrSegmentAddSupport.c
//  ARReadingHD
//
//  Created by tclh123 on 13-2-14.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#include <stdio.h>

#include "matrix.h"

int arrSegmentAddSupport(ARRSegment *seg, ARREdge *edge)
{
    if(seg->num + 1 > ARR_EACH_SEGMENT_EDGES_MAX) return -1;
    
    // edge：要不要重新alloc一个？ ANS：要，外面的引用直接free掉
    
    ARREdge *edgeNew = arrEdgeAlloc(edge->position.x, edge->position.y);
    edgeNew->slope = edge->slope;
    
    if (edgeNew == NULL) {
        return -1;
    }
        
    seg->supportEdgels[seg->num++] = *edgeNew;
    arrEdgeFree(edgeNew);
    
    return 0;
}