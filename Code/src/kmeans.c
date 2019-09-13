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

void kmeansfunction(int rows, int columns, int bands,int pixels,struct parameters * par, float * image,  float * error,float * clusterCentroids, int * assignedCluster) {

	//int nIter = 0;
	//float previousCentroids[par->k*bands];
		/* iterative process: the analysis stops when error is below a minimum threshold or after a maximum number of iteration */
	//while (*error > par->minErr && nIter < par->maxIter) {


		//memcpy(previousCentroids, clusterCentroids, par->k*bands * sizeof(float));
		//nIter++;
		//computeDistance(assignedCluster, centroidDistances, pixels,bands, clusterCentroids, image, par->k);
		//updateClusterCentroids(clusterCentroids, assignedCluster,pixels,bands, image, par->k);
		//computeError(clusterCentroids, previousCentroids, par, bands,error);

		//if (par->verbose == 1) {
			//printf("error: %f\t minErr:%f\t nIter:%d\t maxIter: %d\n", error, par->minErr, nIter, par->maxIter);
		//}
	//}
}
/* assignes an initial value to the centroids: k (number of clusters, one of the parameters) random non-repeated numbers are extracted as indices;
pixels of preProcessedImage corresponding to these indices are the first centroids of the clusters */
void initializeCluster(int pixels, int bands, float * image,struct parameters * par,float *initialCentroids) {
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

	    int indicesCentroids[par->k];
	    initialCentroids[par->k*bands];
	    image[pixels*bands];
		srand(time(NULL));
		int nsost;
		for (int i = 0; i < par->k; i++) {
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

		for (int z = 0; z < par->k; z++) {
			for (int i = 0; i < pixels; i++) {
				for (int j = 0; j < bands; j++) {
					if (indicesCentroids[z]-1 == i) {
						initialCentroids[bands*z + j] = image[bands*i + j];
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

void computeError( int c,char *N,float *initialCentroids,float *clusterCentroids,struct parameters *par,float *error,float *previousCentroids,float *p,int *iterin,int *iterout)  {

	initialCentroids[par->k*bands];
	clusterCentroids[par->k*bands];
	previousCentroids[par->k*bands];
	p[par->k*bands];
	//Number of iterations
	*iterout=*iterin + 1;
	printf("\n");
    printf("iter:%d",*iterout);
    printf("\n");
	float s = 0;
	//First iteration of error
    if (*iterout==1)  {
    	memset(previousCentroids, 0, par->k*c * sizeof(float));
		 for (int z = 0; z < par->k; z++) {
			 for (int j = 0; j < c; j++) {
				clusterCentroids[c*z + j]=initialCentroids[c*z + j];

			}
		 }

		 for (int i = 0; i < par->k ; i++) {
				for (int j = 0; j < c; j++) {
					s += fabs(previousCentroids[c*i + j] - clusterCentroids[c*i + j]);
				}
		 }


		 *error= s / (par->k*c);
		  memcpy(previousCentroids, clusterCentroids, par->k*c * sizeof(float));

    }
    else {

			memset(previousCentroids,p, par->k*c * sizeof(float));
			 for (int i = 0; i < par->k ; i++) {
					for (int j = 0; j < c; j++) {
						s += fabs(previousCentroids[c*i + j] - clusterCentroids[c*i + j]);
					}
			 }


			 *error= s / (par->k*c);

		memcpy(previousCentroids, clusterCentroids, par->k*c * sizeof(float));

    }

}


/* calculates the distance between each pixel and the centroids of the clusters: every pixel is assigned to the cluster to which it is closest;
 as a function of distance is used the spectral angle;
 centroiDistances is filled with the calculate distances between every pixel and every centroid;
 assignedCluster is filled  with the number of cluster to which each pixel is assigned */
void computeDistance(int rows,int columns,int pixels,int bands,float * image,struct parameters *par, float * clusterCentroids,int *assignedCluster,float *error,float *p2,int *iterin,int *iterout,int *assignedClusterIn) {

	image[pixels*bands];
	clusterCentroids[par->k*bands];
	assignedCluster[rows*columns];
	assignedClusterIn[rows*columns];
	p2[par->k*bands];
	printf("\n");
	printf("error: %f",*error);
    printf("\n");

    *iterout=*iterin;
	memset(p2, clusterCentroids, par->k*bands * sizeof(float));


	if (*error > par->minErr) {

		float centroidDistances[pixels];
		for (int i = 0; i < pixels; i++) {
					for (int j = 0; j < par->k; j++) {
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
					assignedCluster[i] = findMinimum(centroidDistances, par->k)+1;

		}
	 }
	 else {
		 for (int p = 0; p < rows; p++) {
		 		 		for (int q = 0; q < columns; q++) {
		 		 			 assignedCluster[rows*q+p]=assignedClusterIn[rows*q+p];
		 		 		}
		 		  }

	 }

	 if (*iterin==1)  {
		 for (int p = 0; p < rows; p++) {
		 		for (int q = 0; q < columns; q++) {
		 			 assignedClusterIn[rows*q+p]=assignedCluster[rows*q+p];
		 		}
		  }
	  }


	for (int p = 0; p < rows; p++) {
			for (int q = 0; q < columns; q++) {
			  if (p==0){
				 printf("%d\t", assignedCluster[rows*q+p]);
			  }
			}
		}


	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	for (int p = 0; p < rows; p++) {
		for (int q = 0; q < columns; q++) {
		  if (p==0){
			 printf("%d\t", assignedClusterIn[rows*q+p]);
		  }
		}
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
void updateClusterCentroids( int pixels,int bands,int rows,int columns, int * assignedCluster,struct parameters *par,float *image,float *error,float * clusterCentroids,float *p,float *p2,int *iterin,int *iterout) {

assignedCluster[rows*columns];
image[pixels*bands];
clusterCentroids[par->k*bands];
p[par->k*bands];
p2[par->k*bands];
*iterout=*iterin;
memset(p, p2, par->k*bands * sizeof(float));
int n;
  if (*error > par->minErr) {
		float sumperb;
		for (int z = 0; z < par->k; z++) {
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
}


void loop (char *N) {
}



