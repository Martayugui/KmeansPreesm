#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "preesm.h"

#define clustervalues PROJECT_ROOT_PATH "/dat/clusterCentroid.txt"

void kmeansfunction(int rows, int columns, int bands,int pixels,IN float *clusterCentroids, IN float *image,IN struct parameters *par,OUT int *assignedCluster);

void initializeCluster(float *clusterCentroids, int pixels, int bands,struct parameters *par, float *image);

float computeError(float *clusterCentroids,struct parameters *par, int c);

void computeDistance(int *assignedCluster, float *centroidDistances, int pixels, int bands, float *clusterCentroids, float *image, int k);

int findMinimum(float *vet, int size);

void updateClusterCentroids(float *clusterCentroids, int *assignedCluster, int pixels,int bands, float *image, int k);

