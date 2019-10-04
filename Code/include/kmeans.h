#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "preesm.h"


#define clustervalues PROJECT_ROOT_PATH "/dat/clusterCentroid.txt"

void kmeansfunction(int rows, int columns, int bands,int pixels,struct parameters * par, float * image, float * error, float * clusterCentroids, int * assignedCluster);

void initializeCluster(int pixels, int bands, float * image,struct parameters * par,float *initialCentroids);

void computeError( int c,char *N,float *initialCentroids,struct parameters *par,float *clusterCentroids,float *previousCentroids,int *iterin,float *errorIn,float *error,int *iterout,float *updatedCentroids);

void computeDistance(int rows,int columns,int pixels,int bands,float * image,struct parameters *par, float * clusterCentroids,float *error,int *assignedCluster);

int findMinimum(float *vet, int size);

void updateClusterCentroids( int pixels,int bands,int rows,int columns, int * assignedCluster,struct parameters *par,float *image,float *error,float * clusterCentroids);

void loop (char *N);
