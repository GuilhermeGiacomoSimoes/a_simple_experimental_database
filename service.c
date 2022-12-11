#include<stdlib.h>
#include<stdint.h>
#include<string.h>

#include "service.h"

typedef enum {
	PREPARE_SUCCESS,
	PREPARE_UNRECOGNIZED_STATEMENT,
	PREPARE_SYNTAX_ERROR, 
	PREPARE_STRING_TOO_LONG, 
	PREPARE_NEGATIVE_ID
} PrepareResult;

Result do_meta_command(Input_Buffer* input_buffer) {
	if(strcmp(input_buffer->buffer, ".exit") == 0){
		exit(EXIT_SUCCESS);	
	}

	Result *result = (Result*) malloc(sizeof(Result));
	result->description = "ERROR: meta command does not exist\n";
	result->code = 0;
	return *(result);
}

static uint8_t is_meta_command(Input_Buffer* input_buffer) {
	return input_buffer->buffer[0] == '.';
}

Result execute(Input_Buffer* input_buffer) {
	if(is_meta_command(input_buffer)) {
		return do_meta_command(input_buffer);
	}

	prepare_statement(input_buffer);
	//execute_statement();
}

char* prepare_statement(Input_Buffer* input_buffer) {
	if(strncmp(input_buffer->buffer, "insert", 6) == 0){
		return "insert";
	}
	if(strncmp(input_buffer->buffer, "select", 6) == 0) {
		return "select";
	}
	//return prepare_select(input_buffer);
}
