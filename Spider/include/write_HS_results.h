/*
	============================================================================
	Name        : write_HS_results.h
	Author      : rlazcano
	Version     :
	Copyright   :
	Description : Save KNN results
	============================================================================
*/
#ifndef SAVE_HS_RESULTS
#define SAVE_HS_RESULTS

#include "preesm.h"

#define PATHOUT_PCA_BIN PROJECT_ROOT_PATH "/dat/pcaOutput.bin"
#define PATHOUT_SVM_PROBS_BIN PROJECT_ROOT_PATH "/dat/svmProbs.bin"
#define PATHOUT_SVM_CLASSES_BIN PROJECT_ROOT_PATH "/dat/svmClasses.bin"
#define PATHOUT_KNN_PROBS_BIN PROJECT_ROOT_PATH "/dat/knnProbs.bin"
#define PATHOUT_KNN_CLASSES_BIN PROJECT_ROOT_PATH "/dat/knnClasses.bin"

#define PATHOUT_PCA_TXT PROJECT_ROOT_PATH "/dat/pcaOutput.txt"
#define PATHOUT_SVM_PROBS_TXT PROJECT_ROOT_PATH "/dat/svmProbs.txt"
#define PATHOUT_SVM_CLASSES_TXT PROJECT_ROOT_PATH "/dat/svmClasses.txt"
#define PATHOUT_KNN_PROBS_TXT PROJECT_ROOT_PATH "/dat/knnProbs.txt"
#define PATHOUT_KNN_CLASSES_TXT PROJECT_ROOT_PATH "/dat/knnClasses.txt"

/**
* Functions called to save the results.
*
* @param rows
*        the rows of the image
* @param columns
*        the columns of the image
* @param classes
*        Number of classes
* @param pcaOutput
*        1 band representation
* @param svmProbs
*        Probabilities computed by SVM
* @param svmClasses
*        Class map computed by SVM
* @param knnProbs
*        Probabilities computed by KNN
* @param knnClasses
*        Class map computed by SVM
*/
void saveResultsBin(int rows, int columns, int numClasses, IN double *pcaOutput, IN float *svmProbs, IN char *svmClasses, IN float *knnProbs, IN char *knnClasses);

void saveResultsTxt(int rows, int columns, int numClasses, IN double *pcaOutput, IN float *svmProbs, IN char *svmClasses, IN float *knnProbs, IN char *knnClasses);

void saveResults(int rows, int columns, int numClasses, IN double *pcaOutput, IN float *svmProbs, IN char *svmClasses, IN float *knnProbs, IN char *knnClasses);

void saveResultsPcaBin(int rows, int columns, int pcs, IN double *pcaOutput);

void saveResultsPcaTxt(int rows, int columns, int pcs, IN double *pcaOutput);

void saveResultsPca(int rows, int columns, int pcs, IN double *pcaOutput);

void saveResultsSvmBin(int rows, int columns, int numClasses, IN float *svmProbs, IN char *svmClasses);

void saveResultsSvmTxt(int rows, int columns, int numClasses, IN float *svmProbs, IN char *svmClasses);

void saveResultsSvm(int rows, int columns, int numClasses, IN float *svmProbs, IN char *svmClasses);

void saveResultsKnnBin(int rows, int columns, int numClasses, IN float *knnProbs, IN char *knnClasses);

void saveResultsKnnTxt(int rows, int columns, int numClasses, IN float *knnProbs, IN char *knnClasses);

void saveResultsKnn(int rows, int columns, int numClasses, IN float *knnProbs, IN char *knnClasses);

#endif
