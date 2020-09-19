#include <stdio.h>
#include <stdlib.h>
#include "Utility.h"
#include "grayImage.h"
#include "imgPosCell.h"
#include "segment.h"

void checkMalloc(void* pointer)
{
	if (!pointer)
	{
		printf("ERROR: MEMORY ALLOCATION FAILED !!!");
		exit(MEMORY_ALLOCATION_FAILED);
	}
}

