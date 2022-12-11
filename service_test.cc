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
	Statement statemet;
	EXPECT_EQ(prepare_statement(ib, &statemet), "select");
}

TEST(Service, prepare_statement__RETURN_INSERT) {
	Input_Buffer* ib = (Input_Buffer*) malloc(sizeof(Input_Buffer));
	ib->buffer = "insert";
	Statement statement;
	EXPECT_EQ(prepare_statement(ib, &statement), "insert");
}

TEST(Service, is_insert_statement__RETURN_TRUE) {
	EXPECT_TRUE(is_insert_statement("insert"));
}

TEST(Service, is_insert_statement__RETURN_FALSE) {
	EXPECT_FALSE(is_insert_statement("inseot"));
}

TEST(Service, is_select_statement__RETURN_TRUE) {
	EXPECT_TRUE(is_select_statement("select"));
}

TEST(Service, is_select_statement__RETURN_FALSE) {
	EXPECT_FALSE(is_select_statement("inseot"));
}
