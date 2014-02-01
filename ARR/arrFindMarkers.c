//
//  arrFindMarkers.c
//  ARReadingHD
//
//  Created by tclh123 on 13-2-14.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#include <stdio.h>
#include "ar.h"

////////////////////////////////////////////////////////////////////////
// DEBUGS

void debug_prints(void *p, int n, void (*print_sub)(void *p, int i))
{
    printf("==================================================\n");
    int i;
    for (i=0; i<n; i++) {
        print_sub(p, i);
        printf("\n");
    }
}
void print_vec(ARRVec *vec)
{
    printf("(%.4f,%.4f)", vec->x, vec->y);
}
// edge < (1,2), (3,4) >
void print_edge(void *p, int i)
{
    ARREdge *it = (ARREdge*)p + i;
    printf("edge < ");
    print_vec(&it->position);
    printf(", ");
    print_vec(&it->slope);
    printf(" >");
}
// segment < start=edge < (1,2), (3,4) >, end=edge < (1,2), (3,4) >, slope=(5,6), remove=0, start_corner=0, end_corner=0, supportEdgels[0]=edge < (1,2), (3,4) >, num=0 >
void print_segment(void *p, int i)
{
    ARRSegment *it = (ARRSegment*)p + i;
    printf("segment < start=");
    print_edge(&it->start, 0);
    printf(", end=");
    print_edge(&it->end, 0);
    printf(", slope=");
    print_vec(&it->slope);
    printf(", remove=");
    printf("%d", it->remove);
    printf(", start_corner=");
    printf("%d", it->start_corner);
    printf(", end_corner=");
    printf("%d", it->end_corner);
    printf(", supportEdgels[0]=");
    print_edge(it->supportEdgels, 0);
    printf(", num=");
    printf("%d", it->num);
    printf(" >");
}



static ARRSegment mergedSegments[ARR_MERGED_SEGMENTS_MAX];
static int mergedSegments_num;

static ARRMarker markers_static[ARR_MARKERS_MAX];
static int markers_num;

int arrFindMarkers(ARREdgeDetector *detector,
                   ARRMarker **markers, int *num) //output markers, change num
{
    mergedSegments_num = 0;
    
    int x, y;
    int i;
    
    // 遍历每个 Region
    for (y = 2; y < detector->image->height - 3; y += ARR_REGION_SIZE) {
        for (x = 2; x < detector->image->width - 3; x += ARR_REGION_SIZE) {
            
            // 1、Get edges
            ARREdge *edges = NULL;
            int edges_num = 0;
            if (arrFindEdgesInRegion(detector, x, y,
                                     MIN(detector->image->width - x - 3, ARR_REGION_SIZE),
                                     MIN(detector->image->height - y - 3, ARR_REGION_SIZE),
                                     &edges, &edges_num) < 0)
            {
                return -1;
            }
            ////////// debug_prints
            //debug_prints(edges, edges_num, print_edge);
            
            // 2、若 边缘点 超过5个，找组成的 线段
            ARRSegment *segments = NULL;
            int segments_num = 0;
            if (edges_num > 5) {
                arrFindSegments(detector, edges, edges_num, &segments, &segments_num);  // =0
            }  // 至此 edges 变成残留的
            
            ////////// debug_prints
            //debug_prints(segments, segments_num, print_segment);
            
#if DEBUG_ENABLE
            // debug functions: 画线段
            if (detector->drawLineSegments) {
                for (i = 0; i < segments_num; i++) {
                    //drawArrow
                }
            }
#endif
            // 3、若 线段 超过1条，则尝试合并线段
            if (segments_num > 1) {
                arrMergeSegments(detector, 50, segments, &segments_num);   // 合并线段，最大迭代50次
            }
            
#if DEBUG_ENABLE
            // debug functions: 画部分合并的线段
            if (detector->drawPartialMergedLineSegments) {
                for (i = 0; i < segments_num; i++) {
                    //drawArrow
                }
            }
#endif

            // Add segments to mergedSegments
            for (i=0; i<segments_num; i++) {
                mergedSegments[mergedSegments_num++] = segments[i];
            }
        }
    }
    
    // 4、在整个图像合并线段一次（因为之前都是Region内合并过）
    arrMergeSegments(detector, 50, mergedSegments, &mergedSegments_num);
    
#if DEBUG_ENABLE
    // debug functions: 画 整图合并后的线段
    if (detector->drawMergedLineSegments) {
        for (i=0; i<mergedSegments_num; i++) {
            // drawArrow
        }
    }
#endif
    
    // 5、extend segments
    
    arrExtendSegments(detector, mergedSegments, mergedSegments_num);
    
#if DEBUG_ENABLE
    if (detector->drawExtendedLineSegments) {
        for (i=0; i<mergedSegments_num; i++) {
            //drawArrow
        }
    }
#endif
    
    // 6、detect corners (Find Segments With Corners)
    ARRSegment *linesWithCorners = NULL;
    int linesWithCorners_num = 0;
    arrFindLinesWithCorners(detector, mergedSegments, mergedSegments_num, &linesWithCorners, &linesWithCorners_num);
    
#if DEBUG_ENABLE
    // debug functions: 画 带角点的线段
    if (detector->drawCorners) {
        for (i=0; i<linesWithCorners_num; i++) {
            ARRSegment it = linesWithCorners[i];
            if (it.start_corner) {
                // drawPoint
            }
            if (it.end_corner) {
                // drawPoint
            }
            
            // drawArrow
        }
    }
#endif
    
    // 7、detect markers（Find Chain Of Lines）
    markers_num = 0;
    
    while (linesWithCorners_num) {
        // 随机选 线段，并尝试做一个 chain
        
        ARRSegment chainSegment = linesWithCorners[0];
        linesWithCorners[0] = linesWithCorners[--linesWithCorners_num];
        
        // 		std::vector<LineSegment> chain;
        ARRSegment chain[ARR_EACH_MARKER_SEGMENT_MAX];
        int chain_num = 0;
        int chain_length = 1;   // TODO: 改下，这 num 跟 length 太搓了，因为 chainSegment 后面才进 chain 的
        
        // 检查 以 chainSegment 为首 的 chain
        arrFindChainOfLines(detector,
                            &chainSegment, TRUE,
                            linesWithCorners, &linesWithCorners_num,    // linesWithCorners 找出 符合的 segment，删去
                            chain, &chain_num, &chain_length);          // 再 加入 chain 中，直到加入3个。
        
        chain[chain_num++] = chainSegment;  // 至此，chain_num == chain_length

        // 继续 以chainSegment 不为首 找
        if ( chain_length < 4) {
            arrFindChainOfLines(detector,
                                &chainSegment, FALSE,
                                linesWithCorners, &linesWithCorners_num,
                                chain, &chain_num, &chain_length);
        }
        
        if (chain_length > 2) {
            ARRMarker marker;
            
            // marker.chain = chain;
            arrMarkerCopyChain(&marker, chain, chain_num);
            
            arrMarkerReconstruct(&marker);  //Reconstruct?
            
            markers_static[markers_num++] = marker;
        }
    }
    
#if DEBUG_ENABLE
    // debug functions: 画 带角点的线段
    if (detector->drawMarkers) {
        for (i=0; i<markers_num; i++) {
			//drawLine( markers[i].c1.x, markers[i].c1.y, markers[i].c2.x, markers[i].c2.y, 255, 0, 0, THICKNESS);
			//drawLine( markers[i].c2.x, markers[i].c2.y, markers[i].c3.x, markers[i].c3.y, 255, 0, 0, THICKNESS);
			//drawLine( markers[i].c3.x, markers[i].c3.y, markers[i].c4.x, markers[i].c4.y, 255, 0, 0, THICKNESS);
			//drawLine( markers[i].c4.x, markers[i].c4.y, markers[i].c1.x, markers[i].c1.y, 255, 0, 0, THICKNESS);
        }
    }
#endif
    
    *markers = markers_static;
    *num = markers_num;
    return 0;
}