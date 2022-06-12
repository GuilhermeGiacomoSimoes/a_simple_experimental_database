#include<iostream>
#include"test-main.hh"

void basic_tests() {
	ALPH_ASSERT_THROW(1 != 2);
	ALPH_ASSERT_EQUAL(2, 2);
	ALPH_ASSERT_EQUAL(2, 2);
}


int main() {
	basic_tests();
}
