#pragma once
#include "segment.h"
#include "imgPosCell.h"
#define FOUND 1
#define NOT_FOUND 0


typedef struct _grayImage { /*char matrix that stores the numberic value of shade of gray*/
	unsigned short rows;
	unsigned short cols;
	unsigned char** pixles;
} grayImage;

/***** FUNCTION PROTOTYPES *****/
void initImage(grayImage* img);/*Allocate a matrix to store all pixels of an image*/
void printImage(grayImage img);/*printing an image for testing*/
void insertRandomValues(grayImage* img);/*insert random values to the entire matrix for testing*/
grayImage* colorSegments(grayImage* img, imgPosCell** segments, unsigned int size);// TODO add description
void freeImg(grayImage* img); /*gets a pointer to an img and frees the pixels arr in img */
Segment* findSingleSegment(grayImage* img, imgPos kernel, unsigned char threshold);//todo add desctiption








