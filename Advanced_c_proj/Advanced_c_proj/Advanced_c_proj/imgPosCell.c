#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Utility.h"
#include "grayImage.h"
#include "imgPosCell.h"
#include "segment.h"

/***** Function Implementation *****/
imgPosCell* creatImgPosCellNode(imgPos positionToAdd) {
	imgPosCell* res;
	res = (imgPosCell*)malloc(sizeof(imgPosCell));
	res->position[0] = positionToAdd[0];
	res->position[1] = positionToAdd[1];
	res->next = res->prev = NULL;
	return res;
}

imgPosCell* findPlaceToInsert(imgPosCell* list, imgPos position) {
	imgPosCell* curr = list, * prev = NULL;
	while (curr && (curr->position[1] < position[1])) {
		prev = curr;
		curr = curr->next;
	}
	while (curr && (curr->position[1] == position[1]) && ((curr->position[0] < position[0]))) {
		prev = curr;
		curr = curr->next;
	}
	return prev;
}

void addToBeginningOfList(imgPosCell** list, imgPosCell* node_to_add) {
	node_to_add->next = *list;
	(*list)->prev = node_to_add;
	(*list) = node_to_add;
}

void addToInnerPlaceInList(imgPosCell* prev, imgPosCell* node_to_add) {
	if (prev->next == NULL) {
		node_to_add->prev = prev;
		node_to_add->next = prev->next;
		prev->next = node_to_add;
	}
	else {
		node_to_add->prev = prev;
		node_to_add->next = prev->next;
		prev->next = node_to_add;
		node_to_add->next->prev = node_to_add;
	}
}