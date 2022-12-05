#include <gtest/gtest.h>

extern "C" {
#include "main.c"
}

TEST(MainTest, test_1) {
	EXPECT_EQ(sum(1, 1), 2);
}

TEST(MainTest, test_2) {
	EXPECT_EQ(2, 2);
}
