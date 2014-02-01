//
//  matrix.h
//  ARReadingHD
//
//  Created by tclh123 on 13-2-14.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#ifndef ARReadingHD_matrix_h
#define ARReadingHD_matrix_h

#include "type.h"
#include "config.h"

/* vector 
    向量
 */
typedef struct {
    float x, y;
} ARRVec;

ARRVec* arrVecAlloc(float x, float y);
ARRVec* arrVecAllocMinus(const ARRVec* a, const ARRVec* b);
int arrVecFree(ARRVec*);
int arrVecAdd(ARRVec *vec, const ARRVec *add);
float arrVecLength(ARRVec *v);
int arrVecNormalize(ARRVec*);
float arrVecInnerProduct(const ARRVec *vec1, const ARRVec *vec2);

/* Edge(edgel?) 
    边缘点，有方向
 */
typedef struct {
    ARRVec position;
    ARRVec slope;   //斜率
} ARREdge;

ARREdge* arrEdgeAlloc(float x, float y);
int arrEdgeFree(ARREdge*);
Bool arrEdgeIsCompatible(ARREdge *edge1, ARREdge *edge2);

/* linesegment
    线段
 */
typedef struct {
    ARREdge start, end;
    ARRVec slope;
    Bool remove, start_corner, end_corner;
	ARREdge supportEdgels[ARR_EACH_SEGMENT_EDGES_MAX];     // 当 Alloc Segment?
    int num;    // supportEdgels' num
} ARRSegment;

// TODO:
//ARRSegment* arrSegmentAlloc(); //LineSegment() : remove(false), start_corner(false), end_corner(false) {}
void arrSegmentInit(ARRSegment*);
int arrSegmentFree(ARRSegment*);

int arrSegmentAddSupport(ARRSegment *seg, ARREdge *edge);

Bool arrSegmentIsInclude(ARRSegment *seg, ARREdge *edge);
Bool arrSegmentIsCompatible(ARRSegment *seg, ARRSegment *seg2);
ARRVec arrSegmentIntersect(ARRSegment *seg, ARRSegment *seg2);
Bool arrSegmentIsEquel(ARRSegment *seg, ARRSegment *seg2);



#endif
