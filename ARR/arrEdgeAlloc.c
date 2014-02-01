//
//  arrEdgeAlloc.c
//  ARReadingHD
//
//  Created by tclh123 on 13-2-14.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

ARREdge* arrEdgeAlloc(float x, float y) {
    ARREdge *edge = (ARREdge*)malloc(sizeof(ARREdge));
    if(edge == NULL) return NULL;
    
    edge->position.x = x;
    edge->position.y = y;
    //edge->slope

    return edge;
}