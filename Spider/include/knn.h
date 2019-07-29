/*
	============================================================================
	Name        : knn.h
	Author      : rlazcano and dmadronal
	Version     : 1.0
	Copyright   : CECILL-C ?
	Description : Code to apply K-Nearest Neighbors in the hyperspectral image processing chain
	============================================================================
*/

#ifndef KNN_H
#define KNN_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "preesm.h"
#include "utils.h"

/**
* Struct to define the different feature values of each pixel
* @value PCA_pVAL 
* 	 Spectral information of the pixel
* @value r
* 	 Spatial component 'row' of the pixel
* @value c 
* 	 Spatial component 'column' of the pixel
* @value rc 
* 	 Position (id) of the pixel in the image rc = r * columns + c
*/

typedef struct featureMatrixNode{
    float PCA_pVal;
    short r;
    short c;
    int rc;
}featureMatrixNode;

/**
* Struct to assign a distance from the pixel under comparison to pixel in rc position
* @value distance
* 	 The distance to the pixel
* @value rc 
* 	 Position of the pixel in the image rc = r * columns + c
*/
typedef struct featureDistance{
    float distance;
    int rc;
}featureDistance;

/**
* Create the spatial-spectral structure to perform the searching of the K-Nearest Neighbors and split in slices.
*
* @param rows
*        The rows of the opened hyperspectral image
* @param columns
*        The columns of the opened hyperspectral image
* @param windowSizeKnn
*        Window that will be used during the searching phase
* @param lambda
*        Value to weight up spectral and spatial contributions during searching
* @param slices
*        Number of slices to generate (parallelism)
* @param pcaOutput
*        1-band representation of the spectral information of the image
* @param featMat
*        The spatial-spectral structure with all the info
*/
void featMatCreationSplit(int rows, int columns, int windowSizeKnn, int lambda, int slices, IN double *pcaOutput, OUT featureMatrixNode *featMat);


/**
* Create the spatial-spectral structure to perform the searching of the K-Nearest Neighbors.
*
* @param rows
*        The rows of the opened hyperspectral image
* @param columns
*        The columns of the opened hyperspectral image
* @param windowSizeKnn
*        Window that will be used during the searching phase
* @param lambda
*        Value to weight up spectral and spatial contributions during searching
* @param pcaOutput
*        1-band representation of the spectral information of the image
* @param featMat
*        The spatial-spectral structure with all the info
*/
void featMatCreation(int rows, int columns, int windowSizeKnn, int lambda, IN double *pcaOutput, OUT featureMatrixNode *featMat);


/**
* Do the searching to obtain the K-Nearest Neighbors of each pixel.
*
* @param columns
*        The columns of the opened hyperspectral image
* @param neighbors
*        The number of neighbors to apply the filtering
* @param windowSizeKnn
*        Window that will be used during the searching phase
* @param featMat
*        The spatial-spectral structure with all the info
* @param knnMatrix
*        The K-Nearest Neighbors associated to each pixel 
*/
void searchKnn(int pixels, int columns, int neighbors, int windowSizeKnn, IN featureMatrixNode *featMat, OUT int *knnMatrix);


/**
* Filter the SVM probabilities doing an average of the K-Nearest Neighbors values.
*
* @param pixels
*        Total number of pixels to filter their probabilities
* @param numClasses
*        The number of classes distinguished when classifying
* @param neighbors
*        The number of neighbors to apply the filtering
* @param knnMatrix
*        The K-Nearest Neighbors associated to each pixel 
* @param svmProbs
*        SVM classification that will be filtered
* @param knnProbs
*        Probabilities obtained after filtering the SVM classification 
*/
void filterProbs(int pixels, int numClasses, int neighbors, IN int *knnMatrix, IN float *svmProbs, OUT float *knnProbs);


/**
* Filter the SVM result based on the PCA information and the spatial information
* basing the filtering on the KNN search.
*
* @param rows
*        The rows of the opened hyperspectral image
* @param columns
*        The columns of the opened hyperspectral image
* @param numClasses
*        The number of classes distinguished when classifying
* @param neighbors
*        The number of neighbors to apply the filtering
* @param windowSizeKnn
*        Window that will be used during the searching phase
* @param lambda
*        Value to weight up spectral and spatial contributions during searching
* @param pcaOutput
*        1-band representation of the spectral information of the image
* @param svmProbs
*        SVM classification that will be filtered
* @param knnProbs
*        Probabilities obtained after filtering the SVM classification 
* @param knnClasses
*        Classes obtained after filtering the SVM classification 
*/
void knn(int rows, int columns, int numClasses, int neighbors, int windowSizeKnn, int lambda, IN double *pcaOutput, IN float *svmProbs, OUT float *knnProbs, OUT char *knnClasses);

#endif
