#ifndef READ_HYPERSPECTRAL_AND_PARAMETERS_FILE_H
#define READ_HYPERSPECTRAL_AND_PARAMETERS_FILE_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "preesm.h"

typedef  struct parameters {
	int k;
	float minErr;
	int maxIter;
	int verbose;
}parameters;

#define namefile PROJECT_ROOT_PATH "/dat/Parameters.txt"
//#define nameimage PROJECT_ROOT_PATH "/dat/Op4C3.txt"
#define nameimage PROJECT_ROOT_PATH "/dat/Op20C1.bin"
#define txtfile PROJECT_ROOT_PATH "/dat/image.txt"

void readParAndImg (int rows,int columns,int bands,int pixels,OUT float *image,OUT struct parameters *par);

#endif
