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

void computeErrorInitialization(int c,struct parameters * par,float *initialCentroids,float * error);

void computeError( int c,float *previousCentroids,float *clusterCentroids,struct parameters *par);

void computeDistance(int rows,int columns,int pixels,int bands,int *N,float * image,struct parameters *par, float * clusterCentroids,float *initialCentroids, float *initialError,int *assignedCluster,float *previousCentroids);

int findMinimum(float *vet, int size);

void updateClusterCentroids( int pixels,int bands,int rows,int columns, int * assignedCluster,struct parameters *par,float * image,float * clusterCentroids);

void loop (int *N);
