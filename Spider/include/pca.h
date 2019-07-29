/*
	============================================================================
	Name        : pca.h
	Author      : rlazcano and dmadronal
	Version     : 1.0
	Copyright   : CECILL-C ?
	Description : Code to apply Principal Component Analysis in the hyperspectral image processing chain
	============================================================================
*/

#ifndef PCA_H
#define PCA_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "preesm.h"
#include "utils.h"

/**
* Represent the information of the image in only PCs bands.
*
* @param rows
*        The rows of the hyperspectral image
* @param columns
*        The columns of the hyperspectral image
* @param bands
*        The bands of the hyperspectral image
* @param pcs
*        Principal Components that will be generated
* @param image
*        The hyperspectral image
* @param pcaOutput
*        The one band representation of the image
*/
void pca(int rows, int columns, int bands, int pcs, IN float *image, OUT double *pcaOutput);

/**
* Function to center an image by removing the average of each band.
*
* @param pixels
*        Number of pixels of each band
* @param bands
*        Number of bands to process
* @param image
*        The input hyperspectral image
* @param meanValue
*        The mean value of each band
*/
void bandsMeanValue(int pixels, int bands, float *image, float *meanValue);

/**
* Function to center an image by removing the average of each band.
*
* @param pixels
*        Number of pixels of each band
* @param bands
*        Number of bands to process
* @param image
*        The input hyperspectral image
* @param meanValue
*        The mean value of each band
* @param imageCentered
*        The image without the average
*/
void bandsRemoveMeanValue(int pixels, int bands, float *image, float *meanValue, float *imageCentered);

/**
* Function to center an image by removing the average of each band.
*
* @param pixels
*        Number of pixels of each band
* @param bands
*        Number of bands to process
* @param image
*        The input hyperspectral image
* @param imageCentered
*        The image without the average
*/
void imageCentering(int pixels, int bands, IN float *image, OUT float *imageCentered);

/**
* Function to divide a matrix by a value: A / value
* (A[N][M] * B[Q][M]) / (samples-1) = Cov[N][Q]
*
* @param rows
*        Number of rows of matrixIn
* @param columns
*        Number of columns of matrixIn
* @param value
*        The value to use when dividing
* @param matrixIn
*        Input matrix
* @param matrixOut
*        Output Matrix
*/
void matrixDivValue(int rows, int columns, int value, double *matrixIn, double *matrixOut);

/**
* Function to multiply matrices: A * B (transposed) row by row
* (A[N][M] * B[Q][M]) = Cov[N][Q]
*
* @param rowsMatrixA
*        Number of rows of matrix A
* @param rowsMatrixB
*        Number of rows of matrix B
* @param samples
*        Number of samples of both matrices
* @param matrixA
*        Matrix A
* @param matrixB
*        Matrix B
* @param matrixResult
*        The result of the operation
*/
void matrixMultAxBt(int rowsMatrixA, int rowsMatrixB, int samples, float *matrixA, float *matrixB, double *matrixResult);

/**
* Function to multiply matrices and normalize: A * B (transposed) row by row
* (A[N][M] * B[Q][M]) / (samples-1) = Cov[N][Q]
*
* @param rowsMatrixA
*        Number of rows of matrix A
* @param rowsMatrixB
*        Number of rows of matrix B
* @param samples
*        Number of samples of both matrices
* @param matrixA
*        Matrix A
* @param matrixB
*        Matrix B
* @param cov
*        The associated covariance matrix
*/
void covariance(int rowsMatrixA, int rowsMatrixB, int samples, IN float *matrixA, IN float *matrixB, OUT double *cov);

/**
* Function to diagonalize a matrix using Jacobi approach.
*
* @param bands
*        Number of bands to process
* @param PCs
*        Number of PCs to compute
* @param cov
*        Covariance matrix
* @param eigenvectors
*        Eigenvectors associated
*/
void jacobi(int bands, int pcs, IN double *cov, OUT double *eigenvectors);

/**
* Function to project an image on pcs eigenvectors.
*
* @param pixels
*        Number of pixels to process
* @param bands
*        Number of bands to process
* @param pcs
*        Number of PCs to compute
* @param imageCentered
*        The image without the average
* @param eigenvectors
*        Eigenvectors associated
* @param pcaOutput
*        The output of PCA algorithm
*/
void projection(int pixels, int bands, int pcs, IN float *imageCentered, IN double *eigenvectors, OUT double *pcaOutput);

#endif
