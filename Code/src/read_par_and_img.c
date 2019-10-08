#include "read_par_and_img.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>


/*========================================================================

   Global Variable

   ======================================================================*/

void readParAndImg (int rows,int columns,int bands,int pixels,float *image,struct parameters *par){

	FILE *fp ;
	FILE *fp1;
	FILE *c;
	fp = fopen(namefile, "r");
	fp1 = fopen(nameimage, "rb");
	c=fopen(txtfile,"r+");
    int scan = 0;
    char s[10];
    float n;
	int i;
/*	float *img;
	img = (float *)malloc(pixels * bands * sizeof(float));*/
	int j;
	if (!fp) {
		perror("File opening failed");
		exit(0);
	}
	if (!fp1) {
			perror("File opening failed");
			exit(0);
	}
    rewind(fp);
    //rewind(fp1);
    rewind(c);
/* reads the parameters and populates the corresponding struct */
	while (scan!=EOF) {
		scan=fscanf(fp, "%s", &s);

		if (strcmp(s, "k") == 0) {
			fscanf(fp, "%d", &par->k);
		}else if (strcmp(s, "minErr") == 0) {
			fscanf(fp, "%f", &par->minErr);
		}else if (strcmp(s, "maxIter") == 0) {
			fscanf(fp, "%d", &par->maxIter);
		}else if (strcmp(s, "verbose") == 0) {
			fscanf(fp, "%d", &par->verbose);
		}
	}
	printf("parameters k: %d\t minErr: %f\t maxIter: %d\n",par->k, par->minErr, par->maxIter);
	fflush(stdout);
/* reads the image and populates the corresponding struct txt image*/

    int valueRead = 0;

	if( ftell(fp1)/(rows*columns*bands * sizeof(float)) >=1){
	    	unsigned int time = 0;
		rewind(fp1);
	    }
	valueRead = fread(image, sizeof(float), rows * columns * bands, fp1);
    valueRead = valueRead + 1;
    /*for (int i = 0; i<1; i++) {
    	    		for (int j = 0; j < bands; j++) {
    	    			printf("%f\t", image[bands*i + j]);
    	    		}
     printf("\n");
    }*/
    /*for (int i = 0; i<rows * columns; i++) {
		for (int j = 0; j < bands; j++) {
			image[(rows * columns)*j + i] = img[bands*i + j];
		}
    }*/
    	/*for (i = 0; i<pixels; i++) {
    		for (j = 0; j < bands ; j++) {
    			fprintf(c, "%f\t", image[bands*i + j]);
    		}
    		fprintf(c, "\n");
    	}
    fprintf(c, "\n");


	for (i = 0; i<pixels; i++) {
		for (j = 0; j < bands; j++) {
			fscanf(c, "%f", &image[bands*i + j]);
		}
	}
	 for (int i = 0; i<1; i++) {
	    		for (int j = 0; j < bands; j++) {
	    			printf("%f\t", image[bands*i + j]);
	    		}
	    printf("\n");
	    }*/
	fclose(fp);
    fclose(fp1);
    fclose(c);
}


