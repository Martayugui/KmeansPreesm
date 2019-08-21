#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "preesm.h"

#define clustervalues PROJECT_ROOT_PATH "/dat/clusterCentroid.txt"

void kmeansfunction(int rows, int columns, int bands,int pixels,struct parameters * par, float * image, float * error, float * clusterCentroids, int * assignedCluster);

void initializeCluster(int pixels, int bands, float * image,struct parameters * par,float * clusterCentroids);

void computeErrorInitialization(int c,struct parameters * par,float * clusterCentroids,float * error);

void computeError( int c,struct parameters *par,float *clusterCentroids,float *previousCentroids);

void computeDistance(int pixels, int bands, int rows,int columns,float * image,struct parameters *par, float * clusterCentroids,char *N,int *assignedCluster);

int findMinimum(float *vet, int size);

void updateClusterCentroids( int pixels,int bands,int rows,int columns, int * assignedCluster,float *error,struct parameters *par,float * image,float * clusterCentroids,float *previousCentroids);

void loop (int *N);

void setter (int bands, float *inputsetter, float *clusterCentroids);

void getter (int bands, float *clusterCentroids);
