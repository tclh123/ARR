//
//  ar.h
//  ARReadingHD
//
//  Created by tclh123 on 13-2-14.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#ifndef ARReadingHD_ar_h
#define ARReadingHD_ar_h

#include "common.h"
#include "type.h"
#include "config.h"
#include "matrix.h"

/// 除了 ARRImage、ARREdgeDetector 结构体，其他结构体中都不含成员指针

/// 函数风格，所谓 output：指带Alloc功能的，change：只是改变值而已


/* Image Buffer */
typedef struct {
    int width;
    int height;
    ARRByte *data;  // 在实际运行中，指向每个视频帧
} ARRImage;

ARRImage* arrImageAlloc(int width, int height);
// TODO: free或setData？
ARRByte arrImageGetPixel(ARRImage *image, int x, int y, int channel);
float arrImageGetPixelColor(ARRImage *image, int x, int y, int channel);

/* ARR Marker */
typedef struct {
    ARRVec c1, c2, c3, c4; // corner
    ARRSegment chain[ARR_EACH_MARKER_SEGMENT_MAX]; //4条边
    int num;    // size of chain.
} ARRMarker;
void arrMarkerReconstruct(ARRMarker *marker);
void arrMarkerCopyChain(ARRMarker *marker, ARRSegment *chain, int num);
void arrMarkerGetTrans(ARRMarker *marker, float scale,float bufferWidth, float bufferHeight, float focalX, float focalY, float matrix[4][4], float matrixGL[16]);

/* Edeg Detector */
typedef struct {
    ARRImage *image;
    
    // debug line settings
    Bool drawLineSegments,
        drawPartialMergedLineSegments,
        drawMergedLineSegments,
        drawExtendedLineSegments,
        drawMarkers,
        drawSectors,
        drawSectorGrids,
        drawEdges,
        drawCorners;
} ARREdgeDetector;

// TODO: 把画线的抽离出来
// TODO: Edeg Detector's functions
void arrSetDebugLine(ARREdgeDetector *detector,
                     Bool drawLineSegments,
                     Bool drawPartialMergedLineSegments,
                     Bool drawMergedLineSegments,
                     Bool drawExtendedLineSegments,
                     Bool drawCorners,
                     Bool drawMarkers,
                     Bool drawSectors,
                     Bool drawSectorGrids,
                     Bool drawEdges);

// Edge Kernel?
int arrEdgeKernel(ARRByte *offset, const int pitch);
int arrEdgeKernelX(ARREdgeDetector *detector, int x, int y);
int arrEdgeKernelY(ARREdgeDetector *detector, int x, int y);

int arrFindEdgesInRegion(ARREdgeDetector *detecotr, const int left, const int top, const int width, const int height,
                         ARREdge **edges, int *num);  //output edges, change num.
ARRVec arrEdgeGradientIntensity(ARREdgeDetector *detector, int x, int y);

int arrFindMarkers(ARREdgeDetector *detector,
                   ARRMarker **markers, int *num); //output markers, change num

//  Segments
// 找线段
int arrFindSegments(ARREdgeDetector *detector, ARREdge *edges, int edges_num,
                       ARRSegment **segments, int *num);    //output segment, change num
// 合并（连接）线段
int arrMergeSegments(ARREdgeDetector *detector, int max_iterations,
                     ARRSegment *segments, int *num);  // change segments, num  // 调用 arrExtendLine
Bool arrExtendLine(ARREdgeDetector *detector, ARRVec *startpoint, const ARRVec *slope, const ARRVec * gradient,
                          ARRVec *endpoint, const int maxlength );    // output endpoint
// 扩展（延伸）线段
void arrExtendSegments(ARREdgeDetector *detector, ARRSegment *segments, int num);    // change segments //调用 arrExtendLine

// 找带角点的线段
int arrFindLinesWithCorners(ARREdgeDetector *detector, ARRSegment *segments, int num,
                            ARRSegment **lineWithCorners, int *line_num);  // output lineWithCorners, change line_num
// 找链（矩形4条边）
int arrFindChainOfLines(ARREdgeDetector *detector,
                        ARRSegment *startSegment,
                        Bool atStartPoint,
                        ARRSegment *segments,
                        int *segments_num,
                        ARRSegment *chain,
                        int *chain_num,
                        int *length);     // change segments, chain, chain_num, length  // 其实chain_num == length ?


//void arrSetImageBuffer(ARREdgeDetector *detector, ARRImage *image);

 // draw functions

/*
 TODO:
 
 void setBuffer(Buffer* buffer);
 
 void drawLine( int x1, int y1, int x2, int y2, int r, int g, int b, int t);
 void drawPoint( int x, int y, int r, int g, int b, int t);
 void drawArrow( int x1, int y1, int x2, int y2, float xn, float yn, int r, int g, int b, int t);
 */


////////////////////////////////////////////////////////////////////////
// DEBUGS

void debug_prints(void *p, int n, void (*print_sub)(void *p, int i));
void print_vec(ARRVec *vec);
// edge < (1,2), (3,4) >
void print_edge(void *p, int i);
// segment < start=edge < (1,2), (3,4) >, end=edge < (1,2), (3,4) >, slope=(5,6), remove=0, start_corner=0, end_corner=0, supportEdgels[0]=edge < (1,2), (3,4) >, num=0 >
void print_segment(void *p, int i);

#endif
