//
//  hough.h
//  testProj
//
//  Created by Nina Wang on 10/16/24.
//

#ifndef hough_h
#define hough_h

#include <stdio.h>
#include "netpbm.h"

void houghTransformLines(Image image, int ***accumulator, int maxRadius);
void findHoughMaxima(int ***accumulator, int ***maxima, int maxRadius, int height, int width, int threshold);

#endif /* hough_h */
