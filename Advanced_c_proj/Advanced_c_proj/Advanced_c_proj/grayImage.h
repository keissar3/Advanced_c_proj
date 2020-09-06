#pragma once
#include "imgPosCell.h"
#include "utility.h"
#define FOUND 1
#define NOT_FOUND 0


typedef struct _grayImage { /*char matrix that stores the numberic value of shade of gray*/
	unsigned short rows;
	unsigned short cols;
	unsigned char** pixles;
} grayImage;

void initImage(grayImage* img);/*Allocate a matrix to store all pixels of an image*/
void printImage(grayImage img);/*printing an image for testing*/
void insertRandomValues(grayImage* img);/*insert random values to the entire matrix for testing*/
grayImage* colorSegments(grayImage* img, imgPosCell** segments, unsigned int size);// TODO add description
void setMatrixValuesToZero(grayImage* img);/*set the entire matrix values to zero*/


BOOL isPixelInTree(grayImage booleanImage, imgPos positionToCheck);/*checks if a pixel was is already found within tree.*/
BOOL isColorWithInThreshold(unsigned char kerenelColor, unsigned char colorToCheck, unsigned char threshold);/*check if target color is within threshold range from kernel color*/
BOOL isPixelInImgBoundaries(grayImage img, imgPos positionToCheck);/*check if a position is within boundaries of the image*/
void markPixelInBooleanImage(grayImage* booleanImage, imgPos positionToMark);/*Mark a pixel as FOUND in the boolean image*/
treeNode* createTreeNode(imgPos positionToAdd); /*Create a tree node with provided position and an array of 8 treenode pointers*/


void getKernelSegmentAUX(treeNode* root, grayImage* booleanImage, grayImage img, unsigned char kernelColor, unsigned char threshold, int* treeCount);//TODO WRITE DESCRIPTION
Segment* getKernelSegment(Segment* seg, grayImage* booleanImage, grayImage img, imgPos kernel, unsigned char kernelColor, unsigned char threshold);//TODO WRITE DESCRIPTION