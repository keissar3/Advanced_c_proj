#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Utility.h"
#include "grayImage.h"
#include "imgPosCell.h"
#include "segment.h"

/***** Function Implementation *****/
treeNode* createTreeNode(imgPos positionToAdd)
{
	int i;
	treeNode* res = (treeNode*)malloc(sizeof(treeNode));
	checkMalloc(res);
	res->position[0] = positionToAdd[0];
	res->position[1] = positionToAdd[1];
	res->similar_neighbors = (treeNode**)malloc(((MAX_NEIGHBORS + 1) * sizeof(treeNode*)));
	checkMalloc(res->similar_neighbors);
	for (i = 0; i < MAX_NEIGHBORS + 1; i++)
		res->similar_neighbors[i] = NULL;

	return res;
}

//void freeSegment(Segment* seg) {
//
//}