#define _CRT_SECURE_NO_WARNINGS

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

/*q2 prototypes start---------------------*/
static imgPos* findTmpMin(grayImage img, grayImage booleanImg);
static int doesBooleanImageHaveZero(grayImage booleanImg); /*return 1 if there are pixels that are 0. else, return 0*/
static void booleanImageInitialization(grayImage* img, grayImage* booleanImage);
static void doubleArrSize(Segment*** arr, int* currentCapacity);
static int compare(const void* seg1, const void* seg2);
static void insertSegmentToList(Segment** seg_arr, int seg_arr_size, imgPosCell*** segments);
static void insertSegmentToListAux(treeNode* root, imgPosCell** segments);
static void addToList(imgPosCell* node_to_add, imgPosCell** segments);
/*q2 prototypes end--------------------- */


/*q3 prototypes start--------------------- */
static unsigned char getShadeOfGray(int i, int numberOfSegments);/*calculates the shade of gray according to definition*/
static colorSingleSegment(grayImage* imgToColor, imgPosCell* currImgPosCell, unsigned char segmentColor);/* paint a single segment*/
/*q3 prototypes end---------------------  */



/***** Function Implementation *****/

/*--------------------------------------------------------------functions related to testing start*/
void initImage(grayImage* img)
{
	int i;
	img->pixles = (unsigned char**)malloc(img->rows * sizeof(unsigned char*));
	checkMalloc(img->pixles);
	for (i = 0; i < img->rows; i++)
	{
		(img->pixles)[i] = (unsigned char*)malloc(img->cols * sizeof(unsigned char));
		checkMalloc((img->pixles)[i]);
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

/* GENERAL*/
void freePixels(grayImage* img) {
	int i;
	for (i = 0; i < img->rows; i++)
	{
		free((img->pixles[i]));
	}
	free(img->pixles);
}

void freeImg(grayImage** img) {
	int i;
	for (i = 0; i < (*img)->rows; i++)
	{
		free(((*img)->pixles[i]));
	}
	free((*img)->pixles);
	free(*img);
}







/*--------------------------------------------------------------solution to q1 start*/





//NOTES ! TODO - AFTER Q1 IF FULLY SOLVED MOVE THE FUNCTION AND THEIR DECLERATION TO THE BEST MATCH .C AND .H FILES .
Segment* findSingleSegment(grayImage* img, imgPos kernel, unsigned char threshold)
{
	unsigned char kernelColor;
	kernelColor = img->pixles[kernel[0]][kernel[1]];
	grayImage booleanImage;
	Segment* res = (Segment*)malloc(sizeof(Segment)); //free in main, because main calls this func and gets back 'res'
	checkMalloc(res);
	booleanImageInitialization(img, &booleanImage);
	getKernelSegment(res, &booleanImage, *img, kernel, kernelColor, threshold);
	freePixels(&booleanImage);
	return res;
}

Segment* findSingleSegmentByTable(grayImage* img, imgPos kernel, unsigned char threshold, grayImage* booleanImage) {
	unsigned char kernelColor;
	kernelColor = img->pixles[kernel[0]][kernel[1]];
	Segment* res = (Segment*)malloc(sizeof(Segment)); //free
	checkMalloc(res);
	getKernelSegment(res, booleanImage, *img, kernel, kernelColor, threshold);
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
	char** temp = (unsigned char**)realloc(root->similar_neighbors, ((rootNeighbors + 2) * sizeof(unsigned char*)));
	if (temp == NULL)
		exit(MEMORY_ALLOCATION_FAILED);
	root->similar_neighbors = temp;

	for (int i = 0; i < rootNeighbors; i++)
		getKernelSegmentAUX(root->similar_neighbors[i], booleanImage, img, kernelColor, threshold, treeCount);
}

/*--------------------------------------------------------------solution to q1 end*/


/*--------------------------------------------------------------solution to q2 start*/
unsigned int findAllSegments(grayImage* img, unsigned char threshold, imgPosCell*** segments)
{
	Segment** helper;  /*an array of pointers to segment, will hold the different segments we will find in img, before their
					 insertation to 'segments', array of imgPossCell*/
	grayImage booleanImage;  /*will help us to know which pixels are allready in a certain segment*/
	booleanImageInitialization(img, &booleanImage); /*initializes booleanImage to zeros and updates the rows and cols according
													to img rows and cols*/
	int currentCapacity, numOfSegments;
	currentCapacity = numOfSegments = 0;
	helper = NULL;
	while (doesBooleanImageHaveZero(booleanImage) == TRUE) {
		imgPos* tmpMin = findTmpMin(*img, booleanImage);
		if (currentCapacity == numOfSegments)
			doubleArrSize(&helper, &currentCapacity); /*resizing of needed*/
		helper[numOfSegments] = findSingleSegmentByTable(img, *tmpMin, threshold, &booleanImage);
		numOfSegments++;
		free(tmpMin);
	}
	helper = (Segment**)realloc(helper, numOfSegments * sizeof(Segment));
	checkMalloc(helper);
	qsort(helper, numOfSegments, sizeof(Segment*), (&compare)); /*sorts the array of pointers to segments*/
	insertSegmentToList(helper, numOfSegments, segments);
	freePixels(&booleanImage);
	//TODO SEGEMNTS AND THEIR CONTENT IN HELPER NEED TO BE FREE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	return numOfSegments;
}

static void insertSegmentToList(Segment** seg_arr, int seg_arr_size, imgPosCell*** segments) {
	int i;
	*segments = (imgPosCell**)malloc(seg_arr_size * sizeof(imgPosCell*));
	checkMalloc(*segments);
	for (i = 0; i < seg_arr_size; i++)
	{
		(*segments)[i] = NULL;
		if (seg_arr[i]->root != NULL) {
			insertSegmentToListAux(seg_arr[i]->root, &((*segments)[i]));
		}
	
	}
}

static void insertSegmentToListAux(treeNode* root, imgPosCell** segments) {
	int i = 0, root_neighbors = 0;
	imgPosCell* new_node = creatImgPosCellNode(root->position);
	addToList(new_node, segments);

	while (root->similar_neighbors[i] != NULL)
	{
		insertSegmentToListAux(root->similar_neighbors[i], segments);
		i++;
	}

}

static void addToList(imgPosCell* node_to_add, imgPosCell** segments) {
	if (*segments == NULL) {
		*segments = node_to_add;
	}
	else {
		imgPosCell* p = findPlaceToInsert(*segments, node_to_add->position);
		if (p == NULL)
			addToBeginningOfList(segments, node_to_add);
		else
			addToInnerPlaceInList(p, node_to_add);
	}
}

static int compare(const void* seg1, const void* seg2) {
	Segment* seg1P = *((Segment**)(seg1));
	Segment* seg2P = *((Segment**)(seg2));
	return seg2P->size - seg1P->size;
}

static void doubleArrSize(Segment*** arr, int* currentCapacity) {
	*currentCapacity = (*currentCapacity) * 2 + 1;
	*arr = (Segment**)realloc(*arr, (*currentCapacity) * sizeof(Segment*));
	checkMalloc(*arr);
}

static int doesBooleanImageHaveZero(grayImage booleanImg) {
	int i, j;
	for (i = 0; i < booleanImg.rows; i++)
		for (j = 0; j < booleanImg.cols; j++)
			if (booleanImg.pixles[i][j] == NOT_FOUND)
				return TRUE;
	return FALSE;
}

static imgPos* findTmpMin(grayImage img, grayImage booleanImg) {
	imgPos* res;
	res = (imgPos*)malloc(sizeof(imgPos));
	checkMalloc(res);
	int i, j, min;
	min = MAX_VALUE; 
	for (i = 0; i < img.rows; i++) {
		for (j = 0; j < img.cols; j++)
			if ((booleanImg.pixles[i][j] == NOT_FOUND) && img.pixles[i][j] <= min)
			{
				min = img.pixles[i][j];
				(*res)[0] = i;
				(*res)[1] = j;
			}
	}
	return res;
}

static void booleanImageInitialization(grayImage* img, grayImage* booleanImage)
{
	booleanImage->cols = img->cols;
	booleanImage->rows = img->rows;
	initImage(booleanImage);
	setMatrixValuesToZero(booleanImage);
}



/*--------------------------------------------------------------solution to q2 end*/


/*--------------------------------------------------------------solution to q3 start*/
grayImage* colorSegments(grayImage* img, imgPosCell** segments, unsigned int size)
{
	unsigned int i;
	unsigned char segmentColor;
	grayImage* coloredImage = (grayImage*)malloc(sizeof(grayImage));
	checkMalloc(coloredImage); // free malloc?
	coloredImage->cols = img->cols;
	coloredImage->rows = img->rows;
	initImage(coloredImage);
	for (i = 0; i < size; i++)
	{
		segmentColor = getShadeOfGray(i, size);
		colorSingleSegment(coloredImage, segments[i], segmentColor);
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

/*--------------------------------------------------------------solution to q4 Start*/

void doubleStringSize(char** string, int* size)
{
	*size = (*size) * 2 + 1;
	*string = (char*)realloc(*string, (*size) * sizeof(char));
	checkMalloc(*string);

}

char* getLineFromPGM(FILE* fptr_in)
{
	BOOL lineWasComment = FALSE;
	char* line_read = (char*)malloc(sizeof(char) * 0);
	checkMalloc(line_read);
	int logicSize = 0, physicalSize = 0;
	char c = fgetc(fptr_in);
	do
	{
		if (lineWasComment)
		{
			free(line_read);
			logicSize = physicalSize = 0;
			c = fgetc(fptr_in);
			line_read = (char*)malloc(sizeof(char) * 0);
			checkMalloc(line_read);

		}
		while (c != '\n')
		{
			if (logicSize == physicalSize)
				doubleStringSize(&line_read, &physicalSize);
			line_read[logicSize] = c;
			logicSize++;
			c = fgetc(fptr_in);
		}
		line_read = (char*)realloc(line_read, (logicSize + 1) * sizeof(char));
		checkMalloc(line_read);
		line_read[logicSize] = '\0';
		if (line_read[0] == '#')
			lineWasComment = TRUE;
		else
			lineWasComment = FALSE;
	} while (lineWasComment);
	return line_read;
}

grayImage* readPGM(char* fname)
{
	int logicSize = 0, phisicallSize = 0, rows, cols, max_color, i, j, num;
	grayImage* uploaded_image = (grayImage*)malloc(sizeof(grayImage));
	checkMalloc(uploaded_image);
	FILE* fptr_in;
	fptr_in = fopen(fname, "r");
	checkFileOpen(fptr_in);
	read_PGM_info(fptr_in, &rows, &cols, &max_color);
	uploaded_image->rows = rows, uploaded_image->cols = cols;
	uploaded_image->pixles = (unsigned char**)malloc(sizeof(unsigned char*) * (rows));
	checkMalloc(uploaded_image->pixles);
	for (i = 0; i < rows; i++)
	{
		uploaded_image->pixles[i] = (unsigned char*)malloc(sizeof(unsigned char) * (cols));
		checkMalloc(uploaded_image->pixles[i]);
	}
	for (i = 0; i < rows; i++)
		for (j = 0; j < cols; j++)
		{
			fscanf(fptr_in, "%d", &num);
			uploaded_image->pixles[i][j] = (unsigned char)num;
		}
	fclose(fptr_in);
	return uploaded_image;
}


void read_PGM_info(FILE* fptr_in, int* rows, int* cols, int* max_color)
{
	char* temp_line;
	temp_line = getLineFromPGM(fptr_in);
	if (strcmp(temp_line, "P2") != 0)
	{
		printf("Not a P2 file");
		exit(NOT_P2_PGM);
	}
	free(temp_line);
	temp_line = getLineFromPGM(fptr_in);

	if (sscanf(temp_line, "%d %d", cols, rows) != 2)
	{
		printf("Failed reading image cols and rows sizes");
		exit(COLS_ROWS_WRONG_INFO);
	}
	free(temp_line);
	temp_line = getLineFromPGM(fptr_in);
	if (sscanf(temp_line, "%d", max_color) != 1)//TODO /* what is the use of this value */
	{
		printf("MAX color reading failed ");
		exit(MAX_COLOR_INCORRECT);
	}
	free(temp_line);
}
/*--------------------------------------------------------------solution to q4 end*/