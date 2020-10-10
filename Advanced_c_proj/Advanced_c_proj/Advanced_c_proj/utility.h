#ifndef ___Utility___
#define ___Utility___
#include "grayImage.h"


/********* DEFINES **********/
#define TRUE 1
#define FALSE 0
#define MEMORY_ALLOCATION_FAILED -1

/********* TYPEDEF **********/
typedef char BOOL;

/***** FUNCTION PROTOTYPES *****/
void checkMalloc(void* pointer);/*If memory allocation failed the program will exit*/
void checkFileOpen(void* ptr);/*check File Open*/
#endif