//
//  segmentTexture.h
//  testProj
//
//  Created by Nina Wang on 11/1/24.
//

#ifndef segmentTexture_h
#define segmentTexture_h

#include <stdio.h>
#include "netpbm.h"
#include "imageProc.h"
//#include "main.h"

Image segmentTexture(Image inputImg, int segments);
Matrix kMeans(Matrix featureVectors[], Matrix clusterLabels, int segments, int maxIter);

#endif /* segmentTexture_h */
