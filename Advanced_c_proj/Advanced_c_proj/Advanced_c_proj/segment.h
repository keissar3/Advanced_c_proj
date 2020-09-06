#pragma once
typedef unsigned short imgPos[2];
#define MAX_NEIGHBORS 8

typedef struct _treeNode {                /*a tree is used to store all positions of pixel, and their negihbors
											within single segments */
	imgPos position;                      /*position of a single pixel within the segment*/
	struct _treeNode** similar_neighbors; /*Array of children / neighbors*/
}treeNode;

typedef struct _Segment {
	treeNode* root;   /*the kernel of the segment*/
	unsigned int size;/*the total number of tree nodes*/
}Segment;