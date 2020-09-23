#define _CRT_SECURE_NO_WARNINGS
#include "grayImage.h"
#include "imgPosCell.h"
#include "segment.h"
#include "utility.h"
#include <stdlib.h>
void main() {

	grayImage testimage; /*creating a test image*/
	testimage.cols = testimage.rows = 5; /* a four by four matrix with random numbers with low value for debugging purposes*/
	initImage(&testimage);
	insertRandomValues(&testimage);
	printImage(testimage);



	//testing q1
	imgPos kernel;
	kernel[0] = 2;
	kernel[1] = 2;
	unsigned char threshold = 0;
	Segment* test = findSingleSegment(&testimage, kernel,threshold);
	//TODO create a func to free 'test' -freeSegment func

	//testing q1 end


	/*testing q2*/
	//imgPosCell** segments;
	//int n = findAllSegments(&testimage, threshold, &segments);
	/*testing q2 end*/

	/*-----testing q3 start - seems to be working :)*/
	//	printImage(*(colorSegments(&testimage, segments,n)));
	/*-----testing q3 end*/

	/*testing q4*/
	grayImage* uploaded_image = readPGM("hi.pgm");
	//printImage(*uploaded_image);



	imgPosCell** segments;
	int n = findAllSegments(uploaded_image, threshold, &segments);
	printImage(*(colorSegments(uploaded_image, segments, n)));
	freeImg(&uploaded_image);
	/*testing q4*/







	
	freeImgPosCell(&segments,n);
	freePixels(&testimage);

}
