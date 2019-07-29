/*
	============================================================================
	Name        : utils.c
	Author      : rlazcano and dmadronal
	Version     : 1.0
	Copyright   : CECILL-C ?
	Description : Useful functions for the hyperspectral image processing chain
	============================================================================
*/
#include "utils.h"


/*========================================================================

   transposeMatrix DEFINITION

   ======================================================================*/

void transposeMatrix(int matrixRows, int matrixColumns, float *matrix, float *matrixTransposed) {

	int i = 0;
    int j = 0;	
	
	for(i = 0; i < matrixRows; i++){
		for(j = 0; j < matrixColumns; j++){
			matrixTransposed[j * matrixRows + i] = matrix[i * matrixColumns + j];
		}
	}
}

/*========================================================================

   decisionMaking DEFINITION

   ======================================================================*/
void decisionMaking(int pixels, int numClasses, float *pixelProbs, char *pixelClasses) {

	int q = 0;
	int b = 0;
	char decision = 0;

	for(q = 0; q < pixels; q++){		//for each pixel
		//decision
		decision = 0;
		for(b = 1; b < numClasses; b++){
			if(pixelProbs[q * numClasses + b] > pixelProbs[q * numClasses + decision]){
				decision = b;
			}
		}
		
		pixelClasses[q] = decision + 1;
	}
}

