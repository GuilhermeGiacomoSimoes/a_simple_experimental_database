#ifndef _STRUCTURE_H_ 
#define _STRUCTURE_H_ 

#define MAX_ELEMENTS 55 
#define COLUMN_DATA_SIZE 32

typedef struct {
	uint32_t id;
	char data[COLUMN_DATA_SIZE];
} row;

typedef struct {
	row* info[MAX_ELEMENTS];
} page;

#endif
