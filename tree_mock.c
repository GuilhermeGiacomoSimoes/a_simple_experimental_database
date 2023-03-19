#include<stdio.h>
#include<stdint.h>

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
            row->data = value;
            row->id = index;
            return row;
        }
    }
}

Page* load_root() {
    return NULL;
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
            *(integer_array_for_test_mock + index) = ++index;
            values_array_for_test_mock[index] = k->data;
            return index;
        }
    }
}