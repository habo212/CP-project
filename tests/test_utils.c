/**
 * @file test_utils.c
 * @brief Unit tests for utility functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../src/utils.h"

/**
 * @brief Test input sanitization
 * 
 * @return int 0 on success, non-zero on failure
 */
static int test_sanitize_input(void) {
    char test1[] = "  hello world  ";
    sanitize_input(test1);
    if (strcmp(test1, "hello world") != 0) {
        printf("  ❌ test_sanitize_input: Failed to trim whitespace\n");
        return -1;
    }
    
    char test2[] = "no_whitespace";
    sanitize_input(test2);
    if (strcmp(test2, "no_whitespace") != 0) {
        printf("  ❌ test_sanitize_input: Modified string without whitespace\n");
        return -1;
    }
    
    printf("  ✅ test_sanitize_input: PASSED\n");
    return 0;
}

/**
 * @brief Test integer validation
 * 
 * @return int 0 on success, non-zero on failure
 */
static int test_is_valid_integer(void) {
    int value;
    
    if (!is_valid_integer("123", &value) || value != 123) {
        printf("  ❌ test_is_valid_integer: Failed to validate positive integer\n");
        return -1;
    }
    
    if (!is_valid_integer("-456", &value) || value != -456) {
        printf("  ❌ test_is_valid_integer: Failed to validate negative integer\n");
        return -1;
    }
    
    if (is_valid_integer("abc", NULL)) {
        printf("  ❌ test_is_valid_integer: Accepted invalid integer\n");
        return -1;
    }
    
    if (is_valid_integer("12.34", NULL)) {
        printf("  ❌ test_is_valid_integer: Accepted float as integer\n");
        return -1;
    }
    
    printf("  ✅ test_is_valid_integer: PASSED\n");
    return 0;
}

/**
 * @brief Test null pointer handling
 * 
 * @return int 0 on success, non-zero on failure
 */
static int test_null_pointer_handling(void) {
    UtilsError result;
    
    result = sanitize_input(NULL);
    if (result != UTILS_ERROR_NULL_POINTER) {
        printf("  ❌ test_null_pointer_handling: Failed to detect NULL in sanitize_input\n");
        return -1;
    }
    
    if (is_valid_integer(NULL, NULL)) {
        printf("  ❌ test_null_pointer_handling: Failed to detect NULL in is_valid_integer\n");
        return -1;
    }
    
    printf("  ✅ test_null_pointer_handling: PASSED\n");
    return 0;
}

/**
 * @brief Run all utils tests
 * 
 * @return int 0 if all tests pass, non-zero otherwise
 */
int test_utils(void) {
    int failures = 0;
    
    failures += test_sanitize_input();
    failures += test_is_valid_integer();
    failures += test_null_pointer_handling();
    
    return failures;
}

