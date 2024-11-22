//
//  segmentTexture.c
//  testProj
//
//  Created by Nina Wang on 11/1/24.
//

#include "segmentTexture.h"
#include <math.h>

#define NUM_LAWS_FILTERS 25
#define REDUCED_LAWS 15
#define NEIGHBORHOOD_SIZE 15

Matrix matrixMultSpecial(Matrix m1, Matrix m2) {
    // changes m1 to be column vector
    Matrix m1Col = createMatrix(m1.width, 1); // height of m1 width and width of 1
    
    for (int i = 0; i < m1.width; i++) {
        m1Col.map[i][0] = m1.map[0][i];
    }
    
    
    // # column of m1 and # rows in 2 has to be equal
   if (m1Col.height != m2.width) {
       printf("invalid matrices");
       deleteMatrix(m1Col);
       return createMatrix(0,0);
   }
    
    Matrix result = createMatrix(m1Col.height, m2.width);
    for (int i = 0; i < m1Col.height; i++) {
        for (int j = 0; j < m2.width; j++) {
            result.map[i][j] = 0;
            
            for (int k = 0; k < m1Col.width; k++) {
                result.map[i][j] += m1Col.map[i][k] * m2.map[k][j];
            }
        }
    }

    deleteMatrix(m1Col);
    return result;
}

Image segmentTexture(Image inputImg, int segments) {
    // edge detection first?
    
    // law's 5 filters
    double l[] = {1, 4, 6, 4, 1};       // center weighted local average
    double e[] = {-1, -2, 0, 2, 1};     // responds to row or col step edges
    double s[] = {-1, 0, 2, 0, -1};     // detects spots
    double r[] = {1, -4, 6, -4, 1};     // detects ripple
    double w[] = {-1, 2, 0, -2, 1};     // responds to an input pattern
    
    Matrix l5 = createMatrixFromArray(l, 1, 5);
    Matrix e5 = createMatrixFromArray(e, 1, 5);
    Matrix s5 = createMatrixFromArray(s, 1, 5);
    Matrix r5 = createMatrixFromArray(r, 1, 5);
    Matrix w5 = createMatrixFromArray(w, 1, 5);
    
    // create all 25 filters
    Matrix l5l5 = matrixMultSpecial(l5, l5);
    Matrix l5e5 = matrixMultSpecial(l5, e5);
    Matrix l5s5 = matrixMultSpecial(l5, s5);
    Matrix l5w5 = matrixMultSpecial(l5, w5);
    Matrix l5r5 = matrixMultSpecial(l5, r5);
    
    Matrix e5l5 = matrixMultSpecial(e5, l5);
    Matrix e5e5 = matrixMultSpecial(e5, e5);
    Matrix e5s5 = matrixMultSpecial(e5, s5);
    Matrix e5w5 = matrixMultSpecial(e5, w5);
    Matrix e5r5 = matrixMultSpecial(e5, r5);
    
    Matrix s5l5 = matrixMultSpecial(s5, l5);
    Matrix s5e5 = matrixMultSpecial(s5, e5);
    Matrix s5s5 = matrixMultSpecial(s5, s5);
    Matrix s5w5 = matrixMultSpecial(s5, w5);
    Matrix s5r5 = matrixMultSpecial(s5, r5);
    
    Matrix w5l5 = matrixMultSpecial(w5, l5);
    Matrix w5e5 = matrixMultSpecial(w5, e5);
    Matrix w5s5 = matrixMultSpecial(w5, s5);
    Matrix w5w5 = matrixMultSpecial(w5, s5);
    Matrix w5r5 = matrixMultSpecial(w5, r5);
    
    Matrix r5l5 = matrixMultSpecial(r5, l5);
    Matrix r5e5 = matrixMultSpecial(r5, e5);
    Matrix r5s5 = matrixMultSpecial(r5, s5);
    Matrix r5w5 = matrixMultSpecial(r5, w5);
    Matrix r5r5 = matrixMultSpecial(r5, r5);
    
    // derive local feature vectors
    Matrix inputMatrix = image2Matrix(inputImg);
    
    Matrix mat1 = convolve(inputMatrix, l5l5);
    Matrix mat2 = convolve(inputMatrix, l5e5); //
    Matrix mat3 = convolve(inputMatrix, l5s5); //
    Matrix mat4 = convolve(inputMatrix, l5w5); ///
    Matrix mat5 = convolve(inputMatrix, l5r5); //
    
    Matrix mat6 = convolve(inputMatrix, e5l5); //
    Matrix mat7 = convolve(inputMatrix, e5e5);
    Matrix mat8 = convolve(inputMatrix, e5s5); //
    Matrix mat9 = convolve(inputMatrix, e5w5); ///
    Matrix mat10 = convolve(inputMatrix, e5r5); ///
    
    Matrix mat11 = convolve(inputMatrix, s5l5); //
    Matrix mat12 = convolve(inputMatrix, s5e5); //
    Matrix mat13 = convolve(inputMatrix, s5s5);
    Matrix mat14 = convolve(inputMatrix, s5w5); ///
    Matrix mat15 = convolve(inputMatrix, s5r5); //
    
    Matrix mat16 = convolve(inputMatrix, w5l5); ///
    Matrix mat17 = convolve(inputMatrix, w5e5); ///
    Matrix mat18 = convolve(inputMatrix, w5s5); ///
    Matrix mat19 = convolve(inputMatrix, w5w5);
    Matrix mat20 = convolve(inputMatrix, w5r5); ///
    
    Matrix mat21 = convolve(inputMatrix, r5l5); //
    Matrix mat22 = convolve(inputMatrix, r5e5); ///
    Matrix mat23 = convolve(inputMatrix, r5s5); //
    Matrix mat24 = convolve(inputMatrix, r5w5); ///
    Matrix mat25 = convolve(inputMatrix, r5r5);

    // storing all matrices
    Matrix energies[] = {mat1, mat2, mat3, mat4, mat5, mat6, mat7, mat8, mat9, mat10,
                        mat11, mat12, mat13, mat14, mat15, mat16, mat17, mat18, mat19,
                        mat20, mat21, mat22, mat23, mat24, mat25};

    // delete all filter matrices
//    deleteMatrix(mat1);    deleteMatrix(mat2);    deleteMatrix(mat3);    deleteMatrix(mat4);    deleteMatrix(mat5);
//    deleteMatrix(mat6);    deleteMatrix(mat7);    deleteMatrix(mat8);    deleteMatrix(mat9);    deleteMatrix(mat10);
//    deleteMatrix(mat11);    deleteMatrix(mat12);    deleteMatrix(mat13);    deleteMatrix(mat14);    deleteMatrix(mat15);
//    deleteMatrix(mat16);    deleteMatrix(mat17);    deleteMatrix(mat18);    deleteMatrix(mat19);    deleteMatrix(mat20);
//    deleteMatrix(mat21);    deleteMatrix(mat22);    deleteMatrix(mat23);    deleteMatrix(mat24);    deleteMatrix(mat25);

    for (int i = 0; i < NUM_LAWS_FILTERS; i++) {
        // absolute value
        for (int y = 0; y < energies[i].height; y++) {
            for (int x = 0; x < energies[i].width; x++) {
                energies[i].map[y][x] = fabs(energies[i].map[y][x]);
            }
        }
        
        // NORMALIZE ALL MATRICES
        energies[i] = scale(energies[i], 255);
        
        Matrix energyMap = createMatrix(energies[i].height, energies[i].width);
        
//         summing neighborhood of each pixel
        for (int y = 0; y < energies[i].height; y++) {
            for (int x = 0; x < energies[i].width; x++) {
                float sum = 0.0;
                
                // Sum the neighborhood centered at (y,x)
                for (int dy = -NEIGHBORHOOD_SIZE/2; dy <= NEIGHBORHOOD_SIZE/2; dy++) {
                    for (int dx = -NEIGHBORHOOD_SIZE/2; dx <= NEIGHBORHOOD_SIZE/2; dx++) {
                        int ny = y + dy;
                        int nx = x + dx;
                        
                        // Check boundaries
                        if (ny >= 0 && ny < energies[i].height &&
                            nx >= 0 && nx < energies[i].width) {
                            sum += energies[i].map[ny][nx];
                        }
                    }
                }
                
                energyMap.map[y][x] = sum;
            }
        }
        
        energies[i] = energyMap;
        printf("energy map of %d\n", i);
        
        // FOR VISUALIZATION PURPOSES ONLY
//        energies[i] = scale(energies[i], 255);
    }
    
    // Combining complementary pairs, reducing filters from 25 to 15
    
    printf("combining complementary pairs");
    Matrix matLE = averageTwoMatrices(energies[1], energies[5]); // mat2 and mat6
    Matrix matLS = averageTwoMatrices(energies[2], energies[10]); // mat3 and mat11
    Matrix matLR = averageTwoMatrices(energies[4], energies[20]); // mat5 and mat21
    Matrix matES = averageTwoMatrices(energies[7], energies[11]); // mat8 and mat12
    Matrix matSR = averageTwoMatrices(energies[14], energies[22]); // mat15 and mat23
    Matrix matLW = averageTwoMatrices(energies[3], energies[15]); // mat4 and mat16
    Matrix matEW = averageTwoMatrices(energies[8], energies[16]); // mat9 and mat17
    Matrix matER = averageTwoMatrices(energies[9], energies[21]); // mat10 and mat22
    Matrix matSW = averageTwoMatrices(energies[13], energies[17]); // mat14 and mat18
    Matrix matRW = averageTwoMatrices(energies[19], energies[23]); // mat20 and mat24
    
    // I guess this is the 15D feature space vector?
    // Matrix of feature vectors, found per pixel [y][x]
    Matrix featureVectors[] = {energies[0], energies[6], energies[12], energies[18], energies[24],
        matLE, matLS, matLR, matES, matSR, matLW, matEW, matER, matSW, matRW};
    
    // remove biases with z score normalization
    struct zNorm {
        double mean;
        double standardDev;
    };
    
    // variable to store zNorms of all 15 feature vecs
    struct zNorm featureNorms[REDUCED_LAWS];
    
    // populate mean
    for (int i = 0; i < REDUCED_LAWS; i++) {
        double sum = 0;
        for (int y = 0; y < inputMatrix.height; y++) {
            for (int x = 0; x < inputMatrix.width; x++) {
                sum += featureVectors[i].map[y][x];
            }
        }
        featureNorms[i].mean = sum / (inputMatrix.height * inputMatrix.width);
    }
    
    // populate standard deviation
    for (int i = 0; i < REDUCED_LAWS; i++) {
        double sum_sq_diff = 0;
        for (int y = 0; y < inputMatrix.height; y++) {
            for (int x = 0; x < inputMatrix.width; x++) {
                sum_sq_diff += pow(featureVectors[i].map[y][x] - featureNorms[i].mean, 2);
            }
        }
        featureNorms[i].standardDev = sqrt(sum_sq_diff / (inputMatrix.height * inputMatrix.width));
    }
    
    // calculate z norms and update feature vector values
    for (int i = 0; i < REDUCED_LAWS; i++) {
        for (int y = 0; y < inputMatrix.height; y++) {
            for (int x = 0; x < inputMatrix.width; x++) {
                featureVectors[i].map[y][x] = (featureVectors[i].map[y][x] - featureNorms[i].mean) / featureNorms[i].standardDev;
            }
        }
    }
    
    // calculate degree of similarity with euclidian distance
    for (int i = 0; i < REDUCED_LAWS; i++) {
        
    }
    
    // use deg of sim to  k  means
    // zero mean and unit variance
    
    //
    
    // can clean up images with expand shrink????
    
    
    
    return matrix2Image(matRW, 0, 0);
}
