#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<stdint.h>
#include<string.h>

#define  COLUMN_USERNAME_SIZE 	32
#define  COLUMN_EMAIL_SIZE    	255 
#define  TABLE_MAX_PAGES 		100 
#define  size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

typedef struct {
	uint32_t id;
	char username[COLUMN_USERNAME_SIZE + 1];
	char email[COLUMN_EMAIL_SIZE + 1];
} Row;

const uint32_t PAGE_SIZE        = 4096;

const uint32_t ID_SIZE 			= size_of_attribute(Row, id);
const uint32_t USERNAME_SIZE 	= size_of_attribute(Row, username);
const uint32_t EMAIL_SIZE 		= size_of_attribute(Row, email);

const uint32_t ID_OFFSET 		= 0; 
const uint32_t USERNAME_OFFSET 	= ID_OFFSET + ID_SIZE; 
const uint32_t EMAIL_OFFSET 	= USERNAME_OFFSET + USERNAME_SIZE; 
const uint32_t ROW_SIZE 		= EMAIL_OFFSET + EMAIL_SIZE; 

const uint32_t ROWS_PER_PAGE  = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

typedef struct {
	uint32_t num_rows;
	void* pages[TABLE_MAX_PAGES];
} Table; 

//void * memcpy ( void * destination, const void * source, size_t num );
void serialize_row(Row* source, void* destination) {
  memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
  memcpy(destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
  memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}

void deserialize_row(void *source, Row* destination) {
  memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
  memcpy(&(destination->username), source + USERNAME_OFFSET, USERNAME_SIZE);
  memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}

void* row_slot(Table* table, uint32_t row_num) {
	uint32_t page_num = row_num / ROWS_PER_PAGE;
	void* page = table->pages[page_num];

	if (page == NULL) {
		page = table->pages[page_num] = malloc(PAGE_SIZE);	
	}

	uint32_t row_offset  = row_num    % ROWS_PER_PAGE;
	uint32_t byte_offset = row_offset * ROW_SIZE; 

	return page + byte_offset;
}

void print_row(Row* row) {
	printf("(%d, %s, %s)\n", row->id, row->username, row->email);
}

typedef enum {
	EXECUTE_SUCCESS, 
	EXECUTE_TABLE_FULL
} ExecuteResult;

typedef enum {
	META_COMMAND_SUCCESS,
	META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
	PREPARE_SUCCESS,
	PREPARE_UNRECOGNIZED_STATEMENT,
	PREPARE_SYNTAX_ERROR, 
	PREPARE_STRING_TOO_LONG
} PrepareResult;

typedef enum {
	STATEMENT_INSERT, 
	STATEMENT_SELECT
} StatementType;

typedef struct {
	StatementType type;
	Row row_to_insert;
} Statement;

typedef struct {
	char* 	buffer;
	size_t 	buffer_length;
	ssize_t input_length;
} InputBuffer;

InputBuffer* new_input_buffer() {
	InputBuffer* input_buffer 	= malloc(sizeof(InputBuffer));
	input_buffer->buffer 		= NULL;
	input_buffer->buffer_length = 0;
	input_buffer->input_length 	= 0;

	return input_buffer;
}

void print_prompt() {
	printf("db> ");
}

void read_input(InputBuffer* input_buffer){
	ssize_t bytes_read = getline( &(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

	if (bytes_read <= 0) {
		printf("Error reading input \n");
		exit(EXIT_FAILURE);
	}

	input_buffer->input_length = bytes_read - 1;
	input_buffer->buffer[bytes_read - 1] = 0;
}

void close_input_buffer(InputBuffer* input_buffer) {
	free(input_buffer->buffer);
	free(input_buffer);
}

void free_table(Table* table) {
	for (int i = 0; table->pages[i]; i++) {
		free(table->pages[i]);	
	}
	free(table);
}

MetaCommandResult do_meta_command(InputBuffer* input_buffer, Table *table) {
	if(strcmp(input_buffer->buffer, ".exit") == 0){
		close_input_buffer(input_buffer);
		free_table(table);
		exit(EXIT_SUCCESS);	
	}else {
		return META_COMMAND_UNRECOGNIZED_COMMAND;	
	}
}

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement){
	if(strncmp(input_buffer->buffer, "insert", 6) == 0) {
		return prepare_insert(input_buffer, statement);	
	}
	if (strcmp(input_buffer->buffer, "select") == 0) {
		statement->type = STATEMENT_SELECT;
		return PREPARE_SUCCESS;	
	}

	return PREPARE_UNRECOGNIZED_STATEMENT;
}

ExecuteResult execute_insert(Statement* statement, Table* table) {
  if (table->num_rows >= TABLE_MAX_ROWS) {
    return EXECUTE_TABLE_FULL;
  }
  else {
	  Row* row_to_insert = &(statement->row_to_insert);

	  serialize_row(row_to_insert, row_slot(table, table->num_rows));
	  table->num_rows += 1;

	  return EXECUTE_SUCCESS;
  }
}

ExecuteResult execute_select(Statement* statement, Table* table) {
	Row row;
	for (uint32_t i=0; i < table->num_rows; i++) {
		deserialize_row(row_slot(table, i), &row);	
		print_row(&row);
	}

	return EXECUTE_SUCCESS;
}

 ExecuteResult execute_statement(Statement* statement, Table* table) {
	switch (statement->type) {
		case (STATEMENT_INSERT):
			return execute_insert(statement, table);
		case (STATEMENT_SELECT):
			return execute_select(statement, table);
	}
}

Table* new_table() {
	Table* table = malloc(sizeof(Table));
	table->num_rows = 0;
	for(uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
		table->pages[i] = NULL;	
	}
	return table;
}


//void insert_from_table_max_rows(Table *table){
//
//	Statement statement;
//	statement.type = STATEMENT_INSERT; 
//
//	uint32_t id;
//	char username[COLUMN_USERNAME_SIZE];
//	char email[COLUMN_EMAIL_SIZE];
//
//	for (int i = 1; i <= TABLE_MAX_ROWS + 1; i++){
//		Row row;
//		row.id = i;
//		row.username[0] = 'n'; 
//		row.username[1] = 'a'; 
//		row.username[2] = 'm'; 
//		row.username[3] = 'e'; 
//
//		row.email[0] = 'e'; 
//		row.email[1] = 'm'; 
//		row.email[2] = 'a'; 
//		row.email[3] = 'i'; 
//		row.email[4] = 'l'; 
//
//		statement.row_to_insert = row; 
//		switch(execute_insert(&statement, table)){
//			case EXECUTE_SUCCESS:
//				printf("sucess\n");
//				continue; 
//			case EXECUTE_TABLE_FULL:
//				printf("table full\n");
//				continue;
//		}
//	}
//}

int main(int argc, char* argv[]) {
	Table* table = new_table();
	InputBuffer* input_buffer = new_input_buffer();

	//insert_from_table_max_rows(table);

	while(true) {
		print_prompt();
		read_input(input_buffer);

		if(input_buffer->buffer[0] == '.') {
			switch(do_meta_command(input_buffer, table)) {
				case (META_COMMAND_SUCCESS):	
					continue;
				case (META_COMMAND_UNRECOGNIZED_COMMAND):
					printf("Unrecognized command '%s'\n", input_buffer->buffer);
					continue;
			}
		}

		Statement statement;
		switch(prepare_statement(input_buffer, &statement)){
			case(PREPARE_SUCCESS):	
				break;
			case(PREPARE_SYNTAX_ERROR):
				printf("Syntax error. Could not parse statement. \n");
				continue;
			case(PREPARE_UNRECOGNIZED_STATEMENT):
				printf("Unrecognized keyword ar start of '%s'.\n", input_buffer->buffer);
				continue;
		}

		switch(execute_statement(&statement, table)){
			case(EXECUTE_SUCCESS):
				printf("Executed \n");
				break;
			case(EXECUTE_TABLE_FULL):
				printf("Error: Table full. \n");
				break;
		}
	}
}
