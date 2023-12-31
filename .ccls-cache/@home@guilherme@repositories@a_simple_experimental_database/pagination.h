#ifndef _STORE_H_ 
#define _STORE_H_ 

#define MAX_ELEMENTS 55 
#define COLUMN_DATA_SIZE 32

typedef struct {
	uint32_t id;
	char data[COLUMN_DATA_SIZE];
} row;

typedef struct {
	row* info[MAX_ELEMENTS];
} page;

result insert(char* data);
result search(uint32_t we);

#endif
