#ifndef __SERVICE_H__
#define __SERVICE_H__

#include "common.h"

typedef struct {
	char* 	buffer;
	size_t 	buffer_length;
	ssize_t input_length;
} input_buffer_t;

result_t execute(input_buffer_t* input_buffer);

#endif
