#ifndef _STRUCTURE_H_ 
#define _STRUCTURE_H_ 

#define MAX_ELEMENTS 1000
#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255
#define TABLE_MAX_PAGES 100

typedef struct {
	uint32_t id;
	char username[COLUMN_USERNAME_SIZE + 1];
} Row;

typedef struct {
	int8_t root;
	int32_t leaf;
	int32_t amount_of_keys;
	Row* info[MAX_ELEMENTS];
	uint32_t childs[MAX_ELEMENTS + 1];
} Page;

typedef struct {
    char p1[3 * sizeof(uint32_t)];
    char p2[MAX_ELEMENTS * sizeof(uint32_t)];
    struct {
        char data[sizeof(Row)];
    } infos[MAX_ELEMENTS];
} Page_data;

#endif
