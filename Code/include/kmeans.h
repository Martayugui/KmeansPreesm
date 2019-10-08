#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "preesm.h"


#define clustervalues PROJECT_ROOT_PATH "/dat/clusterCentroid.txt"

void kmeansfunction(int rows, int columns, int bands,int pixels,struct parameters * par, float * image, float * error, float * clusterCentroids, int * assignedCluster);

void initializeCluster(int pixels, int bands, float * image,struct parameters * par,float *clusterCentroids);

void computeErrorInitialization(int c,struct parameters *par,float * clusterCentroids,float * error);

void computeError( int c,char *N, struct parameters *par,float *clusterCentroids,float *previousCentroids,float *errorIn,float *error,float *updatedCentroids);

void computeDistance(int rows,int columns,int pixels,int bands,float * image,struct parameters *par, float * clusterCentroids,float *error,int *assignedCluster);

int findMinimum(float *vet, int size);

void updateClusterCentroids( int pixels,int bands,int rows,int columns, int * assignedCluster,struct parameters *par,float *image,float *error,float * clusterCentroids);

void loop (char *N);

void setter (int bands,float *in, float *clusterCentroids);

void getter (int bands,float *out);

void setterError (float *in,float *error);

void getterError (float *out);
