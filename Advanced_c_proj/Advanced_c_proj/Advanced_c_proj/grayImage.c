#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Utility.h"
#include "grayImage.h"
#include "imgPosCell.h"

void initImage(grayImage* img)
{
	int j, i;
	img->pixles = (unsigned char**)malloc(img->rows * sizeof(unsigned char*));
	checkMalloc(img->pixles); // free malloc?
	for (i = 0; i < img->rows; i++)
	{
		(img->pixles)[i] = (unsigned char*)malloc(img->cols * sizeof(unsigned char));
		checkMalloc((img->pixles)[i]);// free malloc?

	}

	for (i = 0; i < img->rows; i++)
		for (j = 0; j < img->cols; j++)
			img->pixles[i][j] = rand() % 256;
}

void printImage(grayImage img)/*printing an image for testing puposes*/
{
	printf("\n\n_______________________________________________________________________________");
	printf("\n grayscale value print of %d rows by %d cols pixles \n\n", img.rows, img.cols);
	int i, j;
	for (i = 0; i < img.rows; i++)
	{
		for (j = 0; j < img.cols; j++)
			printf("%4d ", img.pixles[i][j]);/*generating dummy values*/
		putchar('\n');
	}
	printf("_______________________________________________________________________________\n\n");
}