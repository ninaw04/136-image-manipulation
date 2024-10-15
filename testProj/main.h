//
//  main.h
//  testProj
//
//  Created by Nina Wang on 10/8/24.
//

#ifndef main_h
#define main_h

Matrix scale(Matrix m, int scale);
Image imageBlackWhite(Image originalImg, double threshold);
Image imageExpand(Image originalImg);
Image imageShrink(Image originalImg);
Image imageNoise(Image originalImg, float percentage);

Matrix componentLabeling(Matrix cleanImage, int eightNeighbor);
Image componentColoring(Image originalImg, Matrix components, int threshold);

Matrix smoothing_filter(Matrix image, Matrix filter);
Matrix median_filter(Matrix image, Matrix filter);
Matrix gaussian(Matrix img, int size, double sigma);

void edgeDetection(char *inputFileName, char *sobelFileName, char *cannyFileName);
Image sobel(Image image);
Image canny(Image image);


#endif /* main_h */
