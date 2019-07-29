/*
	============================================================================
	Name        : svm.h
	Author      : rlazcano and dmadronal
	Version     : 1.0
	Copyright   : CECILL-C ?
	Description : Code to apply Support Vector Machines classification in the hyperspectral image processing chain
	============================================================================
*/

#ifndef SVM_H
#define SVM_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "preesm.h"
#include "utils.h"

#define PATHW PROJECT_ROOT_PATH "/dat/w_vector.bin"
#define PATHRHO PROJECT_ROOT_PATH "/dat/rho.bin"
#define PATHLABEL PROJECT_ROOT_PATH "/dat/label.bin"
#define PATHPROBA PROJECT_ROOT_PATH "/dat/ProbA.bin"
#define PATHPROBB PROJECT_ROOT_PATH "/dat/ProbB.bin"

/**
* Initialize the reading of the SVM model actor.
*/
void initReadSvmModel();

/**
* Read the SVM model.
*
* @param bands
*        The bands of the hyperspectral image
* @param numClasses
*        The number of classes to distinguish when classifying
* @param wVector
*        Vectors defining the different hyperplanes
* @param rho
*        Bias of the different hyperplanes
* @param label
*        Names of the different classes of the classification
* @param probA
*        First parameter to compute probabilities when classifying
* @param probB
*        Second parameter to compute probabilities when classifying
*/
void readSvmModel(int bands, int numClasses, OUT float *wVector, OUT float *rho, OUT int *label, OUT float *probA, OUT float *probB);

/**
* Extract the pairwise probabilities of a set of pixels.
*
* @param pixels
*        The number of pixels of the hyperspectral image
* @param bands
*        The bands of the hyperspectral image
* @param numClasses
*        The number of classes to distinguish when classifying
* @param image
*        Image to be classified
* @param wVector
*        Vectors defining the different hyperplanes
* @param rho
*        Bias of the different hyperplanes
* @param probA
*        First parameter to compute probabilities when classifying
* @param probB
*        Second parameter to compute probabilities when classifying
* @param pairwiseProb
*        Binary probabilities of each pixel associated to each binary classifier
*/
void computeSvmPairwiseProbs(int pixels, int bands, int numClasses, IN float *image, IN float *wVector, IN float *rho, IN float *probA, IN float *probB, OUT float *pairwiseProb);

/**
* Compute the probabilities when considering all the classes at the same time 
* taking as an input the pairwise probabilities.
*
* @param pixels
*        The number of pixels of the hyperspectral image
* @param bands
*        The bands of the hyperspectral image
* @param numClasses
*        The number of classes to distinguish when classifying
* @param label
*        Names of the different classes of the classification
* @param pairwiseProb
*        Binary probabilities of each pixel associated to each binary classifier
* @param pixelProbs
*        Probabilities obtained when classifying the pixels
*/
void minimizationSolver(int pixels, int bands, int numClasses, int *label, IN float *pairwiseProb, OUT float *pixelProbs);

/**
* Classify the image with the SVM model.
*
* @param pixels
*        The number of pixels of the hyperspectral image
* @param bands
*        The bands of the hyperspectral image
* @param numClasses
*        The number of classes to distinguish when classifying
* @param image
*        Image to be classified
* @param wVector
*        Vectors defining the different hyperplanes
* @param rho
*        Bias of the different hyperplanes
* @param label
*        Names of the different classes of the classification
* @param probA
*        First parameter to compute probabilities when classifying
* @param probB
*        Second parameter to compute probabilities when classifying
* @param svmProbs
*        Probabilities obtained when classifying the image
* @param svmClasses
*        Classes obtained when classifying the image
*/
void svm(int pixels, int bands, int numClasses, IN float *image, IN float *wVector, IN float *rho, IN int *label, IN float *probA, IN float *probB, OUT float *svmProbs, OUT char *svmClasses);

#endif
