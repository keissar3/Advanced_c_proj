#include <stdio.h>
#include <stdlib.h>
#include "Utility.h"

void checkMalloc(void* pointer)
{
	if (!pointer)
	{
		printf("ERROR: MEMORY ALLOCATION FAILED !!!");
		exit(MEMORY_ALLOCATION_FAILED);
	}

}