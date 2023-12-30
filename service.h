#ifndef __SERVICE_H__
#define __SERVICE_H__

typedef struct {
	char* 	buffer;
	size_t 	buffer_length;
	ssize_t input_length;
} input_buffer;

result execute(input_buffer* input_buffer);

#endif
