#include<stdlib.h>
#include<stdint.h>
#include<string.h>

#include "service.h"
#include "store.h"
#include "common.h"

typedef enum {
	PREPARE_SUCCESS,
	PREPARE_UNRECOGNIZED_STATEMENT,
	PREPARE_SYNTAX_ERROR, 
	PREPARE_STRING_TOO_LONG, 
	PREPARE_NEGATIVE_ID
} prepare_result_t;

typedef enum {
	STATEMENT_INSERT, 
	STATEMENT_SELECT
} statement_type_e;

typedef struct {
	statement_type_e type;
	row_t *row_to_insert;
	uint32_t wanted_element;
} statement_t;

typedef enum {
	EXECUTE_SUCCESS, 
	EXECUTE_TABLE_FULL, 
	EXECUTE_DUPLICATE_KEY,
	UNKNOWN_ERROR
} execute_result_e;

void do_meta_command(input_buffer_t* input_buffer) 
{
	if(strcmp(input_buffer->buffer, ".exit") == 0)
		exit(EXIT_SUCCESS);	
}

static uint8_t is_meta_command(input_buffer_t* input_buffer) 
{
	return input_buffer->buffer[0] == '.';
}

static uint8_t is_insert_statement(char* str) 
{
	return strncmp(str, "insert", 6) == 0;
} 

static uint8_t is_select_statement(char* str) 
{
	return strncmp(str, "select", 6) == 0;
} 

static prepare_result_t prepare_insert(
		input_buffer_t* input_buffer, statement_t* s) 
{
	s->type = STATEMENT_INSERT;

	char* keyword = strtok(input_buffer->buffer, " ");
	char* id_string = strtok(NULL, " ");
	char* value = strtok(NULL, " ");

	if(id_string == NULL || value == NULL) 
		return PREPARE_SYNTAX_ERROR;

	int32_t id = atoi(id_string);
	if(id < 0) return PREPARE_NEGATIVE_ID;

	row_t *r= (row_t*) malloc(sizeof(row_t));
	r->id = id;
	strcpy(r->data, value);

	s->row_to_insert = r;

	return PREPARE_SUCCESS;
}

static prepare_result_t prepare_select(
		input_buffer_t* input_buffer, statement_t* s) 
{
	s->type = STATEMENT_SELECT;

	char *keyword = strtok(input_buffer->buffer, " ");
	char *id_string = strtok(NULL, " ");

	if(id_string == NULL) return PREPARE_SYNTAX_ERROR;

	int32_t id = atoi(id_string);
	if(id < 1) return PREPARE_NEGATIVE_ID;
	s->wanted_element = id;

	return PREPARE_SUCCESS;
}

static prepare_result_t prepare_statement(
		input_buffer_t* input_buffer, statement_t* s) 
{
	if(is_insert_statement(input_buffer->buffer))
		return prepare_insert(input_buffer, s);

	if(is_select_statement(input_buffer->buffer))
		return prepare_select(input_buffer, s);
	
	return PREPARE_UNRECOGNIZED_STATEMENT;
}

static result_t execute_insert(statement_t* s) 
{
	return insert(s->row_to_insert);
}

static result_t execute_select(statement_t* s) 
{
	return search(s->wanted_element);
}

static result_t execute_statement(statement_t* s) 
{
	result_t res;	
	switch (s->type) {
		case (STATEMENT_INSERT):
			res = execute_insert(s);
			break;
		case (STATEMENT_SELECT):
			res = execute_select(s);
			break;
		default:
			res.code = 0;
			res.description = "unknown error";
			break;
	}

	return res;
}

result_t execute(input_buffer_t* ib) 
{
	if(is_meta_command(ib))
		do_meta_command(ib);

	statement_t s; 
	result_t res;
	switch(prepare_statement(ib, &s)){
		case(PREPARE_SUCCESS):
			res.code = 1;
			break;
		case(PREPARE_SYNTAX_ERROR):
			res.description = "Syntax error. Could not parse statement.\n";
			res.code = 0;
			break;
		case(PREPARE_UNRECOGNIZED_STATEMENT):
			res.description = "Unrecognized keyword at start .\n";
			res.code = 0;
			break;
		case(PREPARE_NEGATIVE_ID):
			res.description = "This id is negative\n";
			res.code = 0;
			break;
		case(PREPARE_STRING_TOO_LONG):
			res.description = "This string too long";
			res.code = 0;
			break;
	}

	if(res.code) 
		return execute_statement(&s);

	return res;
}
