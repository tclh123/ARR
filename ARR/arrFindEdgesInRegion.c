//
//  arrFindEdgesInRegion.c
//  ARReadingHD
//
//  Created by tclh123 on 13-2-14.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#include <stdio.h>

#include "ar.h"

static ARREdge edges_static[ARR_EACH_REGION_EDGES_MAX];    //(a container)
static int edges_num;
static int findEdge(ARREdgeDetector *detecotr,
                    int x, int y,
                    const int left, const int top,
                    int *prev1, int *prev2,
                    ARRByte *offset, const int pitch,
                    Bool isHorizontal);


// 在 Region 中 找边缘像素点
int arrFindEdgesInRegion(ARREdgeDetector *detecotr,
                         const int left, const int top, const int width, const int height,
                         ARREdge **edges, int *num) // output edges, change num
{
    edges_num = 0;
    int prev1, prev2;   // 像素差，维护两个历史 ???
    int x, y;
    
#if DEBUG_ENABLE
    // debug functions: 画 此区块(Region) 的 SectorGrids
    if (detecotr->drawSectorGrids) {
        //画横线
        for (y = top; y < top + height; y += ARR_RASTER_SIZE) {
            //drawLine
        }
        //画纵线
        for (x = left; x < left + width; x += ARR_RASTER_SIZE) {
            //drawLine
        }
    }
    // debug functions: 画 此区块(Region) 的 Sectors（4条边界线）
    if (detecotr->drawSectors) {
        //drawLine
    }
#endif
    
    for (y = 0; y < height; y += ARR_RASTER_SIZE) { // 遍历横向扫描线
        ARRByte *offset = detecotr->image->data + (left + (y+top) * detecotr->image->width) * 3;    //指向扫描线左端点byte
        prev1 = prev2 = 0;
        const int pitch = 3;    //大概就是间隔的意思，每个横向扫描线，3个channel为一个pixel
        
        for (x = 0; x < width; x++, offset += pitch) {
            findEdge(detecotr, x, y, left, top, &prev1, &prev2, offset, pitch, TRUE);
        }
    }

#if DEBUG_ENABLE    
    // debug function: 画横边缘像素点
    if( detecotr->drawEdges ) {
        for( i=0; i<edges_num; i++ ) {
            //drawPoint
        }
    }
    int debugNumOfHorizontalEdges = edges_num;
#endif
    
    for( x=0; x<width; x+=ARR_RASTER_SIZE ) {
        ARRByte *offset = detecotr->image->data + (left + x + top * detecotr->image->width ) * 3;
        const int pitch = 3 * detecotr->image->width;
        prev1 = prev2 = 0;
        
        for( int y=0; y<height; y++, offset += pitch) {
            findEdge(detecotr, x, y, left, top, &prev1, &prev2, offset, pitch, FALSE);
        }
    }

#if DEBUG_ENABLE
    // debug function: 画纵边缘像素点
    if( detecotr->drawEdges ) {
        for( i=debugNumOfHorizontalEdges; i<edges_num; i++ ) {
            //drawPoint
        }
    }
#endif
    
    *edges = edges_static;
    *num = edges_num;
    
    return 0;
}

static int findEdge(ARREdgeDetector *detecotr,
                    int x, int y,
                    const int left, const int top,
                    int *prev1, int *prev2,
                    ARRByte *offset, const int pitch,
                    Bool isHorizontal)
{
    int current = arrEdgeKernel(offset, pitch); // arrEdgeKernel 能反应像素点与周围的对比度？类似
    
    // check channels（看是不是 RGB 都大于阈值）
    if (current > ARR_THRESHOLD &&
        arrEdgeKernel(offset+1, pitch) > ARR_THRESHOLD &&
        arrEdgeKernel(offset+2, pitch) > ARR_THRESHOLD)
    {
        // edge!
    } else {
        current = 0;
    }
    
    // find local maximum
    if (*prev1 > 0 && *prev1 > *prev2 && *prev1 > current) {
        float nx = left + x, ny = top + y;  // 这里是整数（像素）
        if (isHorizontal) {
            nx -= 1;
        }
        else {
            ny -= 1;
        }
        ARREdge *edge = arrEdgeAlloc(nx, ny);
        edge->slope = arrEdgeGradientIntensity(detecotr, nx, ny);  // 用 边缘点 周围的梯度强度 计算其斜率
        
        if (edges_num + 1 > ARR_EACH_REGION_EDGES_MAX) {
            arrEdgeFree(edge);
            return -1;
        }
        edges_static[edges_num++] = *edge;   // add edge to edges_static(a container).
        arrEdgeFree(edge);
    }
    
    *prev2 = *prev1;
    *prev1 = current;
    
    return 0;
}