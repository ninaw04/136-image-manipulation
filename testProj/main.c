//
//  main.c
//  CS136
//
//  Created by nha2 on 8/27/24.
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

int main(int argc, const char * argv[]) {
    //-------------------------------------------------------------------------------
       //create blackWhiteImage:
        // You need to change the path file: /Users/nha2/Downloads/Proj1_part1_2/Proj1_part1_2/ to your local directory
    Image inputImage = readImage("/Users/ninawang/Documents/School/CS136/testProj/netpbm/gray.pgm");
    Image cleanImage = readImage("/Users/ninawang/Documents/School/CS136/testProj/netpbm/image_cleaned.pbm");
    
//    printf("what does rand do anyways: %d\n", rand());
//    for (int i = 0; i < 10; i++) {
//        printf("%f\n", (double)rand()/RAND_MAX);
//    }
    
    Image noiseImage = imageNoise(cleanImage, 0.6);
    writeImage(noiseImage, "/Users/ninawang/Documents/School/CS136/testProj/netpbm/noise.pbm");

       //-------------------------------------------------------------------------------
       // COMMENT THIS FUNCTION IF YOU DON'T WANT IT TO RUN EVEY TIME
//    Image rotatedImage = createImage(inputImage.height, inputImage.width);
//    Image invertedImage = createImage(inputImage.height, inputImage.width);
//       for (int x = 0; x < inputImage.width; x++)
//           for (int y = 0; y < inputImage.height; y++)
//           {
//               rotatedImage.map[y][x] = inputImage.map[inputImage.height - y - 1][inputImage.width - x - 1];
//
//               invertedImage.map[y][x].r = 255 - inputImage.map[y][x].r;
//               invertedImage.map[y][x].g = 255 - inputImage.map[y][x].g;
//               invertedImage.map[y][x].b = 255 - inputImage.map[y][x].b;
//               // Let's just ignore 'i' here; it's irrelevant if we want to save image as PPM.
//           }
//       writeImage(rotatedImage, "/Users/ninawang/Documents/School/CS136/project1/netpbm/rotated.pbm");
//       writeImage(invertedImage, "/Users/ninawang/Documents/School/CS136/project1/netpbm/inverted.pbm");
//       writeImage(inputImage, "/Users/ninawang/Documents/School/CS136/testProj/netpbm/gray.pgm");
//       writeImage(inputImage, "/Users/ninawang/Documents/School/CS136/testProj/netpbm/black-white.pbm");

    //-------------------------------------------------------------------------------
        //  create blackWhiteImage:
    printf("creating black white image\n");
    Image blackWhite = imageBlackWhite(inputImage, 180);
    writeImage(blackWhite, "/Users/ninawang/Documents/School/CS136/testProj/netpbm/black-white.pbm");
    
       // create noiseImage:


    // Sequence of expand shrink shrink expand image
    
    printf("0. expand image\n\tbc white is 8 neighbor\n");
    Image imageCleaned = imageExpand(blackWhite);
    writeImage(imageCleaned, "/Users/ninawang/Documents/School/CS136/testProj/netpbm/image_cleaned.pbm");
    
    printf("1. shrink image\n\tremoves most black noise\n");
    imageCleaned = imageShrink(imageCleaned);
    writeImage(imageCleaned, "/Users/ninawang/Documents/School/CS136/testProj/netpbm/image_cleaned.pbm");
    
    printf("2. shrink image\n\teliminates rest of black noise\n");
    imageCleaned = imageShrink(imageCleaned);
    writeImage(imageCleaned, "/Users/ninawang/Documents/School/CS136/testProj/netpbm/image_cleaned.pbm");

    printf("4. expand image\n\tthicken lines\n");
    imageCleaned = imageExpand(imageCleaned);
    writeImage(imageCleaned, "/Users/ninawang/Documents/School/CS136/testProj/netpbm/image_cleaned.pbm");

       //-------------------------------------------------------------------------------
       // Uncomment this after you finish your homework
       // Function that does threshold, noise and numbers of spanding and shrinking
       // COMMENT THIS FUNCTION IF YOU DON'T WANT IT TO RUN EVEY TIME
//        function_readImage();

        /* Delete back and white, noise, Expand and Shrink */
    deleteImage(inputImage);
//    deleteImage(blackWhite);
    deleteImage(cleanImage);
//       deleteImage(rotatedImage);
//       deleteImage(invertedImage);




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
//    int eqTable[(img.height*img.width)/2 + 1];
    int eqTable[(img.height*img.width)];
    int eqTableIndex = 0;
    int label = 0;
    
    // initialize away garbage values in my equivalence table
    for (int i = 0; i < (img.height*img.width); i++) {
        eqTable[i] = -1;
    }
    
    // columns
    for (int i = 0; i < img.height - 1; i++) {
        // rows
        for (int j = 0; j < img.width - 1; j++) {
            // if the pixel is black
            if (img.map[i][j].i == 1) {
                if (i == 0 && j == 0) {
                    // if pixel is top left
                    eqTable[label*2] = label;
                    eqTable[label*2+1] = label;
                    
                    comp.map[i][j] = label;
                    label++;
                } else if (j == 0) {
                    // leftmost pixels
                    if (img.map[i-1][j].i == 1) {
                        // if upper pixel is labeled
                        comp.map[i][j] = comp.map[i-1][j];
                    } else {
                        eqTable[label*2] = label;
                        eqTable[label*2+1] = label;
                        
                        comp.map[i][j] = label;
                        label++;
                    }
                } else if (img.map[i-1][j].i == 1 && img.map[i][j-1].i == 1) {
                    // if top and left neighbor are black
                    comp.map[i][j] = comp.map[i-1][j]; // label is top label regardless
                    
                    if (comp.map[i-1][j] != comp.map[i-1][j]) {
                        // pairs of indexes in the array are equivalent
                        eqTable[(int) comp.map[i][j-1]*2+1] = comp.map[i-1][j];
                    }
                } else if (img.map[i-1][j].i == 1) {
                    // only top has label
                    comp.map[i][j] = comp.map[i-1][j];
                } else if (img.map[i][j-1].i == 1) {
                    // only left has label
                    comp.map[i][j] = comp.map[i][j-1];
                } else {
                    // assign new label to the pixel
                    eqTable[label*2] = label;
                    eqTable[label*2+1] = label;
                    
                    comp.map[i][j] = label;
                    label++;
                }
            }
            
        }
    }
    
    // find lowest label
    for (int i = 0; i < (img.height*img.width); i++) {
        if (eqTable[i*2] != eqTable[i*2+1]) {
            eqTable[i*2+1] = eqTable[eqTable[i*2]+1];
        }
    }
    
    // second pass through picture
    for (int i = 0; i < img.height - 1; i++) {
        for (int j = 0; j < img.width - 1; j++) {
            if (img.map[i][j].i == 1) {
                comp.map[i][j] = eqTable[((int)comp.map[i][j])*2+1];
            }
        }
    }
    
    return comp;
}
