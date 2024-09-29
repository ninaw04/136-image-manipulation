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


Image imageBlackWhite(Image originalImg, double threshold);
Image imageExpand(Image originalImg);
Image imageShrink(Image originalImg);
Image imageNoise(Image originalImg, float percentage);
Matrix componentLabeling(Image cleanImage);
Image componentColoring(Image originalImg, Matrix components, int threshold);
Matrix smoothing_filter(Matrix image, Matrix filter);
Matrix median_filter(Matrix image, Matrix filter);

int main(int argc, const char * argv[]) {
    // creating images
//    Image inputImage = readImage("/Users/ninawang/Documents/School/CS136/testProj/netpbm/gray.pgm");
//    Image cleanImage = readImage("/Users/ninawang/Documents/School/CS136/testProj/netpbm/image_cleaned.pbm");
    Image car = readImage("/Users/ninawang/Documents/School/CS136/testProj/netpbm/car.ppm");
    Image sample = readImage("/Users/ninawang/Documents/School/CS136/testProj/netpbm/sample.ppm");

    // testing smoothing and filter
    printf("smoothing\n");
    Matrix sampleMatrix = image2Matrix(sample);
    Matrix filter = createMatrix(5, 5);
    sampleMatrix = smoothing_filter(sampleMatrix, filter);
    Image smoothed = matrix2Image(sampleMatrix, 0, 0);
    writeImage(smoothed, "/Users/ninawang/Documents/School/CS136/testProj/netpbm/sample_smoothed.ppm");
    printf("finished smoothing filter\n");
    
    // testing median filter
    Image noisySample = imageNoise(sample, 0.02);
    writeImage(noisySample, "/Users/ninawang/Documents/School/CS136/testProj/netpbm/noisy_sample.pbm");
    Matrix noisyMatrix = image2Matrix(noisySample);
    printf("median of sample\n");
    Matrix medianMatrix = median_filter(noisyMatrix, filter);
    Image medianImage = matrix2Image(medianMatrix, 0, 0);
    writeImage(medianImage, "/Users/ninawang/Documents/School/CS136/testProj/netpbm/sample_median.ppm");
    printf("finished median filter\n");
    
    // Create colored text image:
//    printf("finding components\n");
//    Matrix components = componentLabeling(cleanImage);
//    printf("coloring components\n");
//    Image textColoredImage = componentColoring(cleanImage, components, 200);
//    printf("done with components\n");
//    writeImage(textColoredImage, "/Users/ninawang/Documents/School/CS136/testProj/netpbm/text_colored.ppm");

    // Cleaning up images and matrices
//    deleteImage(inputImage);
//    deleteMatrix(components);
//    deleteImage(cleanImage);
    deleteImage(sample);
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
Matrix componentLabeling(Image img) {
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
            if (img.map[i][j].i == 0) {
                int topIndex;
                int leftIndex;
                
                if (i == 0 && j == 0) {
                    // if pixel is top left
                    eqTable[label] = label;
                    comp.map[i][j] = label;
                    label++;
                } else if (j == 0) {
                    // leftmost pixels
                    if (img.map[i-1][j].i == 0) {
                        // if upper pixel is labeled
                        comp.map[i][j] = eqTable[(int) comp.map[i-1][j]];
                    } else {
                        eqTable[label] = label;
                        comp.map[i][j] = label;
                        label++;
                    }
                } else if (i == 0) {
                    // topmost pixels
                    if (img.map[i][j-1].i == 0) {
                        // if left pixel is labeled
                        comp.map[i][j] = eqTable[(int) comp.map[i][j-1]];
                    } else {
                        eqTable[label] = label;
                        comp.map[i][j] = label;
                        label++;
                    }
                } else if (img.map[i-1][j].i == 0 && img.map[i][j-1].i == 0) {
                    topIndex = (int) comp.map[i-1][j];
                    leftIndex = (int) comp.map[i][j-1];
                    // if top and left neighbor are black
                    comp.map[i][j] = eqTable[topIndex]; // label in matrix is top label regardless
                    if (eqTable[leftIndex] > eqTable[topIndex]) {
                        eqTable[leftIndex] = eqTable[topIndex];
                    } else if (eqTable[topIndex] > eqTable[leftIndex]) {
                        eqTable[topIndex] = eqTable[leftIndex];
                    }
                } else if (img.map[i-1][j].i == 0) {
                    topIndex = (int) comp.map[i-1][j];
                    leftIndex = (int) comp.map[i][j-1];
                    // only top has label
                    comp.map[i][j] = eqTable[topIndex];
                } else if (img.map[i][j-1].i == 0) {
                    topIndex = (int) comp.map[i-1][j];
                    leftIndex = (int) comp.map[i][j-1];
                    // only left has label
                    comp.map[i][j] = eqTable[leftIndex];
                } else {
                    // assign new label to the pixel
                    eqTable[label] = label;
                    comp.map[i][j] = label;
                    label++;
                }
            }
            
        }
    }
    
    // find lowest label
    int x = 0;
    while (eqTable[x] != -1) { // might not be good? but also for this use case, it  won't  fill up
        if (x != eqTable[x]) {
            eqTable[x] = eqTable[(int)eqTable[x]];
        }
        x++;
    }
    
    // second pass through picture
    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            if (img.map[i][j].i == 0) {
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

int compare(const void* a, const void* b) {
    double *x = (double *) a;
    double *y = (double *) b;
    // return *x - *y; // this is WRONG...
    if (*x < *y) return -1;
    else if (*x > *y) return 1; return 0;
}

Matrix median_filter(Matrix m1, Matrix m2) {
    Matrix mat = m1;
    int fHeight = m2.height;
    int fWidth = m2.width;
    int halfHeight = (int)floor(fHeight/2);
    int halfWidth = (int)floor(fWidth/2);
    
    for (int i = halfHeight; i < (m1.height - halfHeight); i++) {
        for (int j = halfWidth; j < (m1.width - halfWidth); j++) {
            double weights[fHeight * fWidth]; // probably rounds up??
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
