//
//  hough.c
//  testProj
//
//  Created by Nina Wang on 10/16/24.
//
#include <stdio.h>
#include <math.h>
#include "hough.h"


// Takes in edge detected image and returns circle accumulator
void houghTransformLines(Image image, int ***accumulator, int maxRadius) {

    int count = 0;
    for (int i = 0; i < image.height; i++) { // vertical
        count++;
        for (int j = 0; j < image.width; j++) { // horizontal
            // if image[height][width] is an edge (255 is white)
            if ((int) image.map[i][j].i == 255) {
                for (int r = 1; r < maxRadius; r++) {
                    // This is where I would optimize to look only for orient and orient - 180
                    for (int theta = 0; theta < 360; theta++) {
                        double rad = theta * M_PI / 180.0;
                        int h = (int) (j - r * cos(rad)); // horizontal offset
                        int k = (int) (i - r * sin(rad)); // vertical offset
                        // h and k are valid if they are within the image width and height
                        if (h >= 0 && h < image.width && k >= 0 && k < image.height) {
                            // vertical, horizontal, radius
                            accumulator[k][h][r]++;
                        }
                    }
                }
            }
        }
        printf("hough status: %f\n", (double)count/image.height);
    }
}

void findHoughMaxima(int ***accumulator, int ***maxima, int maxRadius, int height, int width, int threshold) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int r = 30; r < maxRadius; r++) {
                // vertical, horizontal, radius
                int accVal = accumulator[y][x][r];
                if (accVal > threshold) {
                    // consider it a maxima
                    maxima[y][x][r] = 1;
                    // 3x3x3 cube neighbors
                    for (int i = -6; i <= 6; i++) {
                        for (int j = -6; j <= 6; j++) {
                            for (int k = -6; k <= 6; k++) {
                                // skip center point
                                if (i==0 && j==0 && k==0) continue;
                                int ni = y + i;
                                int nj = x + j;
                                int nk = r + k;
                                if (ni >= 0 && ni < height && nj >= 0 && nj < width && nk >= 0 && nk < maxRadius) {
                                    if (accVal < accumulator[ni][nj][nk]) {
                                        // vertical horizontal radius
                                        maxima[y][x][r] = 0;
                                        break; // series of breaks out of cube
                                    }
                                }
                            }
                            if (maxima[y][x][r] == 0) break;
                        }
                        if (maxima[y][x][r] == 0) break;
                    }
                }
            }
        }
    }
}
