#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<stdbool.h>
#include<stdint.h>
#include<string.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>
#include "REPL.h"
#include "b_tree.h"

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

void pager_flush(Pager* pager, uint32_t page_num) {
	if(pager->pages[page_num] == NULL) {
		printf("Tried to flush null page \n");
		exit(EXIT_FAILURE);
	}

	off_t offset = lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);

	if(offset == -1) {
		printf("Error seeking: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	ssize_t bytes_written = write(pager->file_descriptor, pager->pages[page_num], PAGE_SIZE);

	if(bytes_written == -1){
		printf("Error writing: %d\n", errno);
		exit(EXIT_FAILURE);
	}
}

void print_row(Row* row) {
	printf("(%d, %s, %s)\n", row->id, row->username, row->email);
}

InputBuffer* new_input_buffer() {
	InputBuffer* input_buffer 	= (InputBuffer*) malloc(sizeof(InputBuffer));
	input_buffer->buffer 		= NULL;
	input_buffer->buffer_length = 0;
	input_buffer->input_length 	= 0;

	return input_buffer;
}

void print_prompt() {
	printf("db> ");
}

void read_input(InputBuffer* input_buffer) {
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

void indent(uint32_t level){
	for(uint32_t i = 0; i < level; i++) {
		printf("  ");
	}
}

MetaCommandResult do_meta_command(InputBuffer* input_buffer, Page *page) {
	if(strcmp(input_buffer->buffer, ".exit") == 0){
		free(page); 
		exit(EXIT_SUCCESS);	
	} 
	else {
		return META_COMMAND_UNRECOGNIZED_COMMAND;	
	}
}

PrepareResult prepare_insert(InputBuffer* input_buffer, Statement* statement) {
	statement->type = STATEMENT_INSERT;

	char* keyword = strtok(input_buffer->buffer, " ");
	char* id_string = strtok(NULL, " ");
	char* username = strtok(NULL, " ");
	char* email = strtok(NULL, " ");

	if (id_string == NULL || username == NULL || email == NULL) {
		return PREPARE_SYNTAX_ERROR;
	}

	int id = atoi(id_string);
	if( id < 0) {
		return PREPARE_NEGATIVE_ID; 
	}

	if (strlen(username) > COLUMN_USERNAME_SIZE) {
		return PREPARE_STRING_TOO_LONG;
	}
	if (strlen(email) > COLUMN_EMAIL_SIZE) {
		return PREPARE_STRING_TOO_LONG;
	}

	statement->row_to_insert.id = id;
	strcpy(statement->row_to_insert.username, username);
	strcpy(statement->row_to_insert.email, email);

	return PREPARE_SUCCESS;
}

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement){
	if(strncmp(input_buffer->buffer, "insert", 6) == 0) {
		return prepare_insert(input_buffer, statement);	
	}
	if (strcmp(input_buffer->buffer, "select") == 0) {
		statement->type = STATEMENT_SELECT;
		return PREPARE_SUCCESS;	
	}
	if(strncmp(input_buffer->buffer, "create", 6) == 0) {
		statement->type = STATEMENT_CREATE_TABLE;
		return PREPARE_SUCCESS;
	}

	return PREPARE_UNRECOGNIZED_STATEMENT;
}

ExecuteResult execute_insert(Statement* statement, Table* table) {
	void* node = get_page(table->pager, table->root_page_num);
	uint32_t num_cells = *leaf_node_num_cells(node);

	Row* row_to_insert = &(statement->row_to_insert);
	uint32_t key_to_insert = row_to_insert->id;
	Cursor* cursor = table_find(table, key_to_insert);

	if(cursor->cell_num < num_cells) {
		uint32_t key_at_index = *leaf_node_key(node, cursor->cell_num);
		if(key_at_index == key_to_insert) {
			return EXECUTE_DUPLICATE_KEY;
		}
	}

	leaf_node_insert(cursor, row_to_insert->id, row_to_insert);
	free(cursor);

	return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Table* table) {
	Cursor* cursor = table_start(table);

	Row row;
	while(!(cursor->end_of_table)){
		deserialize_row(cursor_value(cursor), &row);
		print_row(&row);
		cursor_advance(cursor);
	}

	free(cursor);

	return EXECUTE_SUCCESS;
}

ExecuteResult execute_statement(Statement* statement, Table* table) {
	switch (statement->type) {
		case (STATEMENT_CREATE_TABLE):
			return execute_create_table(statement);
		case (STATEMENT_INSERT):
			return execute_insert(statement, table);
		case (STATEMENT_SELECT):
			return execute_select(table);
	}
}

Pager* pager_open(const char* filename) {
	int fd = open(filename, 
			O_RDWR |
			O_CREAT,
			S_IWUSR |
			S_IRUSR 
			);

	if(fd == -1) {
		printf("Unable to open or create file \n"); 
		exit(EXIT_FAILURE);
	}

	off_t file_length = lseek(fd, 0, SEEK_END);

	Pager* pager = (Pager*) malloc(sizeof(Pager));
	pager->file_descriptor = fd;
	pager->file_length = file_length;
	pager->num_pages = (file_length / PAGE_SIZE);

	if (file_length % PAGE_SIZE != 0) {
		printf("Db file is not a whole number of pages. Corrupt file. \n"); 
		exit(EXIT_FAILURE);
	}

	for(uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
		pager->pages[i] = NULL;
	}

	return pager;
}

Table* db_open(const char* filename) {
	Pager* pager = pager_open(filename);

	Table* table = (Table*) malloc(sizeof(Table));
	table->pager = pager;

	table->root_page_num = 0;

	if(pager->num_pages == 0) {
		void* root_node = get_page(pager, 0);
		initialize_leaf_node(root_node);
		set_node_root(root_node, true);
	}

	return table; 
}

int main(int argc, char* argv[]) {
	if(argc < 2) {
		printf("Must supply a database filename \n");
		exit(EXIT_FAILURE);
	}

	char* filename = argv[1];
	Table* table = db_open(filename);

	InputBuffer* input_buffer = new_input_buffer();

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
			case(PREPARE_STRING_TOO_LONG):
				printf("String is too long\n");
				continue; 
			case(PREPARE_SUCCESS):
				break;
			case(PREPARE_SYNTAX_ERROR):
				printf("Syntax error. Could not parse statement. \n");
				continue;
			case(PREPARE_UNRECOGNIZED_STATEMENT):
				printf("Unrecognized keyword ar start of '%s'.\n", input_buffer->buffer);
				continue;
			case(PREPARE_NEGATIVE_ID):
				printf("This id is negative \n");
				continue;
		}

		switch(execute_statement(&statement, table)){
			case(EXECUTE_SUCCESS):
				printf("Executed \n");
				break;
			case(EXECUTE_TABLE_FULL):
				printf("Error: Table full. \n");
				break;
			case (EXECUTE_DUPLICATE_KEY):
				printf("Error: Duplicate key \n");
				break;
		}
	}
}
