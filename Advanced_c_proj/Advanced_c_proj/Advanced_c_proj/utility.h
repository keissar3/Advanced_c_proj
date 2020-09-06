#pragma once
#define TRUE 1
#define FALSE 0
#define MEMORY_ALLOCATION_FAILED -1
typedef char BOOL;
void checkMalloc(void* pointer);/*If memory allocation failed the program will exit*/