#include <gtest/gtest.h>

extern "C" {
#include "frontend.c"
}

TEST(FrontEnd, new_input_buffer__RETURN_DEFAULT_INPUT_BUFFER) {
	Input_Buffer* result = new_input_buffer();
	EXPECT_EQ(result->buffer_length, 0);
	EXPECT_EQ(result->input_length, 0);
}
