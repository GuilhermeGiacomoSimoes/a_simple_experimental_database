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

//TEST(Service, prepare_statement__RETURN_SELECT) {
//	Input_Buffer* ib = (Input_Buffer*) malloc(sizeof(Input_Buffer));
//	ib->buffer = "select 1";
//	Statement statemet;
//	prepare_statement(ib, &statemet);
//	EXPECT_EQ(statemet.type, STATEMENT_SELECT);
//}

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

//TEST(Service, prepare_select__PREPARE_SUCCESS) {
//	Input_Buffer* ib = (Input_Buffer*) malloc(sizeof(Input_Buffer));
//	ib->buffer = "select 1";
//	Statement statement;
//	EXPECT_EQ(prepare_select(ib, &statement), PREPARE_SUCCESS);
//}

//TEST(Service, prepare_select__PREPARE_NEGATIVE_ID) {
//	Input_Buffer* ib = (Input_Buffer*) malloc(sizeof(Input_Buffer));
//	ib->buffer = "select -11";
//	Statement statement;
//	EXPECT_EQ(prepare_select(ib, &statement), PREPARE_NEGATIVE_ID);
//}

TEST(Service, prepare_select__PREPARE_SYNTAX_ERROR) {
	Input_Buffer* ib = (Input_Buffer*) malloc(sizeof(Input_Buffer));
	ib->buffer = "select";
	Statement statement;
	EXPECT_EQ(prepare_select(ib, &statement), PREPARE_SYNTAX_ERROR);
}

TEST(Service, execute_statement__RETURN_EXECUTE_SUCCESS) {
	Statement* statement;
	statement->type = STATEMENT_INSERT;
	EXPECT_EQ(execute_statement(statement), EXECUTE_SUCCESS);
}

TEST(Service, execute_statement__RETURN_EXECUTE_DUPLICATE_KEY) {
	Statement* statement;
	statement->type = STATEMENT_SELECT;
	EXPECT_EQ(execute_statement(statement), EXECUTE_DUPLICATE_KEY);
}

TEST(Service, execute_insert__RETURN_EXECUTE_SUCCESS) {
	Statement* statement;
	EXPECT_EQ(execute_insert(statement), EXECUTE_SUCCESS);
}

TEST(Service, execute_select__RETURN_EXECUTE_DUPLICATE_KEY) {
	Statement* statement;
	EXPECT_EQ(execute_select(statement), EXECUTE_DUPLICATE_KEY);
}

//TEST(Service, execute__RETURN_CODE_ZERO) {
//	Input_Buffer* ib = (Input_Buffer*) malloc(sizeof(Input_Buffer));
//	ib->buffer = "select 1";
//	Result result = execute(ib);
//	EXPECT_EQ(result.code, 0);
//}

//TEST(Service, execute__RETURN_DUPLICATE_KEY_DESCRIPTION) {
//	Input_Buffer* ib = (Input_Buffer*) malloc(sizeof(Input_Buffer));
//	ib->buffer = "select 1";
//	Result result = execute(ib);
//	EXPECT_EQ(result.description, "Error: Duplicate key");
//}

//TEST(Service, execute__RETURN_CODE_ONE) {
//	Input_Buffer* ib = (Input_Buffer*) malloc(sizeof(Input_Buffer));
//	ib->buffer = "insert 1 guilherme";
//	Result result = execute(ib);
//	EXPECT_EQ(result.code, 0);
//}

//TEST(Service, execute__RETURN_EXECUTE_DESCRIPTION) {
//	Input_Buffer* ib = (Input_Buffer*) malloc(sizeof(Input_Buffer));
//	ib->buffer = "insert 1 guilherme";
//	Result result = execute(ib);
//	EXPECT_EQ(result.description, "Executed.");
//}