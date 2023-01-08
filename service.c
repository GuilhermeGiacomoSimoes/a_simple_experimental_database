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
} Prepare_Result;

typedef enum {
	STATEMENT_INSERT, 
	STATEMENT_SELECT
} StatementType;

typedef struct {
	uint32_t id;
	char value[256];
} Row;

typedef struct {
	StatementType type;
	Row *row_to_insert;
	uint32_t wanted_element;
} Statement;

typedef enum {
	EXECUTE_SUCCESS, 
	EXECUTE_TABLE_FULL, 
	EXECUTE_DUPLICATE_KEY,
} ExecuteResult;

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

static uint8_t is_insert_statement(char* str) {
	return strncmp(str, "insert", 6) == 0;
} 

static uint8_t is_select_statement(char* str) {
	return strncmp(str, "select", 6) == 0;
} 

static Prepare_Result prepare_insert(Input_Buffer* input_buffer, Statement* statement) {
	statement->type = STATEMENT_INSERT;

	char* keyword = strtok(input_buffer->buffer, " ");
	char* id_string = strtok(NULL, " ");
	char* value = strtok(NULL, " ");

	if(id_string == NULL || value == NULL) return PREPARE_SYNTAX_ERROR;

	int id = atoi(id_string);
	if(id < 0) return PREPARE_NEGATIVE_ID;

	Row *row = (Row*) malloc(sizeof(Row));
	row->id = id;
	strcpy(row->value, value);

	statement->row_to_insert = row;

	return PREPARE_SUCCESS;
}

static Prepare_Result prepare_select(Input_Buffer* input_buffer, Statement* statement) {
	statement->type = STATEMENT_SELECT;
	return PREPARE_SUCCESS;
}

static Prepare_Result prepare_statement(Input_Buffer* input_buffer, Statement* statement) {
	if(is_insert_statement(input_buffer->buffer)){
		return prepare_insert(input_buffer, statement);
	}
	if(is_select_statement(input_buffer->buffer)) {
		return prepare_select(input_buffer, statement);
	}
	return PREPARE_UNRECOGNIZED_STATEMENT;
}

ExecuteResult execute_insert(Statement* statement) {
	return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement* statement) {
	return EXECUTE_DUPLICATE_KEY;
}

ExecuteResult execute_statement(Statement* statement) {
	switch (statement->type) {
		case (STATEMENT_INSERT):
			return execute_insert(statement);
		case (STATEMENT_SELECT):
			return execute_select(statement);
	}
}

Result execute(Input_Buffer* input_buffer) {
	Statement statement; 
	if(is_meta_command(input_buffer)) {
		return do_meta_command(input_buffer);
	}

	prepare_statement(input_buffer, &statement);
	//execute_statement();
	Result result;
	result.code = 1;
	result.description = "tudo certo";

	return result;
}
