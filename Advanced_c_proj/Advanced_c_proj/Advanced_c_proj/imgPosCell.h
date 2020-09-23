#pragma once
#include "segment.h"
typedef struct _imgPosCell { /*linked list to store all pixel's locations in a single segment*/
	imgPos position;
	struct _imgPosCell* next;
	struct _imgPosCell* prev;
}imgPosCell;

/***** FUNCTION PROTOTYPES *****/

imgPosCell* creatImgPosCellNode(imgPos positionToAdd);
imgPosCell* findPlaceToInsert(imgPosCell* list, imgPos position);
void addToBeginningOfList(imgPosCell** list, imgPosCell* node_to_add);
void addToInnerPlaceInList(imgPosCell* prev, imgPosCell* node_to_add);
void freeImgPosCell(imgPosCell*** segments, int size);