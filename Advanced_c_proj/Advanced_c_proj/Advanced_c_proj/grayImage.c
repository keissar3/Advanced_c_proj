#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Utility.h"
#include "grayImage.h"
#include "imgPosCell.h"
#include "segment.h"
#include <math.h>

/*q1 prototypes start--------------------- */
static void setMatrixValuesToZero(grayImage* img);/*set the entire matrix values to zero*/
static void markPixelInBooleanImage(grayImage* booleanImage, imgPos positionToMark);/*Mark a pixel as FOUND in the boolean image*/
static void getKernelSegmentAUX(treeNode* root, grayImage* booleanImage, grayImage img, unsigned char kernelColor, unsigned char threshold, int* treeCount);//TODO WRITE DESCRIPTION
static void getKernelSegment(Segment* seg, grayImage* booleanImage, grayImage img, imgPos kernel, unsigned char kernelColor, unsigned char threshold);//TODO WRITE DESCRIPTION
static BOOL isPixelInTree(grayImage booleanImage, imgPos positionToCheck);/*checks if a pixel was is already found within tree.*/
static BOOL isColorWithInThreshold(unsigned char kerenelColor, unsigned char colorToCheck, unsigned char threshold);/*check if target color is within threshold range from kernel color*/
static BOOL isPixelInImgBoundaries(grayImage img, imgPos positionToCheck);/*check if a position is within boundaries of the image*/
/*q1 prototypes end--------------------- */



/*q3 prototypes start--------------------- */
static unsigned char getShadeOfGray(int i, int numberOfSegments);/*calculates the shade of gray according to definition*/
static colorSingleSegment(grayImage* imgToColor, imgPosCell* currImgPosCell, unsigned char segmentColor);/* paint a single segment*/
/*q3 prototypes end---------------------  */




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
			printf("%2d ", img.pixles[i][j]);
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
	unsigned char kernelColor;
	kernelColor = img->pixles[kernel[0]][kernel[1]];
	grayImage booleanImage;
	Segment* res = (Segment*)malloc(sizeof(Segment));
	checkMalloc(res);
	booleanImage.cols = img->cols;
	booleanImage.rows = img->rows;
	initImage(&booleanImage);
	setMatrixValuesToZero(&booleanImage);
	getKernelSegment(res, &booleanImage, *img, kernel, kernelColor, threshold);

	//TODO create free image func and use it here for bolleanImage/ 
	return res;
}

static void setMatrixValuesToZero(grayImage* img)
{
	int i, j;
	for (i = 0; i < img->rows; i++)
		for (j = 0; j < img->cols; j++)
			img->pixles[i][j] = 0;
}


static BOOL isPixelInTree(grayImage booleanImage, imgPos positionToCheck)
{
	if (booleanImage.pixles[positionToCheck[0]][positionToCheck[1]] == FOUND)
		return FOUND;
	return NOT_FOUND;
}
static BOOL isColorWithInThreshold(unsigned char kernelColor, unsigned char colorToCheck, unsigned char threshold)
{
	if (abs(kernelColor - colorToCheck) <= threshold)
		return TRUE;
	return FALSE;
}

static BOOL isPixelInImgBoundaries(grayImage img, imgPos positionToCheck)
{
	if ((positionToCheck[0] >= 0) && (positionToCheck[0] < img.rows) && (positionToCheck[1] >= 0) && (positionToCheck[1] < img.cols))
		return TRUE;
	return FALSE;
}
static void markPixelInBooleanImage(grayImage* booleanImage, imgPos positionToMark)
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
	res->similar_neighbors = (treeNode**)malloc(((MAX_NEIGHBORS + 1) * sizeof(treeNode*)));
	checkMalloc(res->similar_neighbors);
	for (i = 0; i < MAX_NEIGHBORS + 1; i++)
		res->similar_neighbors[i] = NULL;

	return res;
}


static void getKernelSegment(Segment* seg, grayImage* booleanImage, grayImage img, imgPos kernel, unsigned char kernelColor, unsigned char threshold)
{
	int treeCount = 0;
	seg->root = createTreeNode(kernel);
	markPixelInBooleanImage(booleanImage, kernel);
	printf("(%d,%d) at color (%d) is added to tree \n", seg->root->position[0], seg->root->position[1], img.pixles[seg->root->position[0]][seg->root->position[1]]); //TODO Delete this line
	getKernelSegmentAUX(seg->root, booleanImage, img, kernelColor, threshold, &treeCount);
	seg->size = treeCount;

}
static void getKernelSegmentAUX(treeNode* root, grayImage* booleanImage, grayImage img, unsigned char kernelColor, unsigned char threshold, int* treeCount)
{
	int i, j, rootNeighbors = 0;
	imgPos currPos;
	(*treeCount)++;
	for (i = -1; i <= 1; i++)
		for (j = -1; j <= 1; j++) {
			if (j == 0 && i == 0)
				continue;
			currPos[0] = root->position[0] + i;
			currPos[1] = root->position[1] + j;
			if (isPixelInImgBoundaries(img, currPos))
				if (isPixelInTree(*booleanImage, currPos) == NOT_FOUND)
					if (isColorWithInThreshold(kernelColor, img.pixles[currPos[0]][currPos[1]], threshold))
					{
						printf("(%d,%d) at color (%d) is added to tree \n", currPos[0], currPos[1], img.pixles[currPos[0]][currPos[1]]); //TODO Delete this line
						root->similar_neighbors[rootNeighbors] = createTreeNode(currPos);
						markPixelInBooleanImage(booleanImage, currPos);
						rootNeighbors++;
					}
		}
	char** temp = (unsigned char**)realloc(root->similar_neighbors, ((rootNeighbors+2) * sizeof(unsigned char*)));
	if (temp == NULL)
		exit(MEMORY_ALLOCATION_FAILED);
	root->similar_neighbors = temp;

	for (int i = 0; i < rootNeighbors; i++)
		getKernelSegmentAUX(root->similar_neighbors[i], booleanImage, img, kernelColor, threshold, treeCount);
}

/*--------------------------------------------------------------solution to q1 end*/

//main:
//imgposcell** segments
//findAllSegments (                                                                 &segments)
//rita
unsigned int findAllSegments(grayImage* img, unsigned char threshold, imgPosCell*** segments)
{


}






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