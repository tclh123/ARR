//
//  arrFindLinesWithCorners.c
//  ARReadingHD
//
//  Created by tclh123 on 13-2-17.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#include <stdio.h>

#include "ar.h"

static ARRSegment lineWithCorners_static[ARR_LINE_WITH_CORNERS_MAX];    // 空间换时间，其他地方（费时删除的）看看要不要改。TODO:
static int lineWithCorners_num;

//检测角点
int arrFindLinesWithCorners(ARREdgeDetector *detector, ARRSegment *segments, int num,
                            ARRSegment **lineWithCorners, int *line_num)  // output lineWithCorners, change line_num
{
    lineWithCorners_num = 0;
    
    int i;
    
    // detect corners. We expect black markers on a white background...
    for (i=0; i<num; i++) {
        ARRSegment it = segments[i];    // 不改变 segments 了
        
        int dx = it.slope.x * 4.0f; //const int dx = static_cast<int>(linesegments[i].slope.x * 4.0f);
        int dy = it.slope.y * 4.0f;
        
        // check startpoint
        int x = it.start.position.x - dx;
        int y = it.start.position.y - dy;
        if (arrImageGetPixel(detector->image, x, y, 0) > ARR_WHITE_TRESHHOLD &&
            arrImageGetPixel(detector->image, x, y, 1) > ARR_WHITE_TRESHHOLD &&
            arrImageGetPixel(detector->image, x, y, 2) > ARR_WHITE_TRESHHOLD) {
            it.start_corner = TRUE;
        }
        
        // check endpoint
        x = it.end.position.x + dx;
        y = it.end.position.y + dy;
        if (arrImageGetPixel(detector->image, x, y, 0) > ARR_WHITE_TRESHHOLD &&
            arrImageGetPixel(detector->image, x, y, 1) > ARR_WHITE_TRESHHOLD &&
            arrImageGetPixel(detector->image, x, y, 2) > ARR_WHITE_TRESHHOLD) {
            it.end_corner = TRUE;
        }
        
        if (it.start_corner || it.end_corner) {
            // add to lineWithCorners_static
            // TODO: 防越界处理，其他的地方也有
            lineWithCorners_static[lineWithCorners_num++] = it;     // 结构体变量直接可以整体赋值，没问题，相当于内存的拷贝
        }
    }
    
    *lineWithCorners = lineWithCorners_static;  //改变的是 指针本身的值（改变地址）
    *line_num = lineWithCorners_num;
    return 0;
}