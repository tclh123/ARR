//
//  arrFindChainOfLines.c
//  ARReadingHD
//
//  Created by tclh123 on 13-2-17.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#include <stdio.h>
#include "ar.h"


/*
 startSegment 没变
 segments 减少了
 segments_num 变了
 chain 增加了
 chain_num 变了
 length 变了
 */
int arrFindChainOfLines(ARREdgeDetector *detector,
                        ARRSegment *startSegment,
                        Bool atStartPoint,
                        ARRSegment *segments,
                        int *segments_num,
                        ARRSegment *chain,
                        int *chain_num,
                        int *length)    // change segments, chain, chain_num, length  // 其实chain_num + 1 == length ?
{
    int i;
    
    const ARRVec startPoint = atStartPoint ? startSegment->start.position : startSegment->end.position;
    
    for (i=0; i< *segments_num; i++) {
        // 线不能平行
        if (arrSegmentIsCompatible(startSegment, &segments[i])) {
            continue;
        }
        
        // 端和出发点应该是彼此接近的
        ARRVec tempPoint = atStartPoint ? segments[i].end.position : segments[i].start.position;
        ARRVec *temp2 = arrVecAllocMinus(&startPoint, &tempPoint);
        float squared_length = temp2->x * temp2->x + temp2->y * temp2->y;
        arrVecFree(temp2);
        if (squared_length > 16.0f) {       // 相隔不能大于 4个像素？？？？ //TODO: 各种魔数啊
            continue;
        }
        
        // 方向？顺时针围绕
        if ( (atStartPoint &&
              ( startSegment->slope.x * segments[i].slope.y - startSegment->slope.y * segments[i].slope.x <= 0)) ||
             (!atStartPoint &&
              ( startSegment->slope.x * segments[i].slope.y - startSegment->slope.y * segments[i].slope.x >= 0))) {
                 continue;
        }
        
        // 它似乎好得是真实的，但我们发现1个 :)所以这部分，看看链延长 ？？？？
        
        (*length)++;
        
        ARRSegment chainSegment = segments[i];      // 找到的 线段
        segments[i] = segments[--(*segments_num)];
        
        if (*length == 4) {
            chain[(*chain_num)++] = chainSegment;
            
            return 0;
        }
        
        if (!atStartPoint) {
            chain[(*chain_num)++] = chainSegment;
        }
        // 递归
        arrFindChainOfLines(detector, &chainSegment, atStartPoint, segments, segments_num, chain, chain_num, length);
        if (atStartPoint) {
            chain[(*chain_num)++] = chainSegment;
        }
        
        return 0;
    }
    
    // never?
    return 0;
}