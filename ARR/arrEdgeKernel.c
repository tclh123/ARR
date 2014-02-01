//
//  arrEdgeKernel.c
//  ARReadingHD
//
//  Created by tclh123 on 13-2-16.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#include <stdio.h>
//#include <math.h>

#include "ar.h"


int arrEdgeKernel(ARRByte *offset, const int pitch) {
    
    int ver = -3 * offset[ -2*pitch ];
	ver += -5 * offset[ -pitch ];
	ver += 5 * offset[ pitch ];
	ver += 3 * offset[ 2*pitch ];       //TODO: BAD ACCESS!!!!!!????
	
	return abs( ver );  //c99?
}

int arrEdgeKernelX(ARREdgeDetector *detector, int x, int y) {
	int ver = -3 * arrImageGetPixelColor(detector->image, x, y-2, 0);
	ver += -5 * arrImageGetPixelColor(detector->image, x, y-1, 0);
	ver += 5 * arrImageGetPixelColor(detector->image, x, y+1, 0);
	ver += 3 * arrImageGetPixelColor(detector->image, x, y+2, 0);
	
	return abs( ver );
}

int arrEdgeKernelY(ARREdgeDetector *detector, int x, int y) {
    int ver = -3 * arrImageGetPixelColor(detector->image, x-2, y, 0);
	ver += -5 * arrImageGetPixelColor(detector->image, x-1, y, 0);
	ver += 5 * arrImageGetPixelColor(detector->image, x+1, y, 0);;
	ver += 3 * arrImageGetPixelColor(detector->image, x+2, y, 0);;
	
	return abs( ver );
}