/*
	============================================================================
	Name        : utils.h
	Author      : rlazcano and dmadronal
	Version     : 1.0
	Copyright   : CECILL-C ?
	Description : Useful functions for the hyperspectral image processing chain
	============================================================================
*/

#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "preesm.h"

/**
* Transpose a matrix.
*
* @param matrixRows
*        The rows of the matrix to be transposed
* @param matrixColumns
*        The columns of the matrix to be transposed
* @param matrix
*        The matrix to be transposed
* @param matrixTransposed 
*        The transposed matrix
*/
void transposeMatrix(int matrixRows, int matrixColumns, IN float *matrix, OUT float *matrixTransposed);

/**
* Associate a class to a pixel based on the probabilities previously obtained.
*
* @param pixels
*        The number of pixels of the hyperspectral image
* @param numClasses
*        The number of classes to distinguish when classifying
* @param pixelProbs
*        Probabilities obtained when classifying the image
* @param pixelClasses
*        Classes obtained when classifying the image
*/
void decisionMaking(int pixels, int numClasses, IN float *pixelProbs, OUT char *pixelClasses);

#endif
