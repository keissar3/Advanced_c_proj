#pragma once
#include "imgPosCell.h"
typedef struct _grayImage { /*char matrix that stores the numberic value of shade of gray*/
	unsigned short rows;
	unsigned short cols;
	unsigned char** pixles;
} grayImage;

void initImage(grayImage* img);/*Allocate a matrix to store all pixels of an image*/
void printImage(grayImage img);/*printing an image for testing*/

grayImage* colorSegments(grayImage* img, imgPosCell** segments, unsigned int size);// TODO add description