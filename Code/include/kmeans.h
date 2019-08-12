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

//void computeError(float *clusterCentroids, float *previousCentroids,struct parameters *par, int c,float *error);

void computeDistance(int *assignedCluster, float * centroidDistances, int pixels, int bands, float * clusterCentroids, float * image, int k);

int findMinimum(float *vet, int size);

void updateClusterCentroids(float * clusterCentroids, int * assignedCluster, int pixels,int bands, float * image, int k);

