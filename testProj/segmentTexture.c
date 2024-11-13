//
//  segmentTexture.c
//  testProj
//
//  Created by Nina Wang on 11/1/24.
//

#include "segmentTexture.h"

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
    
    // PRINTING OUT l5s5
//    printf("printing l5s5 matrix\n");
//    for (int i = 0; i < l5s5.height; i++) {
//        for (int j = 0; j < l5s5.width; j++) {
//            printf("%d ", (int)l5s5.map[i][j]);
//        }
//        printf("\n");
//    }
    
    // derive local feature vectors
    Matrix inputMatrix = image2Matrix(inputImg);
    
    Matrix mat1 = convolve(inputMatrix, l5l5);
    Matrix mat2 = convolve(inputMatrix, l5e5);
    Matrix mat3 = convolve(inputMatrix, l5s5);
    Matrix mat4 = convolve(inputMatrix, l5w5);
    Matrix mat5 = convolve(inputMatrix, l5r5);
    
    Matrix mat6 = convolve(inputMatrix, e5l5);
    Matrix mat7 = convolve(inputMatrix, e5e5);
    Matrix mat8 = convolve(inputMatrix, e5s5);
    Matrix mat9 = convolve(inputMatrix, e5w5);
    Matrix mat10 = convolve(inputMatrix, e5r5);
    
    Matrix mat11 = convolve(inputMatrix, s5l5);
    Matrix mat12 = convolve(inputMatrix, s5e5);
    Matrix mat13 = convolve(inputMatrix, s5s5);
    Matrix mat14 = convolve(inputMatrix, s5w5);
    Matrix mat15 = convolve(inputMatrix, s5r5);
    
    Matrix mat16 = convolve(inputMatrix, w5l5);
    Matrix mat17 = convolve(inputMatrix, w5e5);
    Matrix mat18 = convolve(inputMatrix, w5s5);
    Matrix mat19 = convolve(inputMatrix, w5w5);
    Matrix mat20 = convolve(inputMatrix, w5r5);
    
    Matrix mat21 = convolve(inputMatrix, r5l5);
    Matrix mat22 = convolve(inputMatrix, r5e5);
    Matrix mat23 = convolve(inputMatrix, r5s5);
    Matrix mat24 = convolve(inputMatrix, r5w5);
    Matrix mat25 = convolve(inputMatrix, r5r5);

    // storing all matrices
    Matrix matrices[] = {mat1, mat2, mat3, mat4, mat5, mat6, mat7, mat8, mat9, mat10,
                        mat11, mat12, mat13, mat14, mat15, mat16, mat17, mat18, mat19,
                        mat20, mat21, mat22, mat23, mat24, mat25};
    
    // delete all filter matrices
    
    printf("laws texture energy measures for fifth pixel??\n");
    for (int i = 0; i < 25; i++) {
        if(i % 5  == 0) {
            printf("\n");
        }
        printf("%d ", (int)matrices[i].map[5][5]);
    }
    printf("\n");
    
    return matrix2Image(mat10, 0, 0);
}
