#include "write_kmeans_results.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "preesm.h"

/*========================================================================

   Global Variable

   ======================================================================*/
static FILE *f;
static FILE *fp;


/* Colors' references https://web.njit.edu/~kevin/rgb.txt.html
it is supposed to not have more than 25 different classes */
const int COLOR_MAP[25][3] =
{
	{ 255, 255, 255 },	// white
	{ 0, 0, 0 },		// black
	{ 112, 128, 144 },		// slate grey
	{ 192, 192, 192},		// grey silver
	{ 77, 77, 77},		// grey30
	{ 0, 0, 255},		// blue
	{ 0, 0, 128 },		// navy
	{ 0, 127, 255 },		// slate blue
	{ 64, 224, 208},		// turquoise
	{ 92, 64, 51},		// dark brown
	{ 139, 69, 19 },	// chocolate4
	{ 205, 133, 63},		//tan3
	{ 0, 128, 0 },		// green
	{ 0, 205, 0 },		// green3
	{ 128, 128, 0},		// olive
	{ 255, 127, 0 },	// orange
	{ 255, 114, 86},		// coral1
	{ 148, 0, 211},		// dark violet
	{ 171, 130, 255},		//medium purple1
	{ 255, 255, 0},		// yellow
	{ 255, 215, 0 },		// gold
	{ 153, 204, 50},		// yellow green
	{ 255, 0, 0 },		// red
	{ 255, 181, 197},		// pink1
	{ 139, 26, 26},		// firebrick4
};

void writeResults (int rows, int columns, int *assignedCluster)  {

	int p;
	int q;

	fp = fopen(namefileout, "w");

	for (p = 0; p < rows; p++) {
		for (q = 0; q < columns; q++) {
			fprintf(fp, "%d\t", assignedCluster[rows*q+p]);
		}
		fprintf(fp, "\n");
	}
	fprintf(fp, "\n");
	fclose(fp);

/* converts results to an image*/

	int x, y, r, g, b, idx, i, j;
	//unsigned char img[3 * rows*columns];
	unsigned char *img = NULL;
	int filesize = 54 + 3 * rows*columns;  //rows is your image width, columns is image height, both int
	if (img)
	free(img);
    img = (unsigned char *)malloc(3 * rows*columns);
	memset(img, 0, sizeof(img));

	for (i = 0; i<rows; i++)
	{
		for (j = 0; j<columns; j++)
		{
			x = i; y = (columns - 1) - j;
			idx = assignedCluster[i+j*rows];
			r = COLOR_MAP[idx][0];
			g = COLOR_MAP[idx][1];
			b = COLOR_MAP[idx][2];
			img[(x + y*rows) * 3 + 2] = (unsigned char)(r);
			img[(x + y*rows) * 3 + 1] = (unsigned char)(g);
			img[(x + y*rows) * 3 + 0] = (unsigned char)(b);
		}
	}

	unsigned char bmpfileheader[14] = { 'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0 };
	unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0 };
	unsigned char bmppad[3] = { 0,0,0 };

	bmpfileheader[2] = (unsigned char)(filesize);
	bmpfileheader[3] = (unsigned char)(filesize >> 8);
	bmpfileheader[4] = (unsigned char)(filesize >> 16);
	bmpfileheader[5] = (unsigned char)(filesize >> 24);

	bmpinfoheader[4] = (unsigned char)(rows);
	bmpinfoheader[5] = (unsigned char)(rows >> 8);
	bmpinfoheader[6] = (unsigned char)(rows >> 16);
	bmpinfoheader[7] = (unsigned char)(rows >> 24);
	bmpinfoheader[8] = (unsigned char)(columns);
	bmpinfoheader[9] = (unsigned char)(columns >> 8);
	bmpinfoheader[10] = (unsigned char)(columns >> 16);
	bmpinfoheader[11] = (unsigned char)(columns >> 24);

	f = fopen(nameimageout, "wb");
	fwrite(bmpfileheader, 1, 14, f);
	fwrite(bmpinfoheader, 1, 40, f);
	//for (i = 0; i<h; i++)
	for (i = columns-1; i>=0; i--)
	{
		fwrite(img + (rows*(columns - i - 1) * 3), 3, rows, f);
		fwrite(bmppad, 1, (4 - (rows * 3) % 4) % 4, f);
	}
	fclose(f);

}





