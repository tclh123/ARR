//
//  arrMarkerReconstruct.c
//  ARReadingHD
//
//  Created by tclh123 on 13-2-14.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#include <stdio.h>

#include "ar.h"

// 估计出 4 个点
void arrMarkerReconstruct(ARRMarker* marker) {
	marker->c1 = arrSegmentIntersect(&marker->chain[0], &marker->chain[1]);      //intersection:交叉
	marker->c2 = arrSegmentIntersect(&marker->chain[1], &marker->chain[2]);
	
	if( marker->num == 4 ) {   //链里有4条边
		marker->c3 = arrSegmentIntersect(&marker->chain[2], &marker->chain[3]);
		marker->c4 = arrSegmentIntersect(&marker->chain[3], &marker->chain[0]);
	} else {        // 没有4条边(只有3条边)，估计两个点
		marker->c3 = marker->chain[2].end.position;
		marker->c4 = marker->chain[0].start.position;
	}
}