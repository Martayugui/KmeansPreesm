/*
	============================================================================
	Name        : read_HS_image.h
	Author      : dmadronal
	Version     : 1.0
	Copyright   : CECILL-C ?
	Description : Actor code to read a hyperspectral image from the hard drive
	============================================================================
*/

#ifndef READ_HYPERSPECTRAL_FILE_H
#define READ_HYPERSPECTRAL_FILE_H

#include "preesm.h"

#define PATHIN PROJECT_ROOT_PATH "/dat/Image.bin"

/**
* Initialize the readHypImage actor.
*/
void initreadHSImage();

/**
* Read the hyperspectral image file.
*
* @param rows
*        The rows of the opened hyperspectral image
* @param columns
*        The columns of the opened hyperspectral image
* @param bands
*        The bands of the opened hyperspectral image
* param image
*       Destination of the image read from the file
*/
void readHSImage(int rows, int columns, int bands, OUT float *image);

#endif
