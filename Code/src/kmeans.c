/*
	============================================================================
	Name        : kmeans.c
	Author      : Marta Rguez
	Version     : 1.0
	Copyright   : CECILL-C ?
	Description : Code to apply Kmeans in the hyperspectral image processing chain
	============================================================================
*/


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "kmeans.h"
#include "read_par_and_img.h"

void kmeansfunction(int rows, int columns, int bands,int pixels, float *image, struct parameters *par,int *assignedCluster){


	float error=0;
	int nIter = 0;
	assignedCluster[pixels];
	float clusterCentroids[par->k* bands];
	float previousCentroids[par->k*bands];
	memset(previousCentroids, 0, par->k*bands * sizeof(float));
	initializeCluster(clusterCentroids, par->k,pixels,bands, image);
	error=computeError(clusterCentroids, previousCentroids,par->k, bands);

/*	for (int i = 0; i<pixels; i++) {
			for (int j = 0; j < bands; j++) {
				printf("%f", image[bands*i + j]);
			}
		}*/


	float centroidDistances[par->k];

		/* iterative process: the analysis stops when error is below a minimum threshold or after a maximum number of iteration */
	while (error > par->minErr && nIter < par->maxIter) {

		memcpy(previousCentroids, clusterCentroids, par->k*bands * sizeof(float));
		nIter++;

		computeDistance(assignedCluster, centroidDistances, pixels,bands, clusterCentroids, image, par->k);
		updateClusterCentroids(clusterCentroids, assignedCluster,pixels,bands, image, par->k);
		error = computeError(clusterCentroids, previousCentroids, par->k, bands);

		if (par->verbose == 1) {
			printf("error: %f\t minErr:%f\t nIter:%d\t maxIter: %d\n", error, par->minErr, nIter, par->maxIter);
		}
	}

}
/* assignes an initial value to the centroids: k (number of clusters, one of the parameters) random non-repeated numbers are extracted as indices;
pixels of preProcessedImage corresponding to these indices are the first centroids of the clusters */
void initializeCluster(float *clusterCentroids, int k, int pixels, int bands, float *image) {

	int indicesCentroids[k];

	srand(time(NULL));
	int nsost;
	for (int i = 0; i < k; i++) {
		indicesCentroids[i] = rand() % pixels + 1;
		if (i > 0){
			do {
				nsost = 0;
				for (int j = i - 1; j >= 0; j--) {
					if (indicesCentroids[i] == indicesCentroids[j]) {
						indicesCentroids[i] = rand()% pixels+1;
						nsost=1;
						break;
					}
				}
			}while (nsost == 1);
		}
	}

	for (int z = 0; z < k; z++) {
		for (int i = 0; i < pixels; i++) {
			for (int j = 0; j < bands; j++) {
				if (indicesCentroids[z]-1 == i) {
					clusterCentroids[bands*z + j] = image[bands*i + j];
				}
			}
		}
	}
}

/* calculates error between current and previous centroids:
error is mean of the absolute value of the member-to-member difference of the two vectors */
float computeError(float *clusterCentroids, float *previousCentroids, int r, int c) {
	float error;
	float s = 0;
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			s += fabs(previousCentroids[c*i + j] - clusterCentroids[c*i + j]);
		}
	}
	error = s / (r*c);

	return error;
}

/* calculates the distance between each pixel and the centroids of the clusters: every pixel is assigned to the cluster to which it is closest;
 as a function of distance is used the spectral angle;
 centroiDistances is filled with the calculate distances between every pixel and every centroid;
 assignedCluster is filled  with the number of cluster to which each pixel is assigned */
void computeDistance(int *assignedCluster, float *centroidDistances, int pixels, int bands, float *clusterCentroids, float *image, int k) {

	for (int i = 0; i < pixels; i++) {
		for (int j = 0; j < k; j++) {
			float p = 0;
			float n1 = 0;
			float n2 = 0;
			for (int z = 0; z < bands; z++) {
				p += clusterCentroids[bands*j + z] * image[bands*i + z];
				n1 += powf(clusterCentroids[bands*j + z], 2);
				n2 += powf(image[bands*i + z], 2);
			}
			centroidDistances[j] = (acosf(p / (sqrtf(n1)*sqrtf(n2))))*180.0/3.14;
		}
		assignedCluster[i] = findMinimum(centroidDistances, k)+1;
	}
}

/* finds the minimum index of each line of centroidDistances, that is the cluster whose centroid is at the shortest distance from the pixel under examination */
int findMinimum(float *vet, int size) {
	int imin=0;
	float min = vet[0];
	for (int i = 1; i < size; i++) {
		if (vet[i] < min) {
			min = vet[i];
			imin = i;
		}
	}
	return imin;
}

/* updates the centroids: they are calculated as mean of the elements of the cluster*/
void updateClusterCentroids(float *clusterCentroids, int *assignedCluster, int pixels,int bands, float *image, int k) {
	int n;
	float sumperb;
	for (int z = 0; z < k; z++) {
		for (int j = 0; j < bands; j++) {
			sumperb = 0;
			n = 0;
			for (int i = 0; i < pixels; i++) {
				if (assignedCluster[i] == z+1) {
					n++;
					sumperb += image[bands*i + j];
				}
			}
			if (n == 0) {
				clusterCentroids[bands*z + j] = 0;
			}
			else {
				clusterCentroids[bands*z + j] = sumperb / n;
			}
		}
	}
}




