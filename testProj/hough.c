//
//  hough.c
//  testProj
//
//  Created by Nina Wang on 10/16/24.
//
#include <string.h>
#include <math.h>
#include "hough.h"


// Takes in edge detected image and returns accumulator
// Detect circles?
void houghTransformLines(Image image, int ***accumulator, int maxRadius) {

    int count = 0;
    for (int i = 0; i < image.height; i++) { // vertical
        count++;
        for (int j = 0; j < image.width; j++) { // horizontal
            // if image[height][width] is an edge (255 is white)
            if (image.map[i][j].i > 0) {
                for (int r = 1; r < maxRadius; r++) {
                    // This is where I would optimize to look only for orient and orient - 180
                    for (int theta = 0; theta < 360; theta++) {
                        double rad = theta * M_PI / 180.0;
                        int h = (int) j - r * cos(rad); // horizontal offset
                        int k = (int) i - r * sin(rad); // vertical offset
                        // h and k are valid if they are within the image width and height
                        if (h > 0 && h < image.width && k > 0 && k < image.height) {
                            // horizontal, vertical, radius
                            accumulator[h][k][r]++;
                        }
                    }
                }
            }
        }
        printf("hough status: %f\n", (double)count/image.height);
    }
}

void findHoughMaxima(int ***accumulator, int ***maxima, int maxRadius, int height, int width, int threshold) {
//    int maxima[width][height][maxRadius];
//    memset(maxima, 0, sizeof(maxima));
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int r = 1; r < maxRadius; r++) {
                // horizontal, vertical, radius
                int accVal = accumulator[x][y][r];
                if (accVal > threshold) {
                    // 3x3x3 cube neighbors
                    maxima[y][x][r] = 1;
                    for (int i = -1; i == 1; i++) {
                        for (int j = -1; j == 1; j++) {
                            for (int k = -1; k == 1; k++) {
                                if (accVal < accumulator[i][j][k]) {
                                    maxima[y][x][r] = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
//    for (int y = 0; y < height; y++) {
//        for (int x = 0; x < width; x++) {
//            for (int r = 1; r < maxRadius; r++) {
//                if (maxima[y][x][r] == 1) {
//                    
//                }
//            }
//        }
//    }
}
