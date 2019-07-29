/*
	============================================================================
	Name        : read_HS_image.c
	Author      : dmadronal
	Version     : 1.0
	Copyright   : CECILL-C ?
	Description : Actor code to read a hyperspectral image from the hard drive
	============================================================================
*/

#include "read_HS_image.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "clock.h"

/*========================================================================

   Global Variable

   ======================================================================*/
static FILE *ptfile ;

/*========================================================================

   initreadHypImage DEFINITION

   ======================================================================*/
void initreadHSImage() {
   
	ptfile = fopen(PATHIN, "rb");
    	// Set initial clock
    	startTiming(0);
}

/*========================================================================

   readHypImage DEFINITION

   ======================================================================*/
void readHSImage(int rows, int columns, int bands, float *image) {

    int valueRead = 0;

	if( ftell(ptfile)/(rows*columns*bands * sizeof(float)) >=1){
	    	unsigned int time = 0;
		rewind(ptfile);
		time = stopTiming(0);
		printf("\nImage classified in %d us\n", time);
		startTiming(0);
		
	    }
	valueRead = fread(image, sizeof(float), rows * columns * bands, ptfile);
    valueRead = valueRead + 1;

}
