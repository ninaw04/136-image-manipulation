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

//#include "main.h"
#include "imageProc.h"
#include "hough.h"
#include "segmentTexture.h"

#define MAX_RADIUS 150


int main(int argc, const char * argv[]) {
    
    Image first = readImage("/Users/ninawang/Documents/School/CS136/testProj/netpbm/textures/1.pgm");
    Image firstMat = segmentTexture(first, 1);
    writeImage(firstMat, "/Users/ninawang/Documents/School/CS136/testProj/netpbm/test.ppm");
//    Image circleOutput = performHoughTransform(circles);
//    writeImage(circleOutput, "/Users/ninawang/Documents/School/CS136/testProj/netpbm/hough_circles2.ppm");

    printf("Program ends ... ");
    return 0;
}
//
//Image performHoughTransform(Image image) {
//    // perform canny edge detection
//    Image can = canny(image, 2);
//
//    int maxRadius = MAX_RADIUS;
//    int threshold = 60;
//    
//    // ACCUMULATOR SPACE
//    // vertical, horizontal, radius
//    int ***accumulator = (int ***)malloc(image.height * sizeof(int **));
//    for (int i = 0; i < image.height; i++) {
//        accumulator[i] = (int **)malloc(image.width * sizeof(int *));
//        for (int j = 0; j < image.width; j++) {
//            accumulator[i][j] = (int *)calloc(maxRadius, sizeof(int));
//        }
//    }
//    
//    // MAXIMA, detected center and radius of circle
//    int ***maxima = (int ***)malloc(image.height * sizeof(int **));
//    for (int i = 0; i < image.height; i++) {
//        maxima[i] = (int **)malloc(image.width * sizeof(int *));
//        for (int j = 0; j < image.width; j++) {
//            maxima[i][j] = (int *)calloc(maxRadius, sizeof(int));
//        }
//    }
//
//    printf("hough transform lines\n");
//    houghTransformLines(can, accumulator, maxRadius);
//    
//    printf("hough find maxima\n");
//    findHoughMaxima(accumulator, maxima, maxRadius, image.height, image.width, threshold);
//
////     draw circles on original image
//    for (int y = 0; y < image.height; y++) {
//        for (int x = 0; x < image.width; x++) {
//            for (int r = 1; r < maxRadius; r++) {
//                if (maxima[y][x][r] == 1) {
//                    ellipse(image, y, x, r, r, 0, 0, 0, 255, 0, 0, 255);
//                    break;
//                }
//            }
//        }
//    }
//    
//    // Free accumulator and maxima memory
//    for (int i = 0; i < image.height; i++) {
//        for (int j = 0; j < image.width; j++) {
//            free(accumulator[i][j]);
//            free(maxima[i][j]);
//        }
//        free(accumulator[i]);
//        free(maxima[i]);
//    }
//    free(accumulator);
//    free(maxima);
//    
//    deleteImage(can);
//    
//    printf("yippeee");
//    return image;
//}
//
//void edgeDetection(char *inputFileName, char *sobelFileName, char *cannyFileName) {
//    Image img = readImage(inputFileName);
//    Image sob = sobel(img);
//    Image can = canny(img, 3);
//    deleteImage(img);
//    writeImage(sob, sobelFileName);
//    writeImage(can, cannyFileName);
//}
//
//Matrix scale(Matrix m, int scale) {
//    // scale magnitude to use full range of 0 to 255
//    float min = __FLT_MAX__;
//    float max = __FLT_MIN__;
//    for (int i = 0; i < m.height; i++) {
//        for (int j = 0; j < m.width; j++) {
//            if (m.map[i][j] < min) {
//                min = m.map[i][j];
//            } else if (m.map[i][j] > max) {
//                max = m.map[i][j];
//            }
//        }
//    }
//    
//    for (int i = 0; i < m.height; i++) {
//        for (int j = 0; j < m.width; j++) {
//            float val = m.map[i][j];
//            m.map[i][j] = (val-min) / (max-min) * scale;
//        }
//    }
//    return m;
//}
//
////-------------------------------function_imageBlackWhite-------------------------------------------------
///* function that receives an Image structure and an intensity threshold
// to convert each pixel in the image to either black (intensity = 0)
// or white (intensity = 255). The function should return an Image structure
// containing the result. */
//Image imageBlackWhite(Image originalImg, double threshold) {
//    Image img = createImage(originalImg.height, originalImg.width); // store blackwhite image in here
//    
//    for (int i = 0; i < originalImg.height; i++) {
//        for (int j = 0; j < originalImg.width; j++) {
//            if(originalImg.map[i][j].i > threshold){ // change to white
//                img.map[i][j].i = 255.0;
//            } else { // change to black
//                img.map[i][j].i = 0.0;
//            }
//        }
//    }
//    return img;
//}
//
//
//
////--------------------------------Expand function-----------------------------------------------------------
///* Expand operation
// Changes pixel from 1 to 0 if any neighbors are 0
// */
//Image imageExpand(Image orig) {
//    Image img = orig;
//    Matrix imgM = image2Matrix(orig);
//    // start 1 row lower
//    for (int i = 1; i < imgM.height - 1; i++) {
//        for (int j = 1; j < imgM.width - 1; j++) {
//            // 4 neighbor
//            if (imgM.map[i-1][j] == 0 || imgM.map[i+1][j] == 0 || imgM.map[i][j-1] == 0 || imgM.map[i][j+1] == 0) {
//                img.map[i][j].i = 0;
//            }
//        }
//    }
//    deleteMatrix(imgM);
//    return img;
//}
//
//
//
////--------------------------------Shrink function-----------------------------------------------------------
///* Shrink operation 
// Changes pixel from 0 to 1 if any neighbors are 1
// */
//Image imageShrink(Image orig) {
//    Image img = orig;
//    Matrix imgM = image2Matrix(orig);
//    // start 1 row lower
//    for (int i = 1; i < imgM.height - 1; i++) {
//        for (int j = 1; j < imgM.width - 1; j++) {
//            // 8 neighbor
//            if (imgM.map[i-1][j] == 255.0 || imgM.map[i+1][j] == 255.0 || imgM.map[i][j-1] == 255.0 || imgM.map[i][j+1] == 255.0
//                || imgM.map[i-1][j-1] == 255.0 || imgM.map[i-1][j+1] == 255.0 || imgM.map[i+1][j-1] == 255.0 || imgM.map[i+1][j+1]) {
//                img.map[i][j].i = 255.0;
//            }
//        }
//    }
//    deleteMatrix(imgM);
//    return img;
//}
//
//
////--------------------------------noise function-----------------------------------------------------------
///* function that adds binary noise to an image. This function receives an
// image and a floating point number p that indicates the probability
// (in percent) that each pixel in the image will be flipped, i.e.,
// turned from black to white or vice versa.
// */
//Image imageNoise(Image orig, float p) {
//    Image img = orig;
//    
//    for (int i = 1; i < img.height - 1; i++) {
//        for (int j = 1; j < img.width - 1; j++) {
//            float randomNum = (double)rand()/RAND_MAX;
//            if (randomNum < p) {
//                setPixel(img, i, j, NO_CHANGE, NO_CHANGE, NO_CHANGE, INVERT);
//            } else {
//                setPixel(img, i, j, NO_CHANGE, NO_CHANGE, NO_CHANGE, NO_CHANGE);
//            }
//            
//        }
//    }
//    return img;
//}
//
//// only for binary images
//// 4 neighbor component labeling
//Matrix componentLabeling(Matrix img, int eightNeighbor) {
//    Matrix comp = createMatrix(img.height, img.width); // matrix to look at for labeling
//    int eqTable[(img.height*img.width)/2+1];
//    int label = 0;
//    
//    // initializing labels to all -1
//    for (int i = 0; i < img.height; i++) {
//        for (int j = 0; j < img.width; j++) {
//            comp.map[i][j] = -1.0;
//        }
//    }
//    
//    // initialize away garbage values in my equivalence table
//    for (int i = 0; i < (img.height*img.width/2+1); i++) {
//        eqTable[i] = -1;
//    }
//    
//    // columns
//    for (int i = 0; i < img.height; i++) {
//        // rows
//        for (int j = 0; j < img.width; j++) {
//            // if the pixel is black 0 (white is 255)
//            if (img.map[i][j] == 0) {
//                int topIndex, leftIndex, topLeftIndex, topRightIndex;
//                
//                if (i == 0 && j == 0) {
//                    // if pixel is top left
//                    eqTable[label] = label;
//                    comp.map[i][j] = label;
//                    label++;
//                } else if (j == 0) {
//                    // leftmost pixels
//                    if (img.map[i-1][j] == 0) {
//                        // if upper pixel is labeled
//                        comp.map[i][j] = eqTable[(int) comp.map[i-1][j]];
//                    } else {
//                        eqTable[label] = label;
//                        comp.map[i][j] = label;
//                        label++;
//                    }
//                } else if (i == 0) {
//                    // topmost pixels
//                    if (img.map[i][j-1] == 0) {
//                        // if left pixel is labeled
//                        comp.map[i][j] = eqTable[(int) comp.map[i][j-1]];
//                    } else {
//                        eqTable[label] = label;
//                        comp.map[i][j] = label;
//                        label++;
//                    }
//                } else {
//                    if (img.map[i-1][j] == 0 && img.map[i][j-1] == 0) {
//                        topIndex = (int) comp.map[i-1][j];
//                        leftIndex = (int) comp.map[i][j-1];
//                        comp.map[i][j] = eqTable[topIndex]; // label in matrix is top label regardless
//                        if (eqTable[leftIndex] > eqTable[topIndex]) {
//                            eqTable[leftIndex] = eqTable[topIndex];
//                        } else if (eqTable[topIndex] > eqTable[leftIndex]) {
//                            eqTable[topIndex] = eqTable[leftIndex];
//                        }
//                    } else if (img.map[i-1][j] == 0) {
//                        topIndex = (int) comp.map[i-1][j];
//                        comp.map[i][j] = eqTable[topIndex];
//                    } else if (img.map[i][j-1] == 0) {
//                        leftIndex = (int) comp.map[i][j-1];
//                        comp.map[i][j] = eqTable[leftIndex];
//                    } else {
//                        eqTable[label] = label;
//                        comp.map[i][j] = label;
//                        label++;
//                    }
//
//                    // 8-neighbor connectivity
//                    if (eightNeighbor) {
//                        if (i > 0 && j > 0 && img.map[i - 1][j - 1] == 0) {
//                            topLeftIndex = (int)comp.map[i - 1][j - 1];
//                            if (eqTable[topLeftIndex] < eqTable[(int)comp.map[i][j]]) {
//                                eqTable[(int)comp.map[i][j]] = eqTable[topLeftIndex];
//                            } else {
//                                eqTable[topLeftIndex] = eqTable[(int)comp.map[i][j]];
//                            }
//                        }
//                        if (i > 0 && j < img.width - 1 && img.map[i - 1][j + 1] == 0) {
//                            topRightIndex = (int)comp.map[i - 1][j + 1];
//                            if (eqTable[topRightIndex] < eqTable[(int)comp.map[i][j]]) {
//                                eqTable[(int)comp.map[i][j]] = eqTable[topRightIndex];
//                            } else {
//                                eqTable[topRightIndex] = eqTable[(int)comp.map[i][j]];
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }
//    
//    // find lowest label
//    int x = 0;
//    while (eqTable[x] != -1) {
//        if (x != eqTable[x]) {
//            eqTable[x] = eqTable[(int)eqTable[x]];
//        }
//        x++;
//    }
//    
//    // second pass through picture
//    for (int i = 0; i < img.height; i++) {
//        for (int j = 0; j < img.width; j++) {
//            if (img.map[i][j] == 0) {
//                comp.map[i][j] = eqTable[((int)comp.map[i][j])];
//            }
//        }
//    }
//    
//    return comp;
//}
//
//Image componentColoring(Image orig, Matrix comp, int thresh) {
//    // count the amount of pixels per label for threshold
//    int numPixels[orig.height*orig.width/2+1];
//    memset(numPixels, 0, sizeof(numPixels));
//    for (int i = 0; i < orig.height; i++) {
//        for (int j = 0; j < orig.width; j++) {
//            numPixels[(int) comp.map[i][j]]++;
//        }
//    }
//    
//    // set random colors for each label
//    int pixColor[(orig.height * orig.width / 2 + 1) * 3];
//    for (int i = 0; i < (orig.height*orig.width/2+1); i++) {
//        pixColor[i*3] = rand() % 256;   // r
//        pixColor[i*3+1] = rand() % 256; // g
//        pixColor[i*3+2] = rand() % 256; // b
//    }
//    
//    int x = 0;
//    int totalComponents = 0;
//    while (x < (orig.height*orig.width/2+1)) {
//        if (numPixels[x] >= thresh){
//            totalComponents++;
//        }
//        x++;
//    }
//    printf("Total letters: %d\n", totalComponents);
//    
//    for (int i = 0; i < orig.height; i++) {
//        for (int j = 0; j < orig.width; j++) {
//            int label = (int) comp.map[i][j];
//            if((int) comp.map[i][j] != -1) {
//                if (numPixels[label] >= thresh) {
//                    setPixel(orig, i, j, pixColor[label*3], pixColor[label*3+1], pixColor[label*3+2], NO_CHANGE);
//                }
//            }
//        }
//    }
//    return orig;
//}
//
//// Takes in an image matrix of zeroes and a filter matrix, smoothing based on the average of the size of filter
//Matrix smoothing_filter(Matrix m1, Matrix m2) {
//    int fHeight = m2.height;
//    int fWidth = m2.width;
//    
//    for (int i = 0; i < (m1.height - fHeight); i++) {
//        for (int j = 0; j < (m1.width - fWidth); j++) {
//            double average = 0;
//            for (int h = 0; h < fHeight; h++) {
//                for (int w = 0; w < fWidth; w++) {
//                    average += m1.map[i+h][j+w]/(fHeight*fWidth);
//                }
//            }
//            m1.map[i+(int)floor(fHeight/2)][j+(int)floor(fWidth/2)] = average;
//        }
//    }
//    return m1;
//}
//
//// Compare function for doubles for C qsort
//int compare(const void* a, const void* b) {
//    double *x = (double *) a;
//    double *y = (double *) b;
//    if (*x < *y) return -1;
//    else if (*x > *y) return 1; return 0;
//}
//
//// Median filter, takes image matrix and filter matrix size
//Matrix median_filter(Matrix m1, Matrix m2) {
//    Matrix mat = m1;
//    int fHeight = m2.height;
//    int fWidth = m2.width;
//    int halfHeight = (int)floor(fHeight/2);
//    int halfWidth = (int)floor(fWidth/2);
//    
//    for (int i = halfHeight; i < (m1.height - halfHeight); i++) {
//        for (int j = halfWidth; j < (m1.width - halfWidth); j++) {
//            double weights[fHeight * fWidth];
//            int k = 0;
//            for (int h = 0; h < fHeight; h++) {
//                for (int w = 0; w < fWidth; w++) {
//                    weights[k++] = m1.map[i+h - halfHeight][j+w - halfWidth];
//                }
//            }
//            qsort(weights, fHeight * fWidth, sizeof(double), compare);
//            double median = weights[(fWidth*fHeight)/2];
//            mat.map[i][j] = median;
//        }
//    }
//    return mat;
//}
//
//// Image matrix m1
//// Filter m2
//// convolve rotates m2 180 degrees, multiplies 8 neighbors, and sums it in m1[i][j]
//Matrix convolve(Matrix m1, Matrix m2) {
//    Matrix mat = createMatrix(m1.height, m1.width);
//    int halfHeight = m2.height/2;
//    int halfWidth = m2.width/2;
//    
//    for (int i = halfHeight; i < (m1.height - halfHeight); i++) {
//        for (int j = halfWidth; j < (m1.width - halfWidth); j++) {
//            double sum = 0;
//            for (int x = 0; x < m2.height; x++) {
//                for (int y = 0; y < m2.width; y++) {
//                     sum += m2.map[x][y] * m1.map[i-halfHeight+x][j-halfHeight+y];
//                }
//            }
//            mat.map[i][j] = sum;
//        }
//    }
//    return mat;
//}
//
//Matrix gaussian(Matrix img, int filterSize, double sigma) {
//    double sum = 0.0;
//    int halfSize = filterSize / 2;
//    double filter[filterSize];
//        
//    // Calculate each value of the filter
//    for (int i = 0; i < filterSize; i++) {
//        int x = i - halfSize; // Centering the filter at 0
//        filter[i] = (1 / (sqrt(2 * PI) * sigma)) * exp(-(x * x) / (2 * sigma * sigma));
//        sum += filter[i]; // Keep track of the sum for normalization
//    }
//
//    // Normalize the filter so that the sum of all elements is 1
//    for (int i = 0; i < filterSize; i++) {
//        filter[i] /= sum;
//    }
//    
//    // convolve matrix with filter horizontally and vertically
//    Matrix filterX = createMatrixFromArray(filter, 1, filterSize);
//    Matrix filterY = createMatrixFromArray(filter, filterSize, 1);
//
//    // pass img through both -- separated for testing
//    img = convolve(img, filterX);
//    img = convolve(img, filterY);
//    
//    deleteMatrix(filterX);
//    deleteMatrix(filterY);
//    
//    return img;
//}
//
//// applies sobel filter using convolve
//Image sobel(Image img) {
//    double si[] = {1, 2, 1, 0, 0, 0, -1, -2, -1}; // horizontal
//    double sj[] = {1, 0, -1, 2, 0, -2, 1, 0, -1}; // vertical
//    
//    Matrix msi = createMatrixFromArray(si, 3, 3);
//    Matrix msj = createMatrixFromArray(sj, 3, 3);
//    
//    Matrix imgMat = image2Matrix(img);
//    // smoothing filter prior to convolutions
//    Matrix smoothFilter = createMatrix(3, 3);
//    imgMat = smoothing_filter(imgMat, smoothFilter);
//
//    // finding horizontal and vertical edges
//    Matrix mati = convolve(imgMat, msi);
//    Matrix matj = convolve(imgMat, msj);
//    
//    Matrix gMag = createMatrix(imgMat.height, imgMat.width);
//    Matrix gAngle = createMatrix(imgMat.height, imgMat.width);
//
//    int threshold = 180;
//    for (int i = 0; i < imgMat.height; i++) {
//        for (int j = 0; j < imgMat.width; j++) {
//            // Magnitude matrix
//            gMag.map[i][j] = sqrt(mati.map[i][j]*mati.map[i][j] + matj.map[i][j]*matj.map[i][j]);
//            // Angle matrix
//            gAngle.map[i][j] = atan2(matj.map[i][j], mati.map[i][j]);
//            if (gMag.map[i][j] < threshold) {
//                gMag.map[i][j] = 0;
//            }
//        }
//    }
//    
//    gMag = scale(gMag, 255);
//    img = matrix2Image(gMag, 0, 0);
//    
//    deleteMatrix(msi);
//    deleteMatrix(msj);
//    deleteMatrix(imgMat);
//    deleteMatrix(smoothFilter);
//    deleteMatrix(mati);
//    deleteMatrix(matj);
//    deleteMatrix(gMag);
//    deleteMatrix(gAngle);
//    
//    return img;
//}
//
//double maxDouble(double n1, double n2, double n3) {
//    if (n1 > n2 && n1 > n3) {
//        return n1;
//    } else if (n2 > n1 && n2 > n3) {
//        return n2;
//    } else if (n3 > n1 && n3 > n2) {
//        return n3;
//    } else {
//        return n1;
//    }
//}
//
//// applies canny filter using gaussian and convolve
//Image canny(Image img, int gauss) {
//    Matrix imgMat = image2Matrix(img);
//    imgMat = gaussian(imgMat, gauss, 1.0);
//
//    // find x and y gradients
//    double vDer[] = {0.5, 0.5, -0.5, -0.5};
//    double hDer[] = {0.5, -0.5, 0.5, -0.5};
//    Matrix vDerFilter = createMatrixFromArray(vDer, 2, 2);
//    Matrix hDerFilter = createMatrixFromArray(hDer, 2, 2);
//    Matrix gradX = convolve(imgMat, hDerFilter);
//    Matrix gradY = convolve(imgMat, vDerFilter);
//    
//    // find magnitude and orientation
//    Matrix mag = createMatrix(img.height, img.width);
//    
//    Matrix orient = createMatrix(img.height, img.width);
//    Matrix nonMaxima = createMatrix(img.height, img.width);
//    for (int i = 0; i < img.height; i++) {
//        for (int j = 0; j < img.width; j++) {
//            mag.map[i][j] = sqrt(pow(gradX.map[i][j],2) + pow(gradY.map[i][j],2));  // magnitude
//        }
//    }
//    
//    // scale magnitude to use full range of 0 to 255
//    mag = scale(mag, 255);
//    
//    for (int i = 0; i < mag.height; i++) {
//        for (int j = 0; j < mag.width; j++) {
//            double deg = atan2(gradY.map[i][j], gradX.map[i][j]) * (180.0 / M_PI); // convert radians to degrees
//            if (deg < 0) {
//                deg += 180; // normalize to [0, 360)
//            }
//            int q = 255;
//            int r = 255;
//            if ((deg >= 0 && deg < 22.5) || (deg >= 157.5 && deg <= 180)) {
//                q = mag.map[i][j + 1];
//                r = mag.map[i][j - 1];
//                
//                orient.map[i][j] = 0;
//            }
//            else if ((deg >= 22.5 && deg < 67.5) /*|| (deg > 202.5 && deg < 247.5)*/) {
//                q = mag.map[i + 1][j - 1];
//                r = mag.map[i - 1][j + 1];
//                
//                orient.map[i][j] = 50;
//            } else if ((deg >= 67.5 && deg < 112.5) /*|| (deg >= 247.5 && deg < 292.5)*/) {
//                q = mag.map[i + 1][j];
//                r = mag.map[i - 1][j];
//                
//                orient.map[i][j] = 100;
//            } else if ((deg >= 112.5 && deg < 157.5) /*|| (deg >= 292.5 && deg < 337.5)*/) {
//                q = mag.map[i + 1][j + 1];
//                r = mag.map[i - 1][j - 1];
//                
//                orient.map[i][j] = 150;
//            }
//
//            
//            if (mag.map[i][j] >= q && mag.map[i][j] >= r) {
//                nonMaxima.map[i][j] = mag.map[i][j];
//            } else {
//                nonMaxima.map[i][j] = 0;
//            }
//        }
//    }
//    nonMaxima = scale(nonMaxima, 255);
//
//    // hysteresis thresholding
//    Matrix hyst = createMatrix(img.height, img.width);
//    Matrix hystCandidates = createMatrix(img.height, img.width);
//    double threshLow = 55;
//    double threshHigh = 60;
//    
//    for (int i = 1; i < img.height-1; i++) {
//        for (int j = 1; j < img.width-1; j++) {
//            if (nonMaxima.map[i][j] > threshHigh) {
//                hyst.map[i][j] = 255; // edge
//                hystCandidates.map[i][j] = 0; // Components are black
//            } else if (nonMaxima.map[i][j] > threshLow) {
//                hyst.map[i][j] = 100; // to mark edge candidate
//                hystCandidates.map[i][j] = 0;
//            } else {
//                hyst.map[i][j] = 0;
//                hystCandidates.map[i][j] = 255;
//            }
//        }
//    }
//
//    // alternative to component labeling
//    for (int i = 1; i < img.height-1; i++) {
//     for (int j = 1; j < img.width-1; j++) {
//         if (hyst.map[i][j] == 100) {
//             if ((hyst.map[i+1][j-1] == 255) || (hyst.map[i+1][j] == 255) ||
//                 (hyst.map[i+1][j+1] == 255) || (hyst.map[i][j-1] == 255) ||
//                 (hyst.map[i][j+1] == 255) || (hyst.map[i-1][j-1] == 255) ||
//                 (hyst.map[i-1][j] == 255) || (hyst.map[i-1][j+1] == 255)) {
//                 hyst.map[i][j] = 255;
//             } else {
//                 hyst.map[i][j] = 0;
//             }
//         }
//     }
//    }
//    
//    return matrix2Image(hyst, 0, 0);
//}
