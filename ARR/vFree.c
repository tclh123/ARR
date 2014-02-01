//
//  vFree.c
//  ARReadingHD
//
//  Created by tclh123 on 13-2-14.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

int arrVecFree(ARRVec *v)
{
    free(v);
    
    return 0;
}