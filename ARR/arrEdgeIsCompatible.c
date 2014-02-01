//
//  arrEdgeIsCompatible.c
//  ARReadingHD
//
//  Created by tclh123 on 13-2-14.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#include <stdio.h>

#include "matrix.h"

Bool arrEdgeIsCompatible(ARREdge *edge1, ARREdge *edge2) {
    
	return arrVecInnerProduct(&edge1->slope, &edge2->slope) > 0.38f; //cosf( 67.5f / 2 pi ) ;
}