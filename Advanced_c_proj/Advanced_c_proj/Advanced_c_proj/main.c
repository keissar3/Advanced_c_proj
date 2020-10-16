#define _CRT_SECURE_NO_WARNINGS
#include "grayImage.h"
#include "imgPosCell.h"
#include "segment.h"
#include "utility.h"
#include <stdlib.h>

#define Q1
#define Q2
#define Q3
#define Q4
#define Q5
#define Q6

void main() {
#ifdef Q1
	grayImage testimage; /*creating a test image*/
	testimage.cols = testimage.rows = 5; /* a five by five matrix with random numbers with low value for debugging purposes*/
	initImage(&testimage);
	insertRandomValues(&testimage);
	imgPos kernel;
	kernel[0] = 2;
	kernel[1] = 2;
	unsigned char threshold = 0;
	Segment* test = findSingleSegment(&testimage, kernel,threshold);
#endif 

#ifdef Q2
	imgPosCell** segments;
	int size = findAllSegments(&testimage, threshold, &segments);
#endif

#ifdef Q3
	printImage(*(colorSegments(&testimage, segments, size)));
	free_segment_Array(segments,size);
	freeSegment(test);
#endif
	
	
#ifdef Q4
	grayImage* uploaded_image = readPGM("line.pgm");
#endif

#ifdef Q5
	imgPosCell** segmentsUploadedImage;
	int n = findAllSegments(uploaded_image, threshold, &segmentsUploadedImage);
	int z = 32;
	saveCompressed("compressedImage.bin", uploaded_image, z);
#endif

#ifdef Q6
	convertCompressedImageToPGM("compressedImage.bin", "decompressImage.pgm");
#endif
		
	freeImgPosCell(&segmentsUploadedImage,n);
	freePixels(&testimage);
	freeImg(uploaded_image);

}
