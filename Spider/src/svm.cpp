/*
	============================================================================
	Name        : svm.c
	Author      : rlazcano and dmadronal
	Version     : 1.0
	Copyright   : CECILL-C ?
	Description : Code to apply Support Vector Machines classification in the hyperspectral image processing chain
	============================================================================
*/

#include "svm.h"

/*========================================================================

   Global Variable

   ======================================================================*/
static FILE *ptfileW ;
static FILE *ptfileR ;
static FILE *ptfileL ;
static FILE *ptfilePA ;
static FILE *ptfilePB ;

/*========================================================================

   initReadSvmModel DEFINITION

   ======================================================================*/
void initReadSvmModel() {
   
	ptfileW = fopen(PATHW, "rb");
	ptfileR = fopen(PATHRHO, "rb");
	ptfileL = fopen(PATHLABEL, "rb");
	ptfilePA = fopen(PATHPROBA, "rb");
	ptfilePB = fopen(PATHPROBB, "rb");
}

/*========================================================================

   readSvmModel DEFINITION

   ======================================================================*/
void readSvmModel(int bands, int numClasses, float *wVector, float *rho, int *label, float *probA, float *probB) {

    int valueRead = 0;

	int numClassifiers = ((numClasses * (numClasses - 1)) / 2);
	if( ftell(ptfileW)/(bands * numClassifiers * sizeof(float)) >=1){
		rewind(ptfileW);
	}
	valueRead = fread(wVector, sizeof(float), bands * numClassifiers, ptfileW);
	if( ftell(ptfileR)/(numClassifiers * sizeof(float)) >=1){
		rewind(ptfileR);
	}
	valueRead = fread(rho, sizeof(float), numClassifiers, ptfileR);
	if( ftell(ptfileL)/(numClasses * sizeof(int)) >=1){
		rewind(ptfileL);
	}
	valueRead = fread(label, sizeof(int), numClasses, ptfileL);
	if( ftell(ptfilePA)/(numClassifiers * sizeof(float)) >=1){
		rewind(ptfilePA);
	}
	valueRead = fread(probA, sizeof(float), numClassifiers, ptfilePA);
	if( ftell(ptfilePB)/(numClassifiers * sizeof(float)) >=1){
		rewind(ptfilePB);
	}
	valueRead = fread(probB, sizeof(float), numClassifiers, ptfilePB);
    valueRead = valueRead + 1;
}

/*========================================================================

   computeSvmPairwiseProbs DEFINITION

   ======================================================================*/
void computeSvmPairwiseProbs(int pixels, int bands, int numClasses, float *image, float *wVector, float *rho, float *probA, float *probB, float *pairwiseProb) {
    
	int numClassifiers = ((numClasses * (numClasses - 1)) / 2);
	float decValues[numClassifiers];
    int q = 0;
    int b = 0;
    int j = 0;
    int k = 0;
    int classifier = 0;
	float distance = 0.0;
	float sigmoidPredictionFApB = 0.0;
	float sigmoidPrediction = 0.0;
	float minProb = 0.0000001;
	float maxProb = 0.9999999;

	for(q = 0; q < pixels; q++){		//for each pixel
		classifier = 0;
		for(b = 0; b < numClasses; b++){		//for every combination of classes
			for(k = b + 1; k < numClasses; k++){
				distance = 0.0;
				for(j = 0; j < bands; j++){
					distance = distance + image[q * bands + j] * wVector[classifier * bands + j];
				}
				decValues[classifier] = distance - rho[classifier];
				sigmoidPredictionFApB = decValues[classifier] * probA[classifier] + probB[classifier];

				if (sigmoidPredictionFApB >= 0.0){
					sigmoidPrediction = exp(-sigmoidPredictionFApB) / (1.0 + exp(-sigmoidPredictionFApB));
				}
				else{
					sigmoidPrediction = 1.0 / (1.0 + exp(sigmoidPredictionFApB));
				}
				if(sigmoidPrediction < minProb){
					sigmoidPrediction = minProb;
				}		
				if(sigmoidPrediction > maxProb){
					sigmoidPrediction = maxProb;
				}					
				pairwiseProb[(q * numClasses * numClasses) + (b * numClasses + k)] = sigmoidPrediction;
				pairwiseProb[(q * numClasses * numClasses) + (k * numClasses + b)] = 1 - sigmoidPrediction;
					
				classifier++;
			}
		}
	}
}

/*========================================================================

   minimizationSolver DEFINITION

   ======================================================================*/
void minimizationSolver(int pixels, int bands, int numClasses, int *label, float *pairwiseProb, float *pixelProbs) {

	float multiProbQ[numClasses][numClasses];
	float multiProbQp[numClasses];
	float partialProbEstimated[numClasses];
	int j = 0;
	int q = 0;
	int b = 0;
	int iters = 0;
    int stop = 0;
	int position = 0;
	float pQp = 0.0;
	float maxError = 0.0; 
	float maxErrorAux = 0.0;
	float epsilon = 0.005 / numClasses;
	float diffPQp = 0.0;


	for(q = 0; q < pixels; q++){		//for each pixel	
		for(b = 0; b < numClasses; b++){ // for every binary classifier
			partialProbEstimated[b] = 1.0 / numClasses;
			multiProbQ[b][b] = 0.0;
			for(j = 0; j < b; j++){
				multiProbQ[b][b] = multiProbQ[b][b] + pairwiseProb[(q * numClasses * numClasses) + (j * numClasses + b)] * pairwiseProb[(q * numClasses * numClasses) + (j * numClasses + b)];
				multiProbQ[b][j] = multiProbQ[j][b];
			}
			for(j = b + 1; j < numClasses; j++){
				multiProbQ[b][b] = multiProbQ[b][b] + pairwiseProb[(q * numClasses * numClasses) + (j * numClasses + b)] * pairwiseProb[(q * numClasses * numClasses) + (j * numClasses + b)];
				multiProbQ[b][j] = -pairwiseProb[(q * numClasses * numClasses) + (j * numClasses + b)] * pairwiseProb[(q * numClasses * numClasses) + (b * numClasses + j)];
			}
		}

		iters = 0;
		stop = 0;
	
		pQp = 0.0;
		for(b = 0; b < numClasses; b++){
			multiProbQp[b] = 0.0;
			for(j = 0; j < numClasses; j++){
				multiProbQp[b] = multiProbQp[b] + multiProbQ[b][j] * partialProbEstimated[j];
			}
			pQp = pQp + partialProbEstimated[b] * multiProbQp[b];
		}

		while (stop == 0){
			
			maxError = 0.0;
			for(b = 0; b < numClasses; b++){
				maxErrorAux = multiProbQp[b] - pQp;
				if (maxErrorAux < 0.0){
					maxErrorAux = -maxErrorAux;
				}
				if (maxErrorAux > maxError){
					maxError = maxErrorAux;
				}
			}
			if(maxError < epsilon){
				stop = 1;
			}
			if(stop == 0){
				for(b = 0; b < numClasses; b++){
					diffPQp = (-multiProbQp[b] + pQp) / (multiProbQ[b][b]);
					partialProbEstimated[b] = partialProbEstimated[b] + diffPQp;
					pQp = ((pQp + diffPQp * (diffPQp * multiProbQ[b][b] + 2 * multiProbQp[b])) / (1 + diffPQp)) / (1 + diffPQp);
					for(j = 0; j < numClasses; j++){
						multiProbQp[j] = (multiProbQp[j] + diffPQp * multiProbQ[b][j]) / (1 + diffPQp);
						partialProbEstimated[j] = partialProbEstimated[j] / (1 + diffPQp);
					}
				}
			}
			iters++;
			
			if(iters == 100){
				stop = 1;
			}
		}

		for(b = 0; b < numClasses; b++){
			position = label[b] - 1;
			pixelProbs[q * numClasses + position] = partialProbEstimated[b];
		}
	}
}

/*========================================================================

   svm DEFINITION

   ======================================================================*/
void svm(int pixels, int bands, int numClasses, float *image, float *wVector, float *rho, int *label, float *probA, float *probB, float *svmProbs, char *svmClasses) {

	int numClassifiers = ((numClasses * (numClasses - 1)) / 2);
	float pairwiseProb[numClasses][numClasses];
	float multiProbQ[numClasses][numClasses];
	float decValues[numClassifiers];
	float multiProbQp[numClasses];
	float partialProbEstimated[numClasses];
    int j = 0;
    int k = 0;
    int q = 0;
    int b = 0;
    int classifier = 0;
	float distance = 0.0;
	float sigmoidPredictionFApB = 0.0;
	float sigmoidPrediction = 0.0;
	float minProb = 0.0000001;
	float maxProb = 0.9999999;
	int iters = 0;
    int stop = 0;
	float pQp = 0.0;
	float maxError = 0.0; 
	float maxErrorAux = 0.0;
	float epsilon = 0.005 / numClasses;
	float diffPQp = 0.0;
	int decision = 0;
	int position = 0;

	float probEstimated[pixels][numClasses];

	for(q = 0; q < pixels; q++){		//for each pixel
		classifier = 0;
		for(b = 0; b < numClasses; b++){		//for every combination of classes
			for(k = b + 1; k < numClasses; k++){
				distance = 0.0;
				for(j = 0; j < bands; j++){
					distance = distance + image[q * bands + j] * wVector[classifier * bands + j];
				}
				decValues[classifier] = distance - rho[classifier];
				sigmoidPredictionFApB = decValues[classifier] * probA[classifier] + probB[classifier];

				if (sigmoidPredictionFApB >= 0.0){
					sigmoidPrediction = exp(-sigmoidPredictionFApB) / (1.0 + exp(-sigmoidPredictionFApB));
				}
				else{
					sigmoidPrediction = 1.0 / (1.0 + exp(sigmoidPredictionFApB));
				}
				if(sigmoidPrediction < minProb){
					sigmoidPrediction = minProb;
				}		
				if(sigmoidPrediction > maxProb){
					sigmoidPrediction = maxProb;
				}					
				pairwiseProb[b][k] = sigmoidPrediction;
				pairwiseProb[k][b] = 1 - sigmoidPrediction;					
				classifier++;
			}
		}
		for(b = 0; b < numClasses; b++){ // for every binary classifier
			partialProbEstimated[b] = 1.0 / numClasses;
			multiProbQ[b][b] = 0.0;
			for(j = 0; j < b; j++){
				multiProbQ[b][b] = multiProbQ[b][b] + pairwiseProb[j][b] * pairwiseProb[j][b];
				multiProbQ[b][j] = multiProbQ[j][b];
			}
			for(j = b + 1; j < numClasses; j++){
				multiProbQ[b][b] = multiProbQ[b][b] + pairwiseProb[j][b] * pairwiseProb[j][b];
				multiProbQ[b][j] = -pairwiseProb[j][b] * pairwiseProb[b][j];
			}
		}

		iters = 0;
		stop = 0;
	
		pQp = 0.0;
		for(b = 0; b < numClasses; b++){
			multiProbQp[b] = 0.0;
			for(j = 0; j < numClasses; j++){
				multiProbQp[b] = multiProbQp[b] + multiProbQ[b][j] * partialProbEstimated[j];
			}
			pQp = pQp + partialProbEstimated[b] * multiProbQp[b];
		}

		while (stop == 0){
			
			maxError = 0.0;
			for(b = 0; b < numClasses; b++){
				maxErrorAux = multiProbQp[b] - pQp;
				if (maxErrorAux < 0.0){
					maxErrorAux = -maxErrorAux;
				}
				if (maxErrorAux > maxError){
					maxError = maxErrorAux;
				}
			}
			if(maxError < epsilon){
				stop = 1;
			}
			if(stop == 0){
				for(b = 0; b < numClasses; b++){
					diffPQp = (-multiProbQp[b] + pQp) / (multiProbQ[b][b]);
					partialProbEstimated[b] = partialProbEstimated[b] + diffPQp;
					pQp = ((pQp + diffPQp * (diffPQp * multiProbQ[b][b] + 2 * multiProbQp[b])) / (1 + diffPQp)) / (1 + diffPQp);
					for(j = 0; j < numClasses; j++){
						multiProbQp[j] = (multiProbQp[j] + diffPQp * multiProbQ[b][j]) / (1 + diffPQp);
						partialProbEstimated[j] = partialProbEstimated[j] / (1 + diffPQp);
					}
				}
			}
			iters++;
			
			if(iters == 100){
				stop = 1;
			}
		}

		for(b = 0; b < numClasses; b++){
			probEstimated[q][b] = partialProbEstimated[b];
		}
		
		//decision
		decision = 0;
		for(b = 1; b < numClasses; b++){
			if(partialProbEstimated[b] > partialProbEstimated[decision]){
				decision = b;
			}
		}
		
		svmClasses[q] = (char)label[decision];

		for(b = 0; b < numClasses; b++){
			position = label[b] - 1;
			svmProbs[q * numClasses + position] = probEstimated[q][b];
		}
	}
}
