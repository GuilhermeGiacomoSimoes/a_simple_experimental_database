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
	ib->buffer = "select 1";
	Statement statemet;
	prepare_statement(ib, &statemet);
	EXPECT_EQ(statemet.type, STATEMENT_SELECT);
}

//TEST(Service, prepare_statement__RETURN_INSERT) {
//	Input_Buffer* ib = (Input_Buffer*) malloc(sizeof(Input_Buffer));
//	ib->buffer = "insert 1 guilherme";
//	ib->buffer_length = sizeof(ib->buffer);
//	ib->input_length = sizeof(ib->buffer);
//	Statement statemet;
//	prepare_statement(ib, &statemet);
//	EXPECT_EQ(statemet.type, STATEMENT_INSERT);
//}

TEST(Service, prepare_statement__RETURN_UNRECOGNIZED_STATEMENT) {
	Input_Buffer* ib = (Input_Buffer*) malloc(sizeof(Input_Buffer));
	ib->buffer = "taylor";
	Statement statemet;
	EXPECT_EQ(prepare_statement(ib, &statemet), PREPARE_UNRECOGNIZED_STATEMENT);
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

//TEST(Service, prepare_insert__RETURN_PREPARE_SUCCESS) {
//	Input_Buffer* ib = (Input_Buffer*) malloc(sizeof(Input_Buffer));
//	ib->buffer = "insert 1 guilherme";
//	Statement statement;
//	EXPECT_EQ(prepare_insert(ib, &statement), PREPARE_SUCCESS);
//}

TEST(Service, prepare_insert__PREPARE_SYNTAX_ERROR) {
	Input_Buffer* ib = (Input_Buffer*) malloc(sizeof(Input_Buffer));
	ib->buffer = "insert";
	Statement statement;
	EXPECT_EQ(prepare_insert(ib, &statement), PREPARE_SYNTAX_ERROR);
}

//TEST(Service, prepare_insert__PREPARE_NEGATIVE_ID) {
//	Input_Buffer* ib = (Input_Buffer*) malloc(sizeof(Input_Buffer));
//	ib->buffer = "insert -1 guilherme";
//	Statement statement;
//	EXPECT_EQ(prepare_insert(ib, &statement), PREPARE_NEGATIVE_ID);
//}