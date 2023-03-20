#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>

#include "service.h"

Input_Buffer* new_input_buffer() {
	Input_Buffer* input_buffer 	= (Input_Buffer*) malloc(sizeof(Input_Buffer));
	input_buffer->buffer 		= NULL;
	input_buffer->buffer_length = 0;
	input_buffer->input_length 	= 0;

	return input_buffer;
}

static void print_prompt() {
	printf("db> ");
}

static void read_input(Input_Buffer* initialize_input_buffer) {
	size_t bytes_read = getline(&(initialize_input_buffer->buffer), &(initialize_input_buffer->buffer_length), stdin);
	initialize_input_buffer->input_length = bytes_read - 1;
	initialize_input_buffer->buffer[bytes_read - 1] = 0;
}

static void print_the_result(Result result) {
	printf("%s\n", result.description);
}

int main() {

	Input_Buffer* initialize_input_buffer = new_input_buffer();

	while(1) {
		print_prompt();
		read_input(initialize_input_buffer);
		Result result = execute(initialize_input_buffer);
		print_the_result(result);
	}
}
