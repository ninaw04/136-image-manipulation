//
//  main.c
//  CS136
//
//  Created by nha2 on 8/27/24.
//  Edited by Nina Wang on 10/14/24
// Test and demo program for netpbm. Reads a sample image and creates several output images.



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "imageProc.h"
#include "hough.h"
#include "segmentTexture.h"

#define MAX_RADIUS 150


int main(int argc, const char * argv[]) {
    
    Image first = readImage("/Users/ninawang/Documents/School/CS136/testProj/netpbm/textures/15.pgm");
    Image firstMat = segmentTexture(first, 6);
    writeImage(firstMat, "/Users/ninawang/Documents/School/CS136/testProj/netpbm/coloredTexture15.ppm");
//    Image circleOutput = performHoughTransform(circles);
//    writeImage(circleOutput, "/Users/ninawang/Documents/School/CS136/testProj/netpbm/hough_circles2.ppm");

    printf("Program ends ... ");
    return 0;
}
