/*
	============================================================================
	Name        : pca.c
	Author      : rlazcano and dmadronal
	Version     : 1.0
	Copyright   : CECILL-C ?
	Description : Code to apply Principal Component Analysis in the hyperspectral image processing chain
	============================================================================
*/

#include "pca.h"

/*========================================================================

   bandsMeanValue DEFINITION

   ======================================================================*/
void bandsMeanValue(int pixels, int bands, float *image, float *meanValue){

	int i = 0;
	int j = 0;

	// Mean value computation
	
	for(i = 0; i < bands; i++){
		meanValue[i] = 0.0;
		for (j = 0; j < pixels; j++){
			meanValue[i] = meanValue[i] + image[i * pixels + j];
		}
		meanValue[i] = meanValue[i] / pixels;
	}
}

/*========================================================================

   bandsRemoveMeanValue DEFINITION

   ======================================================================*/
void bandsRemoveMeanValue(int pixels, int bands, float *image, float *meanValue, float *imageCentered){

	int i = 0;
	int j = 0;

	// Remove mean value
	
	for(i = 0; i < bands; i++){
		for (j = 0; j < pixels; j++){
			imageCentered[i * pixels + j] = image[i * pixels + j] - meanValue[i];
		}
	}
}

/*========================================================================

   imageCentering DEFINITION

   ======================================================================*/
void imageCentering(int pixels, int bands, float *image, float *imageCentered){

	int i = 0;
	int j = 0;
	float meanValue = 0.0;
	// Mean value computation and removal
	for(i = 0; i < bands; i++){
		meanValue = 0.0;
		for (j = 0; j < pixels; j++){
			meanValue = meanValue + image[i * pixels + j];
		}
		meanValue = meanValue / pixels;
		for (j = 0; j < pixels; j++){
			imageCentered[i * pixels + j] = image[i * pixels + j] - meanValue;
		}
	}
}

/*========================================================================

   matrixDivValue DEFINITION

   ======================================================================*/
void matrixDivValue(int rows, int columns, int value, double *matrixIn, double *matrixOut){

	// matrixDivValue
	int i = 0;
	int j = 0;

	for(i = 0; i < rows; i++){
		for (j = 0; j < columns; j++){
			matrixOut[i * columns + j] = matrixIn[i * columns + j] / (value);
		}
	}
}

/*========================================================================

   matrixMultAxBt DEFINITION

   ======================================================================*/
void matrixMultAxBt(int rowsMatrixA, int rowsMatrixB, int samples, float *matrixA, float *matrixB, double *matrixResult){

	// mult matrix A by B transposed
	int i = 0;
	int j = 0;
	int k = 0;

	for(i = 0; i < rowsMatrixA; i++){
		for (j = 0; j < rowsMatrixB; j++){
			matrixResult[i * rowsMatrixB + j] = 0.0;
			for (k = 0; k < samples; k++){
				matrixResult[i * rowsMatrixB + j] = matrixResult[i * rowsMatrixB + j] + (double)matrixA[i * samples + k] * (double)matrixB[j * samples + k];
			}
		}
	}
}

/*========================================================================

   covariance DEFINITION

   ======================================================================*/
void covariance(int rowsMatrixA, int rowsMatrixB, int samples, float *matrixA, float *matrixB, double *cov){

	// Covariance
	int i = 0;
	int j = 0;
	int k = 0;

	for(i = 0; i < rowsMatrixA; i++){
		for (j = 0; j < rowsMatrixB; j++){
			cov[i * rowsMatrixB + j] = 0.0;
			for (k = 0; k < samples; k++){
				cov[i * rowsMatrixB + j] = cov[i * rowsMatrixB + j] + (double)matrixA[i * samples + k] * (double)matrixB[j * samples + k];
			}
			cov[i * rowsMatrixB + j] = cov[i * rowsMatrixB + j] / (samples - 1);
		}
	}
}

/*========================================================================

   jacobi DEFINITION

   ======================================================================*/
void jacobi(int bands, int pcs, double *cov, double *eigenvectors){
	// Jacobi
	float *matrixP = (float *)malloc(bands * bands * sizeof(float));
	double *eigenvals = (double *)malloc(bands * sizeof(double));
	int *vectorOrderEigenvals = (int *)malloc(bands * sizeof(int));
	volatile int i = 0;
	volatile int j = 0;
	volatile int k = 0;
	int endOp = 0;
	int valuesProcessed = 0;
	double sumDiag = 0.0;
	double stop = 0.0;
	double epsilon = 0.00001;
	double a_ij = 0.0;
	double a_ii = 0.0;
	double a_jj = 0.0;
	double alpha = 0.0;
	double cosA = 0.0;
	double sinA = 0.0;
	double firstValueChanged = 0.0;
	double SecondValueChanged = 0.0;
	int ordered = 0;
	double orderingValue = 0.0;
	int orderingValueInt = 0;

	for(i = 0; i < bands; i++){
		for(j = 0; j < bands; j++){
			if( i == j){
				matrixP[i * bands + j] = 1.0;
			}else{
				matrixP[i * bands + j] = 0.0;			
			}		
		}	
	}

	while (endOp == 0){
		endOp = 1;
		sumDiag = 0.0;
		for(i = 0; i < bands; i++){
			sumDiag = sumDiag + cov[i * bands + i];		
		}
		for(i = 0; i < bands; i++){
			for(j = (i + 1); j < bands; j++){
				stop = epsilon * sumDiag;
				if(cov[i * bands + j] > stop || cov[i * bands + j] < -stop){
					valuesProcessed++;
					endOp = 0;
					a_ij = cov[i * bands + j];
					a_ii = cov[i * bands + i];
					a_jj = cov[j * bands + j];
					
					alpha = a_ij / (a_jj - a_ii);

					cosA = 1 / (sqrt(alpha * alpha + 1));
					sinA = cosA * alpha;	
					for(k = 0; k < bands; k++){
						firstValueChanged = cosA * cov[i * bands + k] - sinA * cov[j * bands + k];
						SecondValueChanged = sinA * cov[i * bands + k] + cosA * cov[j * bands + k];
						cov[i * bands + k] = firstValueChanged;
						cov[j * bands + k] = SecondValueChanged;
					}
					for(k = 0; k < bands; k++){
						firstValueChanged = cov[k * bands + i] * cosA - cov[k * bands + j] * sinA;
						SecondValueChanged = cov[k * bands + i] * sinA + cov[k * bands + j] * cosA;
						cov[k * bands + i] = firstValueChanged;
						cov[k * bands + j] = SecondValueChanged;

						firstValueChanged = matrixP[k * bands + i] * cosA - matrixP[k * bands + j] * sinA;
						SecondValueChanged = matrixP[k * bands + i] * sinA + matrixP[k * bands + j] * cosA;
						matrixP[k * bands + i] = firstValueChanged;
						matrixP[k * bands + j] = SecondValueChanged;
					}
				}
			}
		}
	}
	
	for(i = 0; i < bands; i++){
		eigenvals[i] = cov[i * bands + i];
		vectorOrderEigenvals[i] = i;
	}
	
	ordered = 0;

	while(ordered == 0){
		ordered = 1;
		for(j = 1; j < bands; j++){
			if (eigenvals[j] > eigenvals[j - 1]){
				orderingValue = eigenvals[j];
				eigenvals[j] = eigenvals[j - 1];
				eigenvals[j - 1] = orderingValue;

				orderingValueInt = vectorOrderEigenvals[j];
				vectorOrderEigenvals[j] = vectorOrderEigenvals[j - 1];
				vectorOrderEigenvals[j - 1] = orderingValueInt;
				ordered = 0;
			}		
		}	
	}

	for(i = 0; i < pcs; i++){
		for(j = 0; j < bands; j++){
			eigenvectors[j * pcs + i] = matrixP[j * bands + vectorOrderEigenvals[i]];
		}
	}
    free(matrixP);
    free(eigenvals);
    free(vectorOrderEigenvals);
}

/*========================================================================

   projection DEFINITION

   ======================================================================*/
void projection(int pixels, int bands, int pcs, float *imageCentered, double *eigenvectors, double *pcaOutput){
	// Projection
	int i = 0;
	int j = 0;
	int k = 0;
	
	for(i = 0; i < pixels; i++){
		for(j = 0; j < pcs; j++){
			pcaOutput[i * pcs + j] = 0;
			for(k = 0; k < bands; k++){
				pcaOutput[i * pcs + j] = pcaOutput[i * pcs + j] + (imageCentered[i * bands + k] * eigenvectors[k * pcs + j]);
			}
		}
	}
}

/*========================================================================

   pca DEFINITION

   ======================================================================*/
void pca(int rows, int columns, int bands, int pcs, float *image, double *pcaOutput) {

	// Image centering

	int pixels = rows * columns;
	
	float *imageCentered = (float *)malloc(rows * columns * bands * sizeof(float));

	imageCentering(pixels, bands, image, imageCentered);

	// Covariance

	double *cov = (double *)malloc(bands * bands * sizeof(double));

	covariance(bands, bands, pixels, imageCentered, imageCentered, cov);

	// Jacobi

	double *eigenvectors = (double *)malloc(bands * pcs * sizeof(double));

	jacobi(bands, pcs, cov, eigenvectors);

	// imageCentered needs to be transposed before projecting

	float *imageCenteredTransposed = (float *)malloc(rows * columns * bands * sizeof(float));
	transposeMatrix(bands, pixels, imageCentered, imageCenteredTransposed);

	// Projection
		
	projection(pixels, bands, pcs, imageCenteredTransposed, eigenvectors, pcaOutput);

}
