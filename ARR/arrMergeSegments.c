//
//  arrMergeSegments.c
//  ARReadingHD
//
//  Created by tclh123 on 13-2-17.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "ar.h"

// 用于维护距离
typedef struct {
    float distance;
    int index;
} SegmentDistance;
int cmp(const void *a,const void *b)
{
    return ((SegmentDistance *)a)->distance > ((SegmentDistance *)b)->distance;  // 能用 > 号么？
}

static SegmentDistance distances[ARR_EACH_REGION_SEGMENT_MAX];
static int distances_num;

// 合并线段，会调用 arrExtendLine
int arrMergeSegments(ARREdgeDetector *detector, int max_iterations,
                     ARRSegment *segments, int *num)  // output segments, num
{
    int i, j, k;
    int segments_num = *num;
    
    for (i=0; i<segments_num; i++) {
        ARRSegment *a = &segments[i];
        distances_num = 0;
        
        // 搜索所有部分，它可以合并
        for (j=0; j<segments_num; j++) {
            ARRSegment *b = &segments[j];
            
            ARRVec *temp = arrVecAllocMinus(&b->end.position, &a->start.position);
            arrVecNormalize(temp);
            if (i != j
                && arrVecInnerProduct(&b->slope, &a->slope) > 0.99f    // 它们应该具有相同的方向
                && arrVecInnerProduct(temp, &a->slope) > 0.99f      // 他们必须相互一致，并以同样的方式“点” ?
                ) {

                // 计算两条线之间的距离，存储于索引
                ARRVec *temp2 = arrVecAllocMinus(&b->start.position, &a->end.position);
                const int squared_length = temp2->x * temp2->x + temp2->y * temp2->y;
                arrVecFree(temp2);
                
                if (squared_length < 25*25) {
                    SegmentDistance t; t.distance = squared_length; t.index = j;
                    distances[distances_num++] = t;
                }
            }
            arrVecFree(temp);
        }
        if (distances_num == 0) {
            continue;
        }
        
        // 按距离排序
        qsort(distances, distances_num, sizeof(SegmentDistance), cmp);
        
        // 遍历所有的线段合并
        for (k=0; k<distances_num; k++) {
            j = distances[k].index;
            
            ARRVec startPoint = a->end.position;      // 首尾相接，组成一个更长的线段
            ARRVec endPoint = segments[j].start.position;
            
            ARRVec *temp = arrVecAllocMinus(&endPoint, &startPoint);
            const int length = arrVecLength(temp);
            arrVecNormalize(temp);
            const ARRVec slope = *temp;
            arrVecFree(temp);
            
            ARRSegment between;
            between.start.position = startPoint;
            between.end.position = endPoint;
            
            if (arrExtendLine(detector, &startPoint, &slope, &a->end.slope, &endPoint, length)) {
                // i和j被合并
                
                a->end = segments[j].end;
                ARRVec *temp = arrVecAllocMinus(&segments[i].end.position, &segments[i].start.position);
                arrVecNormalize(temp);
                a->slope = *temp;
                arrVecFree(temp);
                
                segments[j].remove = TRUE;
            } else {
                // between.drawLine(255, 0, 0); //??
                
                break;
            }
        }
        Bool merged = FALSE;
		
		// 根据remove标记，删除所有merge掉的线段
		for( j=0; j < segments_num; j++ ) {
			if( segments[j].remove ) {      // 这又是个变态写法，j 跟 segments_num 在循环时都会变
                segments[j] = segments[--segments_num];
                j--;
				merged = TRUE;
			}
		}
		if(merged) i = -1;  // 下次循环 i=0， 又重新开始，这变态写法不合理啊？ 有时候会白白重新开始吧？
    }
    
    *num = segments_num;
    return 0;
}