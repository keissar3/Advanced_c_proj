#define _CRT_SECURE_NO_WARNINGS
#include "grayImage.h"
#include "imgPosCell.h"
#include "segment.h"
#include <stdlib.h>
void main() {

	grayImage testimage; /*creating a test image*/
	testimage.cols = testimage.rows = 10; /* a four by four matrix with random numbers with low value for debugging purposes*/
	initImage(&testimage);
	insertRandomValues(&testimage);
	printImage(testimage);



	//testing q1
	imgPos kernel;
	kernel[0] = 2;
	kernel[1] = 2;
	unsigned char threshold = 1;
	Segment* test = findSingleSegment(&testimage, kernel,threshold);
	//testing q1

	/*-----testing q3 start - seems to be working :)*/
	/*imgPosCell *testimgposarray[1];
	testimgposarray[0] = (imgPosCell*)malloc(sizeof(imgPosCell));
	testimgposarray[0]->position[0] = 0;
	testimgposarray[0]->position[1] = 3;
	testimgposarray[0]->next = (imgPosCell*)malloc(sizeof(imgPosCell));
	testimgposarray[0]->next->position[0] = 1;
	testimgposarray[0]->next->position[1] = 3;
	testimgposarray[0]->next->next = NULL;
	printImage(*(colorSegments(&testimage,testimgposarray ,1)));*/
	/*-----testing q3 end*/


}
