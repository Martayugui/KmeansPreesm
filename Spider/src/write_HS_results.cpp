/*
	============================================================================
	Name        : write_HS_results.c
	Author      : rlazcano
	Version     :
	Copyright   : CECILL-C
	Description : Save KNN results
	============================================================================
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "write_HS_results.h"

static FILE *ptfile ;
static FILE *fp;

void saveResultsBin(int rows, int columns, int numClasses, double *pcaOutput, float *svmProbs, char *svmClasses, float *knnProbs, char *knnClasses)
{
	saveResultsPcaBin(rows, columns, numClasses, pcaOutput);
	saveResultsSvmBin(rows, columns, numClasses, svmProbs, svmClasses);
	saveResultsKnnBin(rows, columns, numClasses, knnProbs, knnClasses);
}

void saveResultsTxt(int rows, int columns, int numClasses, double *pcaOutput, float *svmProbs, char *svmClasses, float *knnProbs, char *knnClasses)
{
	saveResultsPcaTxt(rows, columns, numClasses, pcaOutput);
	saveResultsSvmTxt(rows, columns, numClasses, svmProbs, svmClasses);
	saveResultsKnnTxt(rows, columns, numClasses, knnProbs, knnClasses);	
}

void saveResults(int rows, int columns, int numClasses, double *pcaOutput, float *svmProbs, char *svmClasses, float *knnProbs, char *knnClasses)
{
	saveResultsPcaBin(rows, columns, numClasses, pcaOutput);
	saveResultsPcaTxt(rows, columns, numClasses, pcaOutput);
	saveResultsSvmBin(rows, columns, numClasses, svmProbs, svmClasses);
	saveResultsSvmTxt(rows, columns, numClasses, svmProbs, svmClasses);
	saveResultsKnnBin(rows, columns, numClasses, knnProbs, knnClasses);
	saveResultsKnnTxt(rows, columns, numClasses, knnProbs, knnClasses);	
}

void saveResultsPcaBin(int rows, int columns, int pcs, double *pcaOutput)
{
	ptfile = fopen(PATHOUT_PCA_BIN, "wb");
    	fwrite(pcaOutput, sizeof(double), rows * columns * pcs, ptfile);
    	fclose(ptfile);
}

void saveResultsPcaTxt(int rows, int columns, int pcs, double *pcaOutput)
{
	int i;

	fp = fopen(PATHOUT_PCA_TXT,"w"); 
	for(i = 0; i < rows * columns * pcs; i++){
		fprintf(fp,"%f\n",pcaOutput[i]);
	}			
	fclose(fp);
}

void saveResultsPcaTxt2(int rows, int columns, int pcs, double *pcaOutput)
{
	int i;

	fp = fopen("../../dat/cov.txt","w"); 
	for(i = 0; i < rows * columns * pcs; i++){
		fprintf(fp,"%f\n",pcaOutput[i]);
	}			
	fclose(fp);
}

void saveResultsPca(int rows, int columns, int pcs, double *pcaOutput)
{
	saveResultsPcaBin(rows, columns, pcs, pcaOutput);
	saveResultsPcaTxt(rows, columns, pcs, pcaOutput);	
}

void saveResultsSvmBin(int rows, int columns, int numClasses, float *svmProbs, char *svmClasses)
{
	ptfile = fopen(PATHOUT_SVM_PROBS_BIN, "wb");
    	fwrite(svmProbs, sizeof(float), rows * columns * numClasses, ptfile);
    	fclose(ptfile);
	ptfile = fopen(PATHOUT_SVM_CLASSES_BIN, "wb");
    	fwrite(svmClasses, sizeof(char), rows * columns, ptfile);
    	fclose(ptfile);
}

void saveResultsSvmTxt(int rows, int columns, int numClasses, float *svmProbs, char *svmClasses)
{
	int i;

	fp = fopen(PATHOUT_SVM_PROBS_TXT,"w"); 
	for(i = 0; i < rows*columns*numClasses; i++){
		fprintf(fp,"%f\n",svmProbs[i]);
	}			
	fclose(fp);
	fp = fopen(PATHOUT_SVM_CLASSES_TXT,"w"); 
	for(i = 0; i < rows*columns; i++){
		fprintf(fp,"%d\n",svmClasses[i]);
	}			
	fclose(fp);
}

void saveResultsSvm(int rows, int columns, int numClasses, float *svmProbs, char *svmClasses)
{
	saveResultsSvmBin(rows, columns, numClasses, svmProbs, svmClasses);
	saveResultsSvmTxt(rows, columns, numClasses, svmProbs, svmClasses);	
}

void saveResultsKnnBin(int rows, int columns, int numClasses, float *knnProbs, char *knnClasses)
{
	ptfile = fopen(PATHOUT_KNN_PROBS_BIN, "wb");
    	fwrite(knnProbs, sizeof(float), rows * columns * numClasses, ptfile);
    	fclose(ptfile);
	ptfile = fopen(PATHOUT_KNN_CLASSES_BIN, "wb");
    	fwrite(knnClasses, sizeof(char), rows * columns, ptfile);
    	fclose(ptfile);
}

void saveResultsKnnTxt(int rows, int columns, int numClasses, float *knnProbs, char *knnClasses)
{
	int i;

	fp = fopen(PATHOUT_KNN_PROBS_TXT,"w"); 
	for(i = 0; i < rows*columns*numClasses; i++){
		fprintf(fp,"%f\n",knnProbs[i]);
	}			
	fclose(fp);
	fp = fopen(PATHOUT_KNN_CLASSES_TXT,"w"); 
	for(i = 0; i < rows*columns; i++){
		fprintf(fp,"%d\n",knnClasses[i]);
	}			
	fclose(fp);
}

void saveResultsKnn(int rows, int columns, int numClasses, float *knnProbs, char *knnClasses)
{
	saveResultsKnnBin(rows, columns, numClasses, knnProbs, knnClasses);
	saveResultsKnnTxt(rows, columns, numClasses, knnProbs, knnClasses);	
}

