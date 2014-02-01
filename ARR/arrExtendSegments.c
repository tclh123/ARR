//
//  arrExtendSegments.c
//  ARReadingHD
//
//  Created by tclh123 on 13-2-17.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#include <stdio.h>

#include "ar.h"


// 扩展线段
void arrExtendSegments(ARREdgeDetector *detector, ARRSegment *segments, int num)    // change segments //调用 arrExtendLine
{
    int i;
    for (i=0; i<num; i++) {
        // 改变 it 的 endpoint, startpoint
        ARRSegment *it = &segments[i];
        
        arrExtendLine(detector, &it->end.position, &it->slope, &it->end.slope, &it->end.position, 999);
        ARRVec *minus_slope = arrVecAlloc(-it->slope.x, -it->slope.y);
        arrExtendLine(detector, &it->start.position, minus_slope, &it->end.slope, &it->start.position, 999);
    }
}

Bool arrExtendLine(ARREdgeDetector *detector, ARRVec *startpoint, const ARRVec *slope, const ARRVec * gradient,
                   ARRVec * endpoint, const int maxlength )    // output endpoint
{
    int i;
    const ARRVec *normal = arrVecAlloc(slope->y, -slope->x);
    Bool merge = TRUE;
    
    // 确保接头位于边缘
    for (i = 0; i<maxlength; i++) {
        arrVecAdd(startpoint, slope);
        
        if (arrEdgeKernelX(detector, startpoint->x, startpoint->y) < ARR_THRESHOLD/2 &&
            arrEdgeKernelY(detector, startpoint->x, startpoint->y) < ARR_THRESHOLD/2) {
            merge = FALSE;
            break;
        }
        
        ARRVec tempGradient = arrEdgeGradientIntensity(detector, startpoint->x, startpoint->y);
        if (arrVecInnerProduct(&tempGradient, gradient)  > 0.38f) {
            continue;
        }
        tempGradient = arrEdgeGradientIntensity(detector, startpoint->x + normal->x, startpoint->y + normal->y);
        if (arrVecInnerProduct(&tempGradient, gradient)  > 0.38f) {
            continue;
        }
        tempGradient = arrEdgeGradientIntensity(detector, startpoint->x - normal->x, startpoint->y - normal->y);
        if (arrVecInnerProduct(&tempGradient, gradient)  > 0.38f) {
            continue;
        }
        
        merge = FALSE;
        break;
    }
    
    ARRVec *ret = arrVecAllocMinus(startpoint, slope);
    *endpoint = *ret;
    arrVecFree(ret);
    return merge;
}