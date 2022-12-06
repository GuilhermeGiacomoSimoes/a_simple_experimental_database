#ifndef __SERVICE_H__
#define __SERVICE_H__

typedef struct {
	char* 	buffer;
	size_t 	buffer_length;
	ssize_t input_length;
} Input_Buffer;

typedef struct {
	char* description;
	uint8_t code;
} Result;

Result execute(Input_Buffer* input_buffer);

#endif
