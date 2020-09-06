#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Utility.h"
#include "grayImage.h"
#include "imgPosCell.h"
#include "segment.h"
#include <math.h>
static unsigned char getShadeOfGray(int i, int numberOfSegments);/*calculates the shade of gray according to definition*/
static colorSingleSegment(grayImage* imgToColor, imgPosCell* currImgPosCell, unsigned char segmentColor);/* paint a single segment*/

/*--------------------------------------------------------------functions related to testing start*/
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


}

void printImage(grayImage img)/*printing an image for testing puposes*/
{
	printf("\n\n_______________________________________________________________________________");
	printf("\n grayscale value print of %d rows by %d cols pixles \n\n", img.rows, img.cols);
	int i, j;
	for (i = 0; i < img.rows; i++)
	{
		for (j = 0; j < img.cols; j++)
			printf("%4d ", img.pixles[i][j]);
		putchar('\n');
	}
	printf("_______________________________________________________________________________\n\n");
}
void insertRandomValues(grayImage* img)
{
	int i, j;
	for (i = 0; i < img->rows; i++)
		for (j = 0; j < img->cols; j++)
			img->pixles[i][j] = rand() % 5;
}



/*--------------------------------------------------------------functions related to testing end*/






/*--------------------------------------------------------------solution to q1 start*/

//NOTES ! TODO - AFTER Q1 IF FULLY SOLVED MOVE THE FUNCTION AND THEIR DECLERATION TO THE BEST MATCH .C AND .H FILES .
Segment* findSingleSegment(grayImage* img, imgPos kernel, unsigned char threshold)
{
	grayImage* booleanImage = (grayImage*)malloc(sizeof(grayImage));
	checkMalloc(booleanImage); // free malloc?
	initImage(&booleanImage);
	setMatrixValuesToZero(&booleanImage);


}

void setMatrixValuesToZero(grayImage* img)
{
	int i, j;
	for (i = 0; i < img->rows; i++)
		for (j = 0; j < img->cols; j++)
			img->pixles[i][j] = 0;
}


BOOL isPixelInTree(grayImage booleanImage, imgPos positionToCheck)
{
	if (booleanImage.pixles[positionToCheck[0]][positionToCheck[1]] == FOUND)
		return FOUND;
	return NOT_FOUND;
}
BOOL isColorWithInThreshold(unsigned char kernelColor, unsigned char colorToCheck, unsigned char threshold)
{
	if (abs(kernelColor - colorToCheck) <= threshold)
		return TRUE;
	return FALSE;
}

BOOL isPixelInImgBoundaries(grayImage img, imgPos positionToCheck)
{
	// Not sure  if positionToCheck[0] represents rows or cols.  this needs to be tested !!! 
	if ((positionToCheck[0] >= 0) && (positionToCheck[0] < img.rows) && (positionToCheck[1] >= 0) && (positionToCheck[1] < img.cols))
		return TRUE;
	return FALSE;
}
void markPixelInBooleanImage(grayImage* booleanImage, imgPos positionToMark)
{
	booleanImage->pixles[positionToMark[0]][positionToMark[1]] = FOUND;

}

treeNode* createTreeNode(imgPos positionToAdd)
{
	int i;
	treeNode* res = (treeNode*)malloc(sizeof(treeNode));
	checkMalloc(res);
	res->position[0] = positionToAdd[0];
	res->position[1] = positionToAdd[1];
	for (i = 0; i < MAX_NEIGHBORS + 1; i++)
	{
		res->similar_neighbors[i] = (treeNode*)malloc(sizeof(treeNode));
		checkMalloc(res->similar_neighbors[i]);
		res->similar_neighbors = NULL;
	}
}


Segment* getKernelSegment(Segment* seg, grayImage* booleanImage, grayImage img, imgPos kernel, unsigned char kernelColor, unsigned char threshold)
{
	int treeCount = 1;
	Segment* res = (Segment*)malloc(sizeof(Segment));
	checkMalloc(res);
	res->root = createTreeNode(kernel);
	getKernelSegmentAUX(res->root, booleanImage, img, kernelColor, threshold,&treeCount);
	res->size = treeCount;
	return res;
}
void getKernelSegmentAUX(treeNode* root, grayImage* booleanImage, grayImage img, unsigned char kernelColor, unsigned char threshold,int* treeCount)
{
	int i,rootNeighbors = 0;
	*treeCount++;
	//TODO - LINE 136 TO 144 ONLY CHECK ONE NEIGHBOUR, WE NEED TO CHECK ALL 8 NEIGHBOURS SURROUNDING THE ROOT {-1,-1,-1,0...}
	imgPos currPos = { root->position[0]+1,root->position[1]+1};
	if (isPixelInImgBoundaries(img, currPos))
		if(isPixelInTree(*booleanImage, currPos)==NOT_FOUND)
			if (isColorWithInThreshold(kernelColor, img.pixles[root->position[0]][root->position[1]], threshold))
			{
				rootNeighbors++;
				root->similar_neighbors[rootNeighbors] = createTreeNode(currPos);
				markPixelInBooleanImage(booleanImage, currPos);
			}
	 char ** temp = (unsigned char**)realloc(root->similar_neighbors, ((rootNeighbors) *sizeof(unsigned char*))+1);
	 if (temp == NULL)
		 exit(MEMORY_ALLOCATION_FAILED);
	 root->similar_neighbors = temp;
	 
	for(int i=0;i<rootNeighbors;i++)
		getKernelSegmentAUX(root->similar_neighbors[i],booleanImage,img,kernelColor,threshold,treeCount);
}

/*--------------------------------------------------------------solution to q1 end*/










/*--------------------------------------------------------------solution to q3 start*/
grayImage* colorSegments(grayImage* img, imgPosCell** segments, unsigned int size)
{
	unsigned int i;
	unsigned char segmentColor;
	imgPosCell* currImgPosCell;
	grayImage* coloredImage = (grayImage*)malloc(sizeof(grayImage));
	checkMalloc(coloredImage); // free malloc?
	coloredImage->cols = img->cols;
	coloredImage->rows = img->rows;
	initImage(coloredImage);
	printImage(*coloredImage);//TODO REMOVE THIS LINE - this is for testing only
	for (i = 0; i < size; i++)
	{
		segmentColor = getShadeOfGray(i, size);
		colorSingleSegment(coloredImage, *segments, segmentColor);
	}
	return coloredImage;
}

static colorSingleSegment(grayImage* imgToColor, imgPosCell* currImgPosCell, unsigned char segmentColor)/* paint a single segment*/
{
	while (currImgPosCell)
	{
		imgToColor->pixles[currImgPosCell->position[0]][currImgPosCell->position[1]] = segmentColor;
		currImgPosCell = currImgPosCell->next;
	}
}

static unsigned char getShadeOfGray(int i, int numberOfSegments)
{
	return (i * (255 / numberOfSegments - 1));
}
/*--------------------------------------------------------------solution to q3 end*/