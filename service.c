#include<stdlib.h>
#include<stdint.h>
#include<string.h>

#include "service.h"
#include "tree.h"
#include "structure.h"

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
} Execute_Result;

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

	int32_t id = atoi(id_string);
	if(id < 0) return PREPARE_NEGATIVE_ID;

	Row *row = (Row*) malloc(sizeof(Row));
	row->id = id;
	strcpy(row->value, value);

	statement->row_to_insert = row;

	return PREPARE_SUCCESS;
}

static Prepare_Result prepare_select(Input_Buffer* input_buffer, Statement* statement) {
	statement->type = STATEMENT_SELECT;

	char *keyword = strtok(input_buffer->buffer, " ");
	char *id_string = strtok(NULL, " ");

	if(id_string == NULL) return PREPARE_SYNTAX_ERROR;

	int32_t id = atoi(id_string);
	if(id < 1) return PREPARE_NEGATIVE_ID;

	return PREPARE_SUCCESS;
}

static Prepare_Result prepare_statement(Input_Buffer* input_buffer, Statement* statement) {
	if(is_insert_statement(input_buffer->buffer)) {
		return prepare_insert(input_buffer, statement);
	}
	if(is_select_statement(input_buffer->buffer)) {
		return prepare_select(input_buffer, statement);
	}
	return PREPARE_UNRECOGNIZED_STATEMENT;
}

static Execute_Result execute_insert(Statement* statement, Page *root) {
	return insert(root, statement->row_to_insert);
}

static Execute_Result execute_select(Statement* statement, Page *root) {
	return search(root, statement->wanted_element);
}

static Execute_Result execute_statement(Statement* statement) {
	Page *root = load_root();	
	switch (statement->type) {
		case (STATEMENT_INSERT):
			return execute_insert(statement, root);
		case (STATEMENT_SELECT):
			return execute_select(statement, root);
	}
}

Result execute(Input_Buffer* input_buffer) {
	Statement statement; 
	if(is_meta_command(input_buffer)) {
		return do_meta_command(input_buffer);
	}

	Result result;
	switch(prepare_statement(input_buffer, &statement)){
		case(PREPARE_SUCCESS):
			result.code = 1;
			break;
		case(PREPARE_SYNTAX_ERROR):
			result.description = "Syntax error. Could not parse statement.\n";
			result.code = 0;
			break;
		case(PREPARE_UNRECOGNIZED_STATEMENT):
			result.description = "Unrecognized keyword ar start .\n";
			result.code = 0;
			break;
		case(PREPARE_NEGATIVE_ID):
			result.description = "This id is negative\n";
			result.code = 0;
			break;
	}

	if(result.code)
		switch(execute_statement(&statement)){
			case(EXECUTE_SUCCESS):
				result.code = 1;
				result.description =  "Executed.";
				break;
			case (EXECUTE_DUPLICATE_KEY):
				result.code = 0;
				result.description = "Error: Duplicate key";
				break;
		}

	return result;
}
