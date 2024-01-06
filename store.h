#ifndef _STORE_H_ 
#define _STORE_H_ 

#define MAX_ELEMENTS 55 
#define COLUMN_DATA_SIZE 32

#include "common.h"

typedef struct {
	uint32_t id;
	char data[COLUMN_DATA_SIZE];
} row_t;

typedef struct {
	row_t* info[MAX_ELEMENTS];
} page_t;

result_t insert(row_t* data);
result_t search(uint32_t we);

#endif
