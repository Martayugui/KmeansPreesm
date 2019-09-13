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

void computeError( int c,char *N,float *initialCentroids,float *clusterCentroids,struct parameters *par,float *error,float *previousCentroids,float *p,int *iterin,int *iterout);

void computeDistance(int rows,int columns,int pixels,int bands,float * image,struct parameters *par, float * clusterCentroids,int *assignedCluster,float *error,float *p2,int *iterin,int *iterout,int *assignedClusterIn);

int findMinimum(float *vet, int size);

void updateClusterCentroids( int pixels,int bands,int rows,int columns, int * assignedCluster,struct parameters *par,float *image,float *error,float * clusterCentroids,float *p,float *p2,int *iterin,int *iterout);

void loop (char *N);
