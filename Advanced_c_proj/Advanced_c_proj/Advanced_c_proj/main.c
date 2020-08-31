#define _CRT_SECURE_NO_WARNINGS
#include "grayImage.h"
#include "imgPosCell.h"
#include "segment.h"
#include <stdlib.h>
void main() {

	grayImage testimage; /*creating a test image*/
	testimage.cols = testimage.rows = 10;
	initImage(&testimage);
	printImage(testimage);
}
