#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>

#include "service.h"
#include "common.h"

input_buffer_t* new_input_buffer() 
{
	input_buffer_t* input_buffer 	= malloc(sizeof(input_buffer));
	input_buffer->buffer 		= NULL;
	input_buffer->buffer_length = 0;
	input_buffer->input_length 	= 0;

	return input_buffer;
}

static void print_prompt() 
{
	printf("db> ");
}

static void read_input(input_buffer_t* initialize_input_buffer) 
{
	size_t bytes_read = getline(&(initialize_input_buffer->buffer), 
			&(initialize_input_buffer->buffer_length), stdin);

	initialize_input_buffer->input_length = bytes_read - 1;
	initialize_input_buffer->buffer[bytes_read - 1] = 0;
}

static void print_the_result(result_t res) 
{
	printf("%s\n", res.description);
}

int main(void) 
{

	input_buffer_t* initialize_input_buffer = new_input_buffer();

	while(1) {
		print_prompt();
		read_input(initialize_input_buffer);
		result_t res = execute(initialize_input_buffer);
		print_the_result(res);
	}
}
