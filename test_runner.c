#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <CUnit/Basic.h>

#include "ui.h"
#include "string_utils.h"
#include "colors.h"
#include "filesystem.h"

Dimension screen_size;

char* temp_string = NULL;

int suite_setup() {
	temp_string = strdup("reminder");
	return 0;
}

int suite_teardown() {
	if(temp_string) {
		free(temp_string);
	}
	return 0;
}

void test_string_is_empty() {
	CU_ASSERT_FALSE(string_is_empty(temp_string));
	CU_ASSERT_TRUE(string_is_empty(""));
	CU_ASSERT_TRUE(string_is_empty(NULL));
}

void test_uppercase_string() {
	const char string[] = "Hello world";
	temp_string = uppercase_string(string);
	CU_ASSERT_EQUAL(0, strcmp(temp_string, "HELLO WORLD"));
}

void test_color_pair_no_color_is_zero() {
	CU_ASSERT_EQUAL(0, COLOR_PAIR_NO_COLOR);
}

void test_file_exists() {
	CU_ASSERT_TRUE(file_exists("test_runner.c"));
}

void test_file_does_not_exist() {
	CU_ASSERT_FALSE(file_exists("test_runner.java"));
}

int main(int argc, char** argv) {
	if(CUE_SUCCESS != CU_initialize_registry()) {
		return CU_get_error();
	}

	CU_pSuite main_suite = CU_add_suite("General", suite_setup, suite_teardown);
	CU_pSuite strings_suite = CU_add_suite("Strings", suite_setup, suite_teardown);
	CU_pSuite filesystem_suite = CU_add_suite("Filesystem", suite_setup, suite_teardown);

	if(NULL == main_suite || NULL == strings_suite || NULL == filesystem_suite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if(
		(NULL == CU_add_test(main_suite, "test of string_is_empty()", test_string_is_empty)) ||
		(NULL == CU_add_test(strings_suite, "test of uppercase_string()", test_uppercase_string)) ||
		(NULL == CU_add_test(strings_suite, "test of test_color_pair_no_color_is_zero()", test_color_pair_no_color_is_zero)) ||
		(NULL == CU_add_test(filesystem_suite, "test of test_file_exists()", test_file_exists)) ||
		(NULL == CU_add_test(filesystem_suite, "test of test_file_does_not_exist()", test_file_does_not_exist))
	) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	unsigned int num_failures = CU_get_number_of_failures();
	CU_cleanup_registry();

	CU_get_error();
	return num_failures;
}
