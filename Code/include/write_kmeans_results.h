/*
	============================================================================
	Name        : read_par_and_img.h
	Author      : Marta Rguez
	Version     : 1.0
	Copyright   : CECILL-C ?
	Description : Actor code to read a hyperspectral image from the hard drive
	============================================================================
*/

#ifndef WRITE_HYPERSPECTRAL_AND_PARAMETERS_FILE_H
#define WRITE_HYPERSPECTRAL_AND_PARAMETERS_FILE_H

#include "preesm.h"

#define namefileout PROJECT_ROOT_PATH "/dat/assignedClusterFinal.txt"
#define nameimageout PROJECT_ROOT_PATH "/dat/immagine.bmp"

void writeResults (int rows, int columns, IN int *assignedCluster);

#endif

