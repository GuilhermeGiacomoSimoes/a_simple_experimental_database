#include<iostream>
#include "main.c";

int TEST_do_meta_command_exit() {
	InputBuffer *input_buffer = malloc(sizeof(InputBuffer));
	input_buffer->bugger = ".exit";
	Page *page = malloc(page);

	MetaCommandResult meta_command_result = do_meta_command(input_buffer, page);

	return meta_command_result != META_COMMAND_UNRECOGNIZED_COMMAND;
}

int main() {
	return TEST_do_meta_command_exit();
}
