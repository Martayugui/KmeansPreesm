/*
	============================================================================
	Name        : knn.c
	Author      : rlazcano and dmadronal
	Version     : 1.0
	Copyright   : CECILL-C ?
	Description : Code to apply K-Nearest Neighbors in the hyperspectral image processing chain
	============================================================================
*/

#include "knn.h"

/*========================================================================

   knn DEFINITION

   ======================================================================*/

void featMatCreationSplit(int rows, int columns, int windowSizeKnn, int lambda, int slices, double *pcaOutput, featureMatrixNode *featMat) {
	
	int i = 0;
	int j = 0;
	int k = 0;
	int pixelCounter = 0;
    int rowIni = 0;
    int rowEnd = 0;
	int safeBorderSize = windowSizeKnn/2;
	//=====================
	// featMat building
	//=====================
	pixelCounter = 0;
    for(i = 0; i < slices; i++){
        rowIni = (rows/slices) * i;
        rowEnd = ((rows/slices) * (i + 1)) + windowSizeKnn;
	    for (j = rowIni; j < rowEnd; j++){
		    for (k = 0; k < columns; k++){
			    if(j < safeBorderSize){ //fill extra beginning rows
				    (featMat+pixelCounter)->PCA_pVal = 32767;
	        		(featMat+pixelCounter)->r = 32767;
	        		(featMat+pixelCounter)->c = 32767;
	        		(featMat+pixelCounter)->rc = -1;
	        		pixelCounter++;
			    }else if (j >= (rows + safeBorderSize)){ //fill extra ending rows
				    (featMat+pixelCounter)->PCA_pVal = 32767;
	        		(featMat+pixelCounter)->r = 32767;
	        		(featMat+pixelCounter)->c = 32767;
	        		(featMat+pixelCounter)->rc = -1;
	        		pixelCounter++;
			    }else{ //fill with real information
				    (featMat+pixelCounter)->PCA_pVal = *(pcaOutput + ((j * columns + k) - safeBorderSize * columns));
	        		(featMat+pixelCounter)->r = lambda * (j - safeBorderSize);
	        		(featMat+pixelCounter)->c = lambda * k;
	        		(featMat+pixelCounter)->rc = j * columns + k - safeBorderSize * columns;
	        		pixelCounter++;
			    }
        	}
	    }
    }
}

/*========================================================================

   knn DEFINITION

   ======================================================================*/

void featMatCreation(int rows, int columns, int windowSizeKnn, int lambda, double *pcaOutput, featureMatrixNode *featMat) {
	
	int i = 0;
	int j = 0;
	int pixelCounter = 0;
	int safeBorderSize = windowSizeKnn/2;
	//=====================
	// featMat building
	//=====================
	pixelCounter = 0;
	for (i = 0; i < rows + windowSizeKnn; i++){
		for (j = 0; j < columns; j++){
			if(i < safeBorderSize){ //fill extra beginning rows
				(featMat+pixelCounter)->PCA_pVal = 32767;
		    		(featMat+pixelCounter)->r = 32767;
		    		(featMat+pixelCounter)->c = 32767;
		    		(featMat+pixelCounter)->rc = -1;
		    		pixelCounter++;
			}else if (i >= (rows + safeBorderSize)){ //fill extra ending rows
				(featMat+pixelCounter)->PCA_pVal = 32767;
		    		(featMat+pixelCounter)->r = 32767;
		    		(featMat+pixelCounter)->c = 32767;
		    		(featMat+pixelCounter)->rc = -1;
		    		pixelCounter++;
			}else{ //fill with real information
				(featMat+pixelCounter)->PCA_pVal = *(pcaOutput + ((i * columns + j) - safeBorderSize * columns));
		    		(featMat+pixelCounter)->r = lambda * (i - safeBorderSize);
		    		(featMat+pixelCounter)->c = lambda * j;
		    		(featMat+pixelCounter)->rc = i * columns + j - safeBorderSize * columns;
		    		pixelCounter++;
			}
    	}
	}
}

/*========================================================================

   knn DEFINITION

   ======================================================================*/

void searchKnn(int pixels, int columns, int neighbors, int windowSizeKnn, featureMatrixNode *featMat, int *knnMatrix) {

	featureDistance *featDist = (featureDistance *)malloc(neighbors * sizeof(featureDistance));
	int i = 0;
	int j = 0;
	int winLEdge = 0;
	int winUEdge = 0;
	float dist = 0.0;
	int distr = 0;
	int distc = 0;
	int neighbor = 0;
	double distance = 0.0;
	int position = 0;
	int knnMIdx = 0;	
	int safeBorderSize = windowSizeKnn/2;
	int safeBorderSizeSamples = safeBorderSize*columns;

	//=====================
	// Neighbors searching
	//=====================

	winUEdge = 0;
	winLEdge = 2*safeBorderSizeSamples;
	dist = 0;
	distr = 0;
	distc = 0;
	neighbor = 0;
	distance = 0.0;
	position = neighbors-1;
	knnMIdx=0;

	for (i=columns*safeBorderSize; i<(pixels + columns*safeBorderSize); i++){

		for(j=0; j < neighbors; j++){
			featDist[j].distance = 100000;
			featDist[j].rc = 100000;
		}

		for (j=winUEdge; j<winLEdge; j++){
			dist  = (((featMat+i)->PCA_pVal) - ((featMat+j)->PCA_pVal));
			distr = ((featMat+i)->r) - ((featMat+j)->r);
			distc = ((featMat+i)->c) - ((featMat+j)->c);
			distance = (double)pow(dist,2) + (double)pow(distc,2) + (double)pow(distr,2);
			neighbor = (featMat+j)->rc;
			position = neighbors-1;

			if(distance < featDist[position].distance && distance > 0){
				featDist[position].distance = distance;
				featDist[position].rc = neighbor;
				position--;
				while(featDist[position+1].distance < featDist[position].distance && distance > 0 && position >= 0){
					featDist[position+1].distance = featDist[position].distance;
					featDist[position+1].rc = featDist[position].rc;
					featDist[position].distance = distance;
					featDist[position].rc = neighbor;
					position--;
				}
			}
		}

		winLEdge++;
		winUEdge++;

		for(j=0; j<neighbors; j++){
			knnMatrix[knnMIdx] = featDist[j].rc;
			knnMIdx++;
		}
	}
    free(featDist);
}

/*========================================================================

   knn DEFINITION

   ======================================================================*/

void filterProbs(int pixels, int numClasses, int neighbors, int *knnMatrix, float *svmProbs, float *knnProbs) {

	int i = 0;
	int j = 0;
	int k = 0;
	int knnMIdx = 0;	
	int kIdx = 0;
	//=====================
	// Filtering
	//=====================

	knnMIdx=0;
	kIdx = 0;
	//1.For each sample
	for (i = 0; i < pixels; i++){
		//2.For each class
		for (j = 0; j < numClasses; j++){
			//3.For each of the KNN indexes of the current sample for the current class ...
			knnProbs[i * numClasses + j] = 0;
			for (k = 0; k < neighbors; k++){
				// ... accumulate
				kIdx = *(knnMatrix + knnMIdx);
				knnMIdx++;
				knnProbs[i * numClasses + j] = knnProbs[i * numClasses + j] + svmProbs[kIdx * numClasses + j];
			}
			// Compute average probability for the given pair {sample,class}

			knnProbs[i * numClasses + j] = knnProbs[i * numClasses + j] / (float)neighbors;

			// Rewind knnMIdx for a new iteration over the KNN of the current sample
			knnMIdx -= neighbors;
		}
		knnMIdx = knnMIdx + neighbors;
	}
}

/*========================================================================

   knn DEFINITION

   ======================================================================*/

void knn(int rows, int columns, int numClasses, int neighbors, int windowSizeKnn, int lambda, double *pcaOutput, float *svmProbs, float *knnProbs, char *knnClasses) {

	featureDistance *featDist = (featureDistance *)malloc(neighbors * sizeof(featureDistance));
	featureMatrixNode *featMat = (featureMatrixNode *)malloc((rows + windowSizeKnn) * columns * sizeof(featureMatrixNode));
	int *knnMatrix = (int *)malloc(rows*columns*neighbors*sizeof(int));

	int i = 0;
	int j = 0;
	int k = 0;
	int pixelCounter = 0;
	int winLEdge = 0;
	int winUEdge = 0;
	int kIdx = 0;
	int maxProb = 0;
	float dist = 0.0;
	int distr = 0;
	int distc = 0;
	int neighbor = 0;
	double distance = 0.0;
	int position = 0;
	int knnMIdx = 0;	

	int safeBorderSize = windowSizeKnn/2;
	int safeBorderSizeSamples = safeBorderSize*columns;
	int pixels = rows*columns;

	
	//=====================
	// featMat building
	//=====================
	pixelCounter = 0;
	for (i = 0; i < rows + windowSizeKnn; i++){
		for (j = 0; j < columns; j++){
			if(i < safeBorderSize){ //fill extra beginning rows
				(featMat+pixelCounter)->PCA_pVal = 32767;
		    		(featMat+pixelCounter)->r = 32767;
		    		(featMat+pixelCounter)->c = 32767;
		    		(featMat+pixelCounter)->rc = -1;
		    		pixelCounter++;
			}else if (i >= (rows + safeBorderSize)){ //fill extra ending rows
				(featMat+pixelCounter)->PCA_pVal = 32767;
		    		(featMat+pixelCounter)->r = 32767;
		    		(featMat+pixelCounter)->c = 32767;
		    		(featMat+pixelCounter)->rc = -1;
		    		pixelCounter++;
			}else{ //fill with real information
				(featMat+pixelCounter)->PCA_pVal = *(pcaOutput + ((i * columns + j) - safeBorderSize * columns));
		    		(featMat+pixelCounter)->r = lambda * (i - safeBorderSize);
		    		(featMat+pixelCounter)->c = lambda * j;
		    		(featMat+pixelCounter)->rc = i * columns + j - safeBorderSize * columns;
		    		pixelCounter++;
			}
    	}
	}

	//=====================
	// Neighbors searching
	//=====================

	winUEdge = 0;
	winLEdge = 2*safeBorderSizeSamples;
	dist = 0;
	distr = 0;
	distc = 0;
	neighbor = 0;
	distance = 0.0;
	position = neighbors-1;
	knnMIdx=0;

	for (i=columns*safeBorderSize; i<(pixels + columns*safeBorderSize); i++){

		for(k=0; k < neighbors; k++){
			featDist[k].distance = 100000;
			featDist[k].rc = 100000;
		}

		for (j=winUEdge; j<winLEdge; j++){
			dist  = (((featMat+i)->PCA_pVal) - ((featMat+j)->PCA_pVal));
			distr = ((featMat+i)->r) - ((featMat+j)->r);
			distc = ((featMat+i)->c) - ((featMat+j)->c);
			distance = (double)pow(dist,2) + (double)pow(distc,2) + (double)pow(distr,2);
			neighbor = (featMat+j)->rc;
			position = neighbors-1;

			if(distance < featDist[position].distance && distance > 0){
				featDist[position].distance = distance;
				featDist[position].rc = neighbor;
				position--;
				while(featDist[position+1].distance < featDist[position].distance && distance > 0 && position >= 0){
					featDist[position+1].distance = featDist[position].distance;
					featDist[position+1].rc = featDist[position].rc;
					featDist[position].distance = distance;
					featDist[position].rc = neighbor;
					position--;
				}
			}
		}

		winLEdge++;
		winUEdge++;

		for(j=0; j<neighbors; j++){
			knnMatrix[knnMIdx] = featDist[j].rc;
			knnMIdx++;
		}


	}

	//=====================
	// Filtering
	//=====================

	knnMIdx=0;
	kIdx = 0;
	maxProb = 0;
	//1.For each sample
	for (i = 0; i < pixels; i++){
		//2.For each class
		for (j = 0; j < numClasses; j++){
			//3.For each of the KNN indexes of the current sample for the current class ...
			knnProbs[i * numClasses + j] = 0;
			for (k = 0; k < neighbors; k++){
				// ... accumulate
				kIdx = *(knnMatrix + knnMIdx);
				knnMIdx++;
				knnProbs[i * numClasses + j] = knnProbs[i * numClasses + j] + svmProbs[kIdx * numClasses + j];
			}
			// Compute average probability for the given pair {sample,class}

			knnProbs[i * numClasses + j] = knnProbs[i * numClasses + j] / (float)neighbors;

			// Rewind knnMIdx for a new iteration over the KNN of the current sample
			knnMIdx -= neighbors;
		}
		// Assign label corresponding to the highest prob. class
		maxProb = 0;
		for(j = 1; j < numClasses; j++){
			if (knnProbs[i * numClasses + j] > knnProbs[i * numClasses + maxProb]){
				maxProb = j;
			}
		}
		*(knnClasses+i) = (char)maxProb+1;

		knnMIdx = knnMIdx + neighbors;
	}

    free(featDist);
    free(featMat);
    free(knnMatrix);

}
