#ifndef _STRUCTURE_H_ 
#define _STRUCTURE_H_ 

#define MAX_ELEMENTS 1000
#define COLUMN_DATA_SIZE 32
#define COLUMN_EMAIL_SIZE 255
#define TABLE_MAX_PAGES 100

typedef struct {
	uint32_t id;
	char data[COLUMN_DATA_SIZE];
} Row;

typedef struct {
	int8_t root;
	int32_t leaf;
	int32_t amount_of_keys;
	Row* info[MAX_ELEMENTS];
	uint32_t childs[MAX_ELEMENTS + 1];
} Page;

#endif
