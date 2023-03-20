#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<stdlib.h>

#include "structure.h"
#include "tree.h"
#include "disk_operation.h"

#define MAX_SIZE_ARR 256

uint32_t integer_array_for_test_mock[MAX_SIZE_ARR];
char *values_array_for_test_mock[MAX_SIZE_ARR];

Row* search(Page *page, int wanted_element) {
    for(uint32_t index = 0; index < MAX_SIZE_ARR; index++) {
        if(*(integer_array_for_test_mock + index) == wanted_element) {
            char *value = *(values_array_for_test_mock + index);
            Row *row = malloc(sizeof(Row));
	        strcpy(row->data, value);
            row->id = index;
            return row;
        }
    }
}

Page* load_root() {
    return create();
}

Page* create() {
    for(uint32_t index = 0; index < MAX_SIZE_ARR; index++) {
        *(integer_array_for_test_mock + index) = NULL;
        *(values_array_for_test_mock + index) = "";
    }
}

int insert(Page *root, Row *k) {
    for(uint32_t index = 0; index < MAX_SIZE_ARR; index++) {
        if(*(integer_array_for_test_mock + index) == NULL) {
            values_array_for_test_mock[index] = k->data;
            *(integer_array_for_test_mock + index) = ++index;
            return index;
        }
    }
}