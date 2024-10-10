//
//  main.c
//  CS136
//
//  Created by nha2 on 8/27/24.
//  Edited by Nina Wang on 9/29/24
// Test and demo program for netpbm. Reads a sample image and creates several output images.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "netpbm.h"
#include <stdio.h>
#include "main.h"

int main(int argc, const char * argv[]) {
    // creating images
    Image car_bw = readImage("/Users/ninawang/Documents/School/CS136/testProj/netpbm/car_bw.pgm");
    Image car = readImage("/Users/ninawang/Documents/School/CS136/testProj/netpbm/car.ppm");

    // testing sobel filter
    printf("starting sobel filter\n");
    Image sobelImg = sobel(car_bw);
    writeImage(sobelImg, "/Users/ninawang/Documents/School/CS136/testProj/netpbm/car_sobel.pgm");
    printf("finished sobel filter\n");
    
    // testing canny filter
    printf("test gaussian\n");
//    Matrix carG = image2Matrix(car_bw);
//    carG = gaussian(carG, 5, 5.0);
//    Image carGImg = matrix2Image(carG, 0, 0);
//    writeImage(carGImg, "/Users/ninawang/Documents/School/CS136/testProj/netpbm/car_g_horiz.pgm");
    printf("starting canny filter\n");
    Image car_nonmax = canny(car_bw);
    writeImage(car_nonmax, "/Users/ninawang/Documents/School/CS136/testProj/netpbm/car_nonmax.pgm");
    
 
    deleteImage(car_bw);
    deleteImage(car);

    printf("Program ends ... ");
    return 0;
}



//-------------------------------function_imageBlackWhite-------------------------------------------------
/* function that receives an Image structure and an intensity threshold
 to convert each pixel in the image to either black (intensity = 0)
 or white (intensity = 255). The function should return an Image structure
 containing the result. */
Image imageBlackWhite(Image originalImg, double threshold) {
    Image img = createImage(originalImg.height, originalImg.width); // store blackwhite image in here
    
    for (int i = 0; i < originalImg.height; i++) {
        for (int j = 0; j < originalImg.width; j++) {
            if(originalImg.map[i][j].i > threshold){ // change to white
                img.map[i][j].i = 255.0;
            } else { // change to black
                img.map[i][j].i = 0.0;
            }
        }
    }
    return img;
}



//--------------------------------Expand function-----------------------------------------------------------
/* Expand operation
 Changes pixel from 1 to 0 if any neighbors are 0
 */
Image imageExpand(Image orig) {
    Image img = orig;
    Matrix imgM = image2Matrix(orig);
    // start 1 row lower
    for (int i = 1; i < imgM.height - 1; i++) {
        for (int j = 1; j < imgM.width - 1; j++) {
            // 4 neighbor
            if (imgM.map[i-1][j] == 0 || imgM.map[i+1][j] == 0 || imgM.map[i][j-1] == 0 || imgM.map[i][j+1] == 0) {
                img.map[i][j].i = 0;
            }
        }
    }
    deleteMatrix(imgM);
    return img;
}



//--------------------------------Shrink function-----------------------------------------------------------
/* Shrink operation 
 Changes pixel from 0 to 1 if any neighbors are 1
 */
Image imageShrink(Image orig) {
    Image img = orig;
    Matrix imgM = image2Matrix(orig);
    // start 1 row lower
    for (int i = 1; i < imgM.height - 1; i++) {
        for (int j = 1; j < imgM.width - 1; j++) {
            // 8 neighbor
            if (imgM.map[i-1][j] == 255.0 || imgM.map[i+1][j] == 255.0 || imgM.map[i][j-1] == 255.0 || imgM.map[i][j+1] == 255.0
                || imgM.map[i-1][j-1] == 255.0 || imgM.map[i-1][j+1] == 255.0 || imgM.map[i+1][j-1] == 255.0 || imgM.map[i+1][j+1]) {
                img.map[i][j].i = 255.0;
            }
        }
    }
    deleteMatrix(imgM);
    return img;
}


//--------------------------------noise function-----------------------------------------------------------
/* function that adds binary noise to an image. This function receives an
 image and a floating point number p that indicates the probability
 (in percent) that each pixel in the image will be flipped, i.e.,
 turned from black to white or vice versa.
 */
Image imageNoise(Image orig, float p) {
    Image img = orig;
    
    for (int i = 1; i < img.height - 1; i++) {
        for (int j = 1; j < img.width - 1; j++) {
            float randomNum = (double)rand()/RAND_MAX;
            if (randomNum < p) {
                setPixel(img, i, j, NO_CHANGE, NO_CHANGE, NO_CHANGE, INVERT);
            } else {
                setPixel(img, i, j, NO_CHANGE, NO_CHANGE, NO_CHANGE, NO_CHANGE);
            }
            
        }
    }
    return img;
}

// only for binary images
// 4 neighbor component labeling
Matrix componentLabeling(Matrix img, int eightNeighbor) {
    Matrix comp = createMatrix(img.height, img.width); // matrix to look at for labeling
    int eqTable[(img.height*img.width)/2+1];
    int label = 0;
    
    // initializing labels to all -1
    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            comp.map[i][j] = -1.0;
        }
    }
    
    // initialize away garbage values in my equivalence table
    for (int i = 0; i < (img.height*img.width/2+1); i++) {
        eqTable[i] = -1;
    }
    
    // columns
    for (int i = 0; i < img.height; i++) {
        // rows
        for (int j = 0; j < img.width; j++) {
            // if the pixel is black 0 (white is 255)
            if (img.map[i][j] == 0) {
                int topIndex, leftIndex, topLeftIndex, topRightIndex;
                
                if (i == 0 && j == 0) {
                    // if pixel is top left
                    eqTable[label] = label;
                    comp.map[i][j] = label;
                    label++;
                } else if (j == 0) {
                    // leftmost pixels
                    if (img.map[i-1][j] == 0) {
                        // if upper pixel is labeled
                        comp.map[i][j] = eqTable[(int) comp.map[i-1][j]];
                    } else {
                        eqTable[label] = label;
                        comp.map[i][j] = label;
                        label++;
                    }
                } else if (i == 0) {
                    // topmost pixels
                    if (img.map[i][j-1] == 0) {
                        // if left pixel is labeled
                        comp.map[i][j] = eqTable[(int) comp.map[i][j-1]];
                    } else {
                        eqTable[label] = label;
                        comp.map[i][j] = label;
                        label++;
                    }
                } else {
                    if (img.map[i-1][j] == 0 && img.map[i][j-1] == 0) {
                        topIndex = (int) comp.map[i-1][j];
                        leftIndex = (int) comp.map[i][j-1];
                        comp.map[i][j] = eqTable[topIndex]; // label in matrix is top label regardless
                        if (eqTable[leftIndex] > eqTable[topIndex]) {
                            eqTable[leftIndex] = eqTable[topIndex];
                        } else if (eqTable[topIndex] > eqTable[leftIndex]) {
                            eqTable[topIndex] = eqTable[leftIndex];
                        }
                    } else if (img.map[i-1][j] == 0) {
                        topIndex = (int) comp.map[i-1][j];
                        comp.map[i][j] = eqTable[topIndex];
                    } else if (img.map[i][j-1] == 0) {
                        leftIndex = (int) comp.map[i][j-1];
                        comp.map[i][j] = eqTable[leftIndex];
                    } else {
                        eqTable[label] = label;
                        comp.map[i][j] = label;
                        label++;
                    }

                    // 8-neighbor connectivity
                    if (eightNeighbor) {
                        if (i > 0 && j > 0 && img.map[i - 1][j - 1] == 0) {
                            topLeftIndex = (int)comp.map[i - 1][j - 1];
                            if (eqTable[topLeftIndex] < eqTable[(int)comp.map[i][j]]) {
                                eqTable[(int)comp.map[i][j]] = eqTable[topLeftIndex];
                            } else {
                                eqTable[topLeftIndex] = eqTable[(int)comp.map[i][j]];
                            }
                        }
                        if (i > 0 && j < img.width - 1 && img.map[i - 1][j + 1] == 0) {
                            topRightIndex = (int)comp.map[i - 1][j + 1];
                            if (eqTable[topRightIndex] < eqTable[(int)comp.map[i][j]]) {
                                eqTable[(int)comp.map[i][j]] = eqTable[topRightIndex];
                            } else {
                                eqTable[topRightIndex] = eqTable[(int)comp.map[i][j]];
                            }
                        }
                    }
                }
            }
        }
    }
    
    // find lowest label
    int x = 0;
    while (eqTable[x] != -1) {
        if (x != eqTable[x]) {
            eqTable[x] = eqTable[(int)eqTable[x]];
        }
        x++;
    }
    
    // second pass through picture
    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            if (img.map[i][j] == 0) {
                comp.map[i][j] = eqTable[((int)comp.map[i][j])];
            }
        }
    }
    
    return comp;
}

Image componentColoring(Image orig, Matrix comp, int thresh) {
    // count the amount of pixels per label for threshold
    int numPixels[orig.height*orig.width/2+1];
    memset(numPixels, 0, sizeof(numPixels));
    for (int i = 0; i < orig.height; i++) {
        for (int j = 0; j < orig.width; j++) {
            numPixels[(int) comp.map[i][j]]++;
        }
    }
    
    // set random colors for each label
    int pixColor[(orig.height * orig.width / 2 + 1) * 3];
    for (int i = 0; i < (orig.height*orig.width/2+1); i++) {
        pixColor[i*3] = rand() % 256;   // r
        pixColor[i*3+1] = rand() % 256; // g
        pixColor[i*3+2] = rand() % 256; // b
    }
    
    int x = 0;
    int totalComponents = 0;
    while (x < (orig.height*orig.width/2+1)) {
        if (numPixels[x] >= thresh){
            totalComponents++;
        }
        x++;
    }
    printf("Total letters: %d\n", totalComponents);
    
    for (int i = 0; i < orig.height; i++) {
        for (int j = 0; j < orig.width; j++) {
            int label = (int) comp.map[i][j];
            if((int) comp.map[i][j] != -1) {
                if (numPixels[label] >= thresh) {
                    setPixel(orig, i, j, pixColor[label*3], pixColor[label*3+1], pixColor[label*3+2], NO_CHANGE);
                }
            }
        }
    }
    return orig;
}

// Takes in an image matrix of zeroes and a filter matrix, smoothing based on the average of the size of filter
Matrix smoothing_filter(Matrix m1, Matrix m2) {
    int fHeight = m2.height;
    int fWidth = m2.width;
    
    for (int i = 0; i < (m1.height - fHeight); i++) {
        for (int j = 0; j < (m1.width - fWidth); j++) {
            double average = 0;
            for (int h = 0; h < fHeight; h++) {
                for (int w = 0; w < fWidth; w++) {
                    average += m1.map[i+h][j+w]/(fHeight*fWidth);
                }
            }
            m1.map[i+(int)floor(fHeight/2)][j+(int)floor(fWidth/2)] = average;
        }
    }
    return m1;
}

// Compare function for doubles for C qsort
int compare(const void* a, const void* b) {
    double *x = (double *) a;
    double *y = (double *) b;
    if (*x < *y) return -1;
    else if (*x > *y) return 1; return 0;
}

// Median filter, takes image matrix and filter matrix size
Matrix median_filter(Matrix m1, Matrix m2) {
    Matrix mat = m1;
    int fHeight = m2.height;
    int fWidth = m2.width;
    int halfHeight = (int)floor(fHeight/2);
    int halfWidth = (int)floor(fWidth/2);
    
    for (int i = halfHeight; i < (m1.height - halfHeight); i++) {
        for (int j = halfWidth; j < (m1.width - halfWidth); j++) {
            double weights[fHeight * fWidth];
            int k = 0;
            for (int h = 0; h < fHeight; h++) {
                for (int w = 0; w < fWidth; w++) {
                    weights[k++] = m1.map[i+h - halfHeight][j+w - halfWidth];
                }
            }
            qsort(weights, fHeight * fWidth, sizeof(double), compare);
            double median = weights[(fWidth*fHeight)/2];
            mat.map[i][j] = median;
        }
    }
    return mat;
}

// Image matrix m1
// Filter m2
// convolve rotates m2 180 degrees, multiplies 8 neighbors, and sums it in m1[i][j]
Matrix convolve(Matrix m1, Matrix m2) {
    Matrix mat = createMatrix(m1.height, m1.width);
    int halfHeight = m2.height/2;
    int halfWidth = m2.width/2;
    
    for (int i = halfHeight; i < (m1.height - halfHeight); i++) {
        for (int j = halfWidth; j < (m1.width - halfWidth); j++) {
            double sum = 0;
            for (int x = 0; x < m2.height; x++) {
                for (int y = 0; y < m2.width; y++) {
                     sum += m2.map[x][y] * m1.map[i-halfHeight+x][j-halfHeight+y];
                }
            }
            mat.map[i][j] = sum;
        }
    }
    return mat;
}

Matrix gaussian(Matrix img, int filterSize, double sigma) {
    double sum = 0.0;
    int halfSize = filterSize / 2;
    double filter[filterSize];
        
    // Calculate each value of the filter
    for (int i = 0; i < filterSize; i++) {
        int x = i - halfSize; // Centering the filter at 0
        filter[i] = (1 / (sqrt(2 * PI) * sigma)) * exp(-(x * x) / (2 * sigma * sigma));
        sum += filter[i]; // Keep track of the sum for normalization
    }

    // Normalize the filter so that the sum of all elements is 1
    for (int i = 0; i < filterSize; i++) {
        filter[i] /= sum;
    }
    
    // convolve matrix with filter horizontally and vertically
    Matrix filterX = createMatrixFromArray(filter, 1, filterSize);
    Matrix filterY = createMatrixFromArray(filter, filterSize, 1);

    // pass img through both -- separated for testing
    img = convolve(img, filterX);
    img = convolve(img, filterY);
    
    deleteMatrix(filterX);
    deleteMatrix(filterY);
    
    return img;
}

// applies sobel filter using convolve
Image sobel(Image img) {
    double si[] = {1, 2, 1, 0, 0, 0, -1, -2, -1}; // horizontal
    double sj[] = {1, 0, -1, 2, 0, -2, 1, 0, -1}; // vertical
    
    Matrix msi = createMatrixFromArray(si, 3, 3);
    Matrix msj = createMatrixFromArray(sj, 3, 3);
    
    Matrix imgMat = image2Matrix(img);
    // smoothing filter prior to convolutions
    Matrix smoothFilter = createMatrix(3, 3);
    imgMat = smoothing_filter(imgMat, smoothFilter);

    // finding horizontal and vertical edges
    Matrix mati = convolve(imgMat, msi);
    Matrix matj = convolve(imgMat, msj);
    
    Matrix gMag = createMatrix(imgMat.height, imgMat.width);
    Matrix gAngle = createMatrix(imgMat.height, imgMat.width);

    int threshold = 180;
    for (int i = 0; i < imgMat.height; i++) {
        for (int j = 0; j < imgMat.width; j++) {
            // Magnitude matrix
            gMag.map[i][j] = sqrt(mati.map[i][j]*mati.map[i][j] + matj.map[i][j]*matj.map[i][j]);
            // Angle matrix
            gAngle.map[i][j] = atan2(matj.map[i][j], mati.map[i][j]);
            if (gMag.map[i][j] < threshold) {
                gMag.map[i][j] = 0;
            }
        }
    }
    
    img = matrix2Image(gMag, 0, 0);
    
    deleteMatrix(msi);
    deleteMatrix(msj);
    deleteMatrix(imgMat);
    deleteMatrix(smoothFilter);
    deleteMatrix(mati);
    deleteMatrix(matj);
    deleteMatrix(gMag);
    deleteMatrix(gAngle);
    
    return img;
}

double maxDouble(double n1, double n2, double n3) {
    if (n1 > n2 && n1 > n3) {
        return n1;
    } else if (n2 > n1 && n2 > n3) {
        return n2;
    } else if (n3 > n1 && n3 > n2) {
        return n3;
    } else {
        return n1;
    }
}

// applies canny filter using convolve
// smooth image: apply gaussian with standard deviation
// compute gradient with smoothed array
Image canny(Image img) {
    Matrix imgMat = image2Matrix(img);
    imgMat = gaussian(imgMat, 5, 2.0);
    // find x and y gradients
     double vDer[] = {0.5, 0.5, -0.5, -0.5};
     double hDer[] = {0.5, -0.5, 0.5, -0.5};
     Matrix vDerFilter = createMatrixFromArray(vDer, 2, 2);
     Matrix hDerFilter = createMatrixFromArray(hDer, 2, 2);
//    double vDer[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
//    double hDer[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
//    Matrix vDerFilter = createMatrixFromArray(vDer, 3, 3);
//    Matrix hDerFilter = createMatrixFromArray(hDer, 3, 3);
    Matrix gradX = convolve(imgMat, hDerFilter);
    Matrix gradY = convolve(imgMat, vDerFilter);
    
    // find magnitude and orientation
    Matrix mag = createMatrix(img.height, img.width);
    Matrix orient = createMatrix(img.height, img.width);
    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            mag.map[i][j] = sqrt(pow(gradX.map[i][j],2) + pow(gradY.map[i][j],2));  // magnitude
            
            // orientation
            // double deg = atan2(gradY.map[i][j], gradX.map[i][j]);
            double deg = atan2(gradY.map[i][j], gradX.map[i][j]) * (180.0 / M_PI); // convert radians to degrees
            if (deg < 0) {
                deg += 360; // normalize to [0, 360)
            }
            if ((deg > 22.5 && deg <= 67.5) || (deg > 202.5 && deg < 247.5)) {
                orient.map[i][j] = 0;  // 45 and 225 deg   1
            } else if ((deg > 67.5 && deg <= 112.5) || (deg >= 247.5 && deg < 292.5)) {
                orient.map[i][j] = 45;  // 90 and 270 deg  2
            } else if ((deg > 112.5 && deg <= 157.5) || (deg >= 292.5 && deg < 337.5)) {
                orient.map[i][j] = 90; // 135 and 315 deg  3
            } else {
                orient.map[i][j] = 135;   // 0 and 180 deg  0
            }
        }
    }
    
    Matrix nonMaxima = createMatrix(img.height, img.width);
    Matrix hyst = createMatrix(img.height, img.width);
    Matrix hystCandidates = createMatrix(img.height, img.width);
    double threshLow = 20;
    double threshHigh = 60;
    
    for (int i = 1; i < img.height-1; i++) {
        for (int j = 1; j < img.width-1; j++) {
            double e = mag.map[i][j];
            // non maximum suppression
            switch ((int)orient.map[i][j]) {
                case 0:
                    if (e >= mag.map[i][j-1] && mag.map[i][j+1]) {
                        nonMaxima.map[i][j] = e;
                    } else {
                        nonMaxima.map[i][j] = 0;
                    }
                    break;
                case 45:
                    if (e >= mag.map[i-1][j+1] && e >= mag.map[i+1][j-1]) {
                        nonMaxima.map[i][j] = e;
                    } else {
                        nonMaxima.map[i][j] = 0;
                    }
                    break;
                case 90:
                    if (e >= mag.map[i-1][j] && e >= mag.map[i+1][j]) {
                        nonMaxima.map[i][j] = e;
                    } else {
                        nonMaxima.map[i][j] = 0;
                    }                    
                    break;
                case 135:    
                    if (e >= mag.map[i-1][j-1] && e >= mag.map[i+1][j+1]) {
                        nonMaxima.map[i][j] = e;
                    } else {
                        nonMaxima.map[i][j] = 0;
                    }                
                    break;
            }
            // hysteresis thresholding
            if (nonMaxima.map[i][j] > threshHigh) {
                hyst.map[i][j] = 255; // edge
                hystCandidates.map[i][j] = 0; // Components are black
            } else if (nonMaxima.map[i][j] > threshLow) {
                hyst.map[i][j] = 100; // to mark edge candidate
                hystCandidates.map[i][j] = 0;
            } else {
                hyst.map[i][j] = 0;
                hystCandidates.map[i][j] = 255;
            }
        }
    }
    
    for (int i = 0; i < img.height; i++) {
        hystCandidates.map[i][0] = 255;
        hystCandidates.map[i][img.width - 1] = 255;
    }
    for (int j = 0; j < img.width; j++) {
        hystCandidates.map[0][j] = 255;
        hystCandidates.map[img.height - 1][j] = 255;
    }
    
    Matrix components = componentLabeling(hystCandidates, 1);
    printf("found components\n");
    
    int acceptedComps[img.width * img.height];
    memset(acceptedComps, 0, sizeof(acceptedComps));
    
    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            if ((int)hyst.map[i][j] == 255) {
                acceptedComps[(int)components.map[i][j]] = 1;
            }
        }
    }
    
    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            if (acceptedComps[(int)components.map[i][j]] == 1) {
                hyst.map[i][j] = 255; // edge pixels set to white
            } else {
                hyst.map[i][j] = 0; // background pixels set to black
            }
        }
    }
    
    return matrix2Image(hyst, 0, 0);
    
    /*
     something wrong with component labeling
     not setting the 255 edges as the actual edges
     */
    
}
