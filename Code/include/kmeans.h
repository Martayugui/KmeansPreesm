#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "preesm.h"


void kmeansfunction(int rows, int columns, int bands,int pixels, IN float *image,IN struct parameters *par,OUT int *assignedCluster);

void initializeCluster(float *clusterCentroids, int k, int pixels, int bands, float *image);

float computeError(float *clusterCentroids, float *previousCentroids, int r, int c);

void computeDistance(int *assignedCluster, float *centroidDistances, int pixels, int bands, float *clusterCentroids, float *image, int k);

int findMinimum(float *vet, int size);

void updateClusterCentroids(float *clusterCentroids, int *assignedCluster, int pixels,int bands, float *image, int k);

