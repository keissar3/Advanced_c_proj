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
treeNode* createTreeNode(imgPos positionToAdd); /*Create a tree node with provided position and an array of 8 treenode pointers*/
Segment* findSingleSegment(grayImage* img, imgPos kernel, unsigned char threshold);//todo add desctiption







