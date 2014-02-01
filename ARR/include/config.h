//
//  config.h
//  ARReadingHD
//
//  Created by tclh123 on 13-2-14.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#ifndef ARReadingHD_config_h
#define ARReadingHD_config_h

#define DEBUG_ENABLE 0

#define ARR_REGION_SIZE 40
#define ARR_RASTER_SIZE 5
#define ARR_THRESHOLD (16*16)   // 边缘点 与周围的差异 意义是什么?
#define ARR_EDGES_ONLINE 5
#define ARR_WHITE_TRESHHOLD 10

/*
#define THRESHOLD (16*16)
#define REGIONSIZE 40
#define EDGELSONLINE 5
//raster: 光栅
#define RASTERSIZE 5
#define WHITETRESHHOLD 10
#define THICKNESS 2
*/

#define ARR_EACH_SEGMENT_EDGES_MAX 1000
#define ARR_EACH_REGION_EDGES_MAX 1000
#define ARR_EACH_REGION_SEGMENT_MAX 1000
#define ARR_MERGED_SEGMENTS_MAX 5000   // 总的 merged_segments
#define ARR_LINE_WITH_CORNERS_MAX 1000
#define ARR_EACH_MARKER_SEGMENT_MAX 5   //4? ARR_MARKER_CHAIN_MAX
#define ARR_MARKERS_MAX 1000

#endif
