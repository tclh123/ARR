//
//  arrEdgeFree.c
//  ARReadingHD
//
//  Created by tclh123 on 13-2-16.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

int arrEdgeFree(ARREdge* edge)
{
    free(edge);               
//    arrVecFree(&edge->position);
//    arrVecFree(&edge->slope);       //??
    
    return 0;
}