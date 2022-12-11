#include <gtest/gtest.h>

extern "C" {
#include "service.c"
}

TEST(Service, is_meta_command_RETURN_TRUE) {
	Input_Buffer* ib = (Input_Buffer*) malloc(sizeof(Input_Buffer));
	ib->buffer = ".exit";
	EXPECT_TRUE(is_meta_command(ib));
}

TEST(Service, is_meta_command_RETURN_FALSE) {
	Input_Buffer* ib = (Input_Buffer*) malloc(sizeof(Input_Buffer));
	ib->buffer = "exit";
	EXPECT_FALSE(is_meta_command(ib));
}

TEST(Service, do_meta_command__RETURN_ERROR) {
	Input_Buffer* ib = (Input_Buffer*) malloc(sizeof(Input_Buffer));
	ib->buffer = ".aaa";
	const Result result = do_meta_command(ib);
	EXPECT_EQ(result.description, "ERROR: meta command does not exist\n");
}

TEST(Service, prepare_statement__RETURN_SELECT) {
	Input_Buffer* ib = (Input_Buffer*) malloc(sizeof(Input_Buffer));
	ib->buffer = "select";
	EXPECT_EQ(prepare_statement(ib), "select");
}

TEST(Service, prepare_statement__RETURN_INSERT) {
	Input_Buffer* ib = (Input_Buffer*) malloc(sizeof(Input_Buffer));
	ib->buffer = "insert";
	EXPECT_EQ(prepare_statement(ib), "insert");
}