//
//  arrMarkerGetTrans.c
//  ARReadingHD
//
//  Created by tclh123 on 13-2-26.
//  Copyright (c) 2013年 tclh123. All rights reserved.
//

#include <stdio.h>
#include <math.h>

#include "ar.h"

//#define DP(p)       printf(""#p" = (%f,%f)\n", p.x, p.y);

void arrMarkerGetTrans(ARRMarker *marker, float scale, float bufferWidth, float bufferHeight, float focalX, float focalY, float matrix[4][4], float matrixGL[16]) {
    int i;
    
    float				homography[3][3];
    
    ARRVec corners[4];

    corners[0] = marker->c1;
    corners[1] = marker->c2;
    corners[2] = marker->c3;
    corners[3] = marker->c4;
    
//    corners[0].x = 277.999969; corners[0].y = 115.756462;
//    corners[1].x = 285.539673; corners[1].y = 255.670319;
//    corners[2].x = 140.471878; corners[2].y = 261.435150;
//    corners[3].x = 137.675323; corners[3].y = 119.435875;

    for (i=0; i<4; i++) {
        corners[i].x -= bufferWidth/2;
        corners[i].x /= focalX;
        corners[i].y -= bufferHeight/2;
        corners[i].y /= focalY;
    }
    
//    DP(corners[0]) DP(corners[1]) DP(corners[2]) DP(corners[3])
    
    // 1. 求单应性矩阵
	float h[8];
	float uv[2][4];     // 4个顶点
	
	for (i = 0; i < 4; i++) {
		uv[0][i] = (corners + i)->x;
		uv[1][i] = (corners + i)->y;
	}
	
	h[6] = uv[0][0];
	h[7] = uv[1][0];
	
	float param_u_4 = uv[0][0] - uv[0][1] - uv[0][3] + uv[0][2];
	float param_v_4 = uv[1][0] - uv[1][1] - uv[1][3] + uv[1][2];
	
	h[5] = param_u_4 * (uv[1][1] - uv[1][2]) - param_v_4 * (uv[0][1] - uv[0][2]);
	h[5] = h[5] / ((uv[0][3] - uv[0][2]) * (uv[1][1] - uv[1][2]) - (uv[0][1] - uv[0][2]) * (uv[1][3] - uv[1][2]));
	
	h[2] = (param_u_4 - (uv[0][3] - uv[0][2]) * h[5]) / (uv[0][1] - uv[0][2]);
	
	h[0] = uv[0][1] * h[2] - uv[0][0] + uv[0][1];
	h[1] = uv[1][1] * h[2] - uv[1][0] + uv[1][1];
	
	h[3] = uv[0][3] * h[5] - uv[0][0] + uv[0][3];
	h[4] = uv[1][3] * h[5] - uv[1][0] + uv[1][3];
	
	homography[0][0] = h[0];
	homography[1][0] = h[1];
	homography[2][0] = h[2];
	
	homography[0][1] = h[3];
	homography[1][1] = h[4];
	homography[2][1] = h[5];
	
	homography[0][2] = h[6];
	homography[1][2] = h[7];
	homography[2][2] = 1;
    
	float e1_length = homography[0][0] * homography[0][0] + homography[1][0] * homography[1][0] + homography[2][0] * homography[2][0];
	float e2_length = homography[0][1] * homography[0][1] + homography[1][1] * homography[1][1] + homography[2][1] * homography[2][1];
	e1_length = sqrtf(e1_length);   // sqrt float
	e2_length = sqrtf(e2_length);
	float length = (e1_length + e2_length) * 0.5;
	
    // 2. 求变换矩阵
	matrix[0][0] = homography[0][0] / e1_length;    // 单位化
	matrix[1][0] = homography[1][0] / e1_length;
	matrix[2][0] = homography[2][0] / e1_length;
	matrix[3][0] = 0;
	
	matrix[0][1] = homography[0][1] / e2_length;
	matrix[1][1] = homography[1][1] / e2_length;
	matrix[2][1] = homography[2][1] / e2_length;
	matrix[3][1] = 0;
	
    // 怎么算出来的??
	matrix[0][2] = matrix[1][0] * matrix[2][1] - matrix[2][0] * matrix[1][1];
	matrix[1][2] = matrix[2][0] * matrix[0][1] - matrix[0][0] * matrix[2][1];
	matrix[2][2] = matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1];
	matrix[3][2] = 0;
	
    // 平移分量 T
	matrix[0][3] = homography[0][2] / length * scale;
	matrix[1][3] = homography[1][2] / length * scale;
	matrix[2][3] = homography[2][2] / length * scale;
	matrix[3][3] = 1;
	
	matrixGL[ 0] = matrix[0][0];	matrixGL[ 4] = matrix[0][1];	matrixGL[ 8] = matrix[0][2];	matrixGL[12] = matrix[0][3];
	matrixGL[ 1] = matrix[1][0];	matrixGL[ 5] = matrix[1][1];	matrixGL[ 9] = matrix[1][2];	matrixGL[13] = matrix[1][3];
	matrixGL[ 2] = matrix[2][0];	matrixGL[ 6] = matrix[2][1];	matrixGL[10] = matrix[2][2];	matrixGL[14] = matrix[2][3];
	matrixGL[ 3] = matrix[3][0];	matrixGL[ 7] = matrix[3][1];	matrixGL[11] = matrix[3][2];	matrixGL[15] = matrix[3][3];
    
    
//    printf("Matrix\n");
//	printf("%+7.2f %+7.2f %+7.2f %+7.2f;\n", matrixGL[ 0], matrixGL[ 4], matrixGL[ 8], matrixGL[12]);
//	printf("%+7.2f %+7.2f %+7.2f %+7.2f;\n", matrixGL[ 1], matrixGL[ 5], matrixGL[ 9], matrixGL[13]);
//	printf("%+7.2f %+7.2f %+7.2f %+7.2f;\n", matrixGL[ 2], matrixGL[ 6], matrixGL[10], matrixGL[14]);
//	printf("%+7.2f %+7.2f %+7.2f %+7.2f;\n\n", matrixGL[ 3], matrixGL[ 7], matrixGL[11], matrixGL[15]);
    
    ///////////////////////
    // rotate Matrix For OpenGL
    ///////////////////////

    float tempMatrix[4][4];
	
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			tempMatrix[i][j] = matrix[i][j];
	
	matrix[0][0] = - tempMatrix[1][0];	matrix[0][1] = - tempMatrix[1][1];	matrix[0][2] = - tempMatrix[1][2];	matrix[0][3] = - tempMatrix[1][3];
	matrix[1][0] = - tempMatrix[0][0];	matrix[1][1] = - tempMatrix[0][1];	matrix[1][2] = - tempMatrix[0][2];	matrix[1][3] = - tempMatrix[0][3];
	matrix[2][0] = - tempMatrix[2][0];	matrix[2][1] = - tempMatrix[2][1];	matrix[2][2] = - tempMatrix[2][2];	matrix[2][3] = - tempMatrix[2][3];
	matrix[3][0] =   tempMatrix[3][0];	matrix[3][1] =   tempMatrix[3][1];	matrix[3][2] =   tempMatrix[3][2];	matrix[3][3] =   tempMatrix[3][3];
	
	matrixGL[ 0] = matrix[0][0];	matrixGL[ 4] = matrix[0][1];	matrixGL[ 8] = matrix[0][2];	matrixGL[12] = matrix[0][3];
	matrixGL[ 1] = matrix[1][0];	matrixGL[ 5] = matrix[1][1];	matrixGL[ 9] = matrix[1][2];	matrixGL[13] = matrix[1][3];
	matrixGL[ 2] = matrix[2][0];	matrixGL[ 6] = matrix[2][1];	matrixGL[10] = matrix[2][2];	matrixGL[14] = matrix[2][3];
	matrixGL[ 3] = matrix[3][0];	matrixGL[ 7] = matrix[3][1];	matrixGL[11] = matrix[3][2];	matrixGL[15] = matrix[3][3];
    
    printf("final Matrix\n");
	printf("%+7.2f %+7.2f %+7.2f %+7.2f;\n", matrixGL[ 0], matrixGL[ 4], matrixGL[ 8], matrixGL[12]);
	printf("%+7.2f %+7.2f %+7.2f %+7.2f;\n", matrixGL[ 1], matrixGL[ 5], matrixGL[ 9], matrixGL[13]);
	printf("%+7.2f %+7.2f %+7.2f %+7.2f;\n", matrixGL[ 2], matrixGL[ 6], matrixGL[10], matrixGL[14]);
	printf("%+7.2f %+7.2f %+7.2f %+7.2f;\n\n", matrixGL[ 3], matrixGL[ 7], matrixGL[11], matrixGL[15]);
}