#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<stdlib.h>
#include<stdarg.h>

#include "structure.h"
#include "tree.h"
#include "disk_operation.h"

#define MAX_SIZE_ARR 256

Row* search(Page *page, int wanted_element, ...) {
    va_list valist;
    va_start(valist, 2);

    uint32_t *integer_array_for_test_mock = va_arg(valist, uint32_t*);
    char** values_array_for_test_mock = va_arg(valist, char**);

    Row *row = (Row*) malloc(sizeof(Row));
    row->id = 0;
    strcpy(row->data, "not found");

    for(uint32_t index = 0; index < MAX_SIZE_ARR; index++) {
        if(*(integer_array_for_test_mock + index) == wanted_element) {
            char *value = *(values_array_for_test_mock + index);
	        strcpy(row->data, value);
            row->id = index;
            return row;
        }
    }

    va_end(valist);
    return row;
}

Page* load_root() {
    //return create();
    return NULL;
}

Page* create() {
    //for(uint32_t index = 0; index < MAX_SIZE_ARR; index++) {
    //    *(integer_array_for_test_mock + index) = 0;
    //    *(values_array_for_test_mock + index) = "";
    //}

    return NULL;
}

int insert(Page *root, Row *k, ...) {
    va_list valist;
    va_start(valist, 2);

    uint32_t *integer_array_for_test_mock = va_arg(valist, uint32_t*);
    char** values_array_for_test_mock = va_arg(valist, char**);

    for(uint32_t index = 0; index < MAX_SIZE_ARR; index++) {
        if(*(integer_array_for_test_mock + index) == 0) {
            values_array_for_test_mock[index] = k->data;
            *(integer_array_for_test_mock + index) = ++index;
            return index;
        }
    }
}