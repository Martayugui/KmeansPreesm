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

static FILE *p;

void kmeansfunction(int rows, int columns, int bands,int pixels,struct parameters * par, float * image,  float * error,float * clusterCentroids, int * assignedCluster) {

	int nIter = 0;
	assignedCluster[pixels];
	float previousCentroids[24*bands];
	float centroidDistances[24];

		/* iterative process: the analysis stops when error is below a minimum threshold or after a maximum number of iteration */
	while ( nIter < 3) {
		//*error > par->minErr &&

		memcpy(previousCentroids, clusterCentroids, 24*bands * sizeof(float));
		nIter++;
		computeDistance(assignedCluster, centroidDistances, pixels,bands, clusterCentroids, image, 24);
		updateClusterCentroids(clusterCentroids, assignedCluster,pixels,bands, image, 24);
		//computeError(clusterCentroids, previousCentroids, par, bands,&error);

		if (par->verbose == 1) {
			//printf("error: %f\t minErr:%f\t nIter:%d\t maxIter: %d\n", error, par->minErr, nIter, par->maxIter);
		}
	}
}
/* assignes an initial value to the centroids: k (number of clusters, one of the parameters) random non-repeated numbers are extracted as indices;
pixels of preProcessedImage corresponding to these indices are the first centroids of the clusters */
void initializeCluster(int pixels, int bands, float * image,struct parameters * par,float * clusterCentroids) {

	/*p = fopen(clustervalues, "r");
	if (!p) {
		perror("File opening failed");
		exit(0);
	}
	rewind(p);
	for (int z = 0; z < k; z++) {
			for (int j = 0; j < bands; j++) {
					fscanf(p,"%f",&clusterCentroids[bands*z + j]);
			}
	}

	for (int z = 0; z < k; z++) {
				for (int j = 0; j < bands; j++) {
						printf("%f",clusterCentroids[bands*z + j]);
				}
		}
	printf("\n");

fclose(p);*/
	int indicesCentroids[24];
    printf("value1: %d",24);
		srand(time(NULL));
		int nsost;
		for (int i = 0; i < 24; i++) {
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

		for (int z = 0; z < 24; z++) {
			for (int i = 0; i < pixels; i++) {
				for (int j = 0; j < bands; j++) {
					if (indicesCentroids[z]-1 == i) {
						clusterCentroids[bands*z + j] = image[bands*i + j];
					}
				}
			}
		}

//Testing clusterCentroids

	/*printf("\n");
	printf("initialization function");
	printf("\n");
	for (int z = 0; z < 24; z++) {
				for (int j = 0; j < bands; j++) {
						printf("%f",clusterCentroids[bands*z + j]);
				}
		}
	printf("\n");*/


}

/* calculates error between current and previous centroids:
error is mean of the absolute value of the member-to-member difference of the two vectors */

void computeErrorInitialization(int c,struct parameters * par,float * clusterCentroids,float * error) {


/*	for (int z = 0; z < par->k; z++) {
					for (int j = 0; j < c; j++) {
							printf("%f",clusterCentroids[c*z + j]);
					}
			}
		printf("\n");*/

	/*printf("value2:%d\n",par->k);
	printf("pepe");
	printf("%f\n",par->minErr);*/
	/*float previousCentroids[24*c];
	float  err=0;
	error=&err;*/
	float s = 0;
	float previousCentroids[24*c];
	memset(previousCentroids, 0, 24*c * sizeof(float));
	for (int i = 0; i < 24 ; i++) {
		for (int j = 0; j < c; j++) {
			s += fabs(previousCentroids[c*i + j] - clusterCentroids[c*i + j]);
		}
	}

	*error= s / (24*c);

	printf("\n");
	printf("%f",*error);
	printf("\n");


	/*//Testing clusterCentroids
		printf("\n");
		printf("Compute error function");
		printf("\n");
		for (int z = 0; z < 24; z++) {
					for (int j = 0; j < c; j++) {
							printf("%f",clusterCentroids[c*z + j]);
					}
			}
		printf("\n");*/


}

/*void computeError(float *clusterCentroids, float *previousCentroids,struct parameters *par, int c,float *error) {

	float s = 0;
	for (int i = 0; i < par->k ; i++) {
		for (int j = 0; j < c; j++) {
			s += fabs(previousCentroids[c*i + j] - clusterCentroids[c*i + j]);
		}
	}
	*error = s / (par->k*c);
}*/


/* calculates the distance between each pixel and the centroids of the clusters: every pixel is assigned to the cluster to which it is closest;
 as a function of distance is used the spectral angle;
 centroiDistances is filled with the calculate distances between every pixel and every centroid;
 assignedCluster is filled  with the number of cluster to which each pixel is assigned */
void computeDistance(int *assignedCluster, float * centroidDistances, int pixels, int bands, float * clusterCentroids, float * image, int k) {


	printf("\n");
	printf("Compute distance function");
	printf("\n");
	for (int z = 0; z < 24; z++) {
				for (int j = 0; j < bands; j++) {
						printf("%f",clusterCentroids[bands*z + j]);
				}
		}
	printf("\n");

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
	  printf("\n");
	printf("hola");
	  printf("\n");
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
void updateClusterCentroids(float * clusterCentroids, int * assignedCluster, int pixels,int bands, float * image, int k) {
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
	printf("hola");
}




