//
//  arrFindSegments.c
//  ARReadingHD
//
//  Created by tclh123 on 13-2-16.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ar.h"

static ARRSegment segments_static[ARR_EACH_REGION_SEGMENT_MAX];
static int segments_num;

// 从 给定的 点集 中找线段
// edges 会改变，反正后面用不到了。
int arrFindSegments(ARREdgeDetector *detector, ARREdge *edges, int edges_num,
                    ARRSegment **segments, int *num)    //output segment, change num
{
//    srand(time(NULL));
    srand(0);
    
    int i, j;
    segments_num = 0;
    ARRSegment segmentInRun;
    arrSegmentInit(&segmentInRun);
    
    do {
        segmentInRun.num = 0;  //clear support edges
        
        // 用随即算法 找出 support 最多的 线段，赋给 segmentInRun
        for (i = 0; i < 25; i++) {
            
            ARREdge r1, r2;
            
            const int max_iterations = 100;
            int iteration = 0, ir1, ir2;
            
            // pick 2 random edges which are compatible
            do {
                ir1 = (rand()%edges_num);
                ir2 = (rand()%edges_num);
                
                r1 = edges[ir1];        // ARREdge中无成员指针，可以直接赋值
                r2 = edges[ir2];

                iteration++;
            } while ( (ir1 == ir2 || !arrEdgeIsCompatible(&r1, &r2)) && iteration < max_iterations);
            
            if (iteration < max_iterations) {
                
                //2 edges
                ARRSegment segment;     //TODO: ARRSegment需要一个初始化，不然 segment.num 没东西。 注意所有东西 struct的初始化。。
                arrSegmentInit(&segment);
                segment.start = r1;
                segment.end = r2;
                segment.slope = r1.slope;
                
                // 把 点集 中 support 此线段的 AddSupport
                for (j = 0; j<edges_num; j++) {
                    if (arrSegmentIsInclude(&segment, &edges[j])) {
                        arrSegmentAddSupport(&segment, &edges[j]);
                    }
                }
                
                if (segment.num > segmentInRun.num) {
                    segmentInRun = segment;
                }
            }
        }// 找到 support 最多的 线段
        //print_segment(&segmentInRun, 0); printf("\n");
        
        
        // 根据 supporter 确定的线的斜率
        if (segmentInRun.num >= ARR_EDGES_ONLINE) {
            float u1 = 0;
            float u2 = 50000;
            ARRVec *slope = arrVecAlloc(segmentInRun.start.position.x - segmentInRun.end.position.x,
                                              segmentInRun.start.position.y - segmentInRun.end.position.y);
            
            ARRVec *orientation = arrVecAlloc(-segmentInRun.start.slope.y, segmentInRun.start.slope.x);
            
            if (abs(slope->x) <= abs(slope->y)) {
                for (i=0; i<segmentInRun.num; i++) {
                    ARREdge it = segmentInRun.supportEdgels[i];
                    if (it.position.y > u1) {
                        u1 = it.position.y;
                        segmentInRun.start = it;
                    }
                    if (it.position.y < u2) {
                        u2 = it.position.y;
                        segmentInRun.end = it;
                    }
                }
            } else {
                for (i=0; i<segmentInRun.num; i++) {
                    ARREdge it = segmentInRun.supportEdgels[i];
                    if (it.position.x > u1) {
                        u1 = it.position.x;
                        segmentInRun.start = it;
                    }
                    if (it.position.x < u2) {
                        u2 = it.position.x;
                        segmentInRun.end = it;
                    }
                }
            }
            
            // swap startpoint and endpoint according to orientation of edge
            ARRVec temp;
            temp.x = segmentInRun.end.position.x - segmentInRun.start.position.x;
            temp.y = segmentInRun.end.position.y - segmentInRun.start.position.y;
            if (arrVecInnerProduct(&temp, orientation) < 0.0f) {
                ARREdge tmp;
                SWAP(segmentInRun.start, segmentInRun.end, tmp);
                
                // 因为交换过 start end，所以重新求下向量
                temp.x = segmentInRun.end.position.x - segmentInRun.start.position.x;
                temp.y = segmentInRun.end.position.y - segmentInRun.start.position.y;
            }
            
            arrVecNormalize(&temp);
            segmentInRun.slope = temp;
            
            arrVecFree(slope);
            arrVecFree(orientation);
            
            // 加入满足的 segmentInRun，删除用过的 edge
            
            segments_static[segments_num++] = segmentInRun;
            
            //TODO: 这里该快点？...随即存取且快速删除...貌似不能？
            for (i=0; i<segmentInRun.num; i++) {
                for (j=0; j<edges_num; j++) {
                    ARREdge it = edges[j];
                    if (it.position.x == segmentInRun.supportEdgels[i].position.x
                        && it.position.y == segmentInRun.supportEdgels[i].position.y) {
                        
//						edgels.erase( it );
//						break;
                        
                        //edges[j] = edges[--edges_num];  // 每次删了都从新开始遍历edges的，所以没事     // 错！这里不是删除这么简单，还要删后保证有序
                        memmove(edges + j, edges + j + 1, (edges_num - (j+1))*sizeof(ARREdge)); // void *memmove(void *dest, const void *source, size_t count)
                        edges_num--;
                        
                        break;
                    }
                }
            }
        }
        
    } while( segmentInRun.num >= ARR_EDGES_ONLINE && edges_num >= ARR_EDGES_ONLINE );   // 一个 segment 至少包含 5个 边缘点
	

    *segments = segments_static;
    *num = segments_num;
    return 0;
}