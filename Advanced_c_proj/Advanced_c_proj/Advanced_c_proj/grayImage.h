#ifndef ___grayImage___
#define ___grayImage___
#include "segment.h"
#include "imgPosCell.h"
#include <stdio.h>

/********* DEFINES **********/
#define FOUND 1
#define NOT_FOUND 0
#define MAX_VALUE 255

/********* TYPEDEF **********/
typedef struct _grayImage { /*char matrix that stores the numberic value of shade of gray*/
	unsigned short rows;
	unsigned short cols;
	unsigned char** pixles;
} grayImage;

/***** FUNCTION PROTOTYPES *****/
/* start q1*/
void initImage(grayImage* img);/*Allocate a matrix to store all pixels of an image*/
void printImage(grayImage img);/*printing an image for testing*/
void insertRandomValues(grayImage* img);/*insert random values to the entire matrix for testing*/
void freePixels(grayImage* img); /*gets a pointer to an img and frees the pixels arr in img */
void freeImg(grayImage** img); /*gets a pointer to an img and frees the pixels arr in img */
Segment* findSingleSegment(grayImage* img, imgPos kernel, unsigned char threshold);//todo add desctiption
 /*q1 end*/


/* start q2*/
Segment* findSingleSegmentByTable(grayImage* img, imgPos kernel, unsigned char threshold, grayImage* booleanImage);
/*gets an image, kernel, treshold and boolenImange, return a segment according to booleanTable. In the places the table has FOUND(1),
the func will not add them to the segment*/
unsigned int findAllSegments(grayImage* img, unsigned char threshold, imgPosCell*** segments);/*gets an image and finds all the segments
 in the image and puts them into given segments array. return how many items are in segments array*/
 /*q2 end*/

/* start q3*/
grayImage* colorSegments(grayImage* img, imgPosCell** segments, unsigned int size);/*color all the segments in the image */

 /*q3 end*/

/* start q4*/
grayImage* readPGM(char* fname);/*reads a pgm file and return a pointer to an grayimage struct*/
/*q4 end*/

/* start q5*/
void saveCompressed(char* file_name, grayImage* image, unsigned char reduced_gray_levels); /*gets an image and compress it into a binary file*/
/*q5 end*/

/* start q6*/
void convertCompressedImageToPGM(char* compressed_file_name, char* pgm_file_name);/*decompress a binary file to a pgm image*/
/*q6 end*/
#endif