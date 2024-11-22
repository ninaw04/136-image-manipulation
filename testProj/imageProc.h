//
//  imageProc.h
//  testProj
//
//  Created by Nina Wang on 11/18/24.
//

#ifndef imageProc_h
#define imageProc_h

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "hough.h"
#include "segmentTexture.h"

Image performHoughTransform(Image image);
Matrix convolve(Matrix m1, Matrix m2);

Matrix scale(Matrix m, int scale);
Image imageBlackWhite(Image originalImg, double threshold);
Image imageExpand(Image originalImg);
Image imageShrink(Image originalImg);
Image imageNoise(Image originalImg, float percentage);

Matrix componentLabeling(Matrix cleanImage, int eightNeighbor);
Image componentColoring(Image originalImg, Matrix components, int threshold);

Matrix averageTwoMatrices(Matrix m1, Matrix m2);
Matrix smoothing_filter(Matrix image, Matrix filter);
Matrix median_filter(Matrix image, Matrix filter);
Matrix gaussian(Matrix img, int size, double sigma);

void edgeDetection(char *inputFileName, char *sobelFileName, char *cannyFileName);
Image sobel(Image image);
Image canny(Image image, int gauss);

#endif /* imageProc_h */
