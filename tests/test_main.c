/**
 * @file test_main.c
 * @brief Test runner for unit tests
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Test function declarations
extern int test_utils(void);
extern int test_questions(void);

/**
 * @brief Run all tests
 * 
 * @return int 0 if all tests pass, non-zero otherwise
 */
int main(int argc, char *argv[]) {
    printf("═══════════════════════════════════════════════════════\n");
    printf("              TERMINAL TRIVIA GAME - TESTS\n");
    printf("═══════════════════════════════════════════════════════\n\n");
    
    int total_tests = 0;
    int passed_tests = 0;
    int failed_tests = 0;
    
    // Determine which tests to run
    bool run_utils = false;
    bool run_questions = false;
    bool run_all = false;
    
    if (argc > 1) {
        if (strcmp(argv[1], "all") == 0) {
            run_all = true;
        } else if (strcmp(argv[1], "utils") == 0) {
            run_utils = true;
        } else if (strcmp(argv[1], "questions") == 0) {
            run_questions = true;
        }
    } else {
        run_all = true;
    }
    
    if (run_all || run_utils) {
        printf("Running Utils Tests...\n");
        printf("─────────────────────────────────────────────────────\n");
        int result = test_utils();
        total_tests++;
        if (result == 0) {
            printf("✅ Utils tests PASSED\n\n");
            passed_tests++;
        } else {
            printf("❌ Utils tests FAILED\n\n");
            failed_tests++;
        }
    }
    
    if (run_all || run_questions) {
        printf("Running Questions Tests...\n");
        printf("─────────────────────────────────────────────────────\n");
        int result = test_questions();
        total_tests++;
        if (result == 0) {
            printf("✅ Questions tests PASSED\n\n");
            passed_tests++;
        } else {
            printf("❌ Questions tests FAILED\n\n");
            failed_tests++;
        }
    }
    
    // Summary
    printf("═══════════════════════════════════════════════════════\n");
    printf("Test Summary:\n");
    printf("  Total: %d\n", total_tests);
    printf("  Passed: %d\n", passed_tests);
    printf("  Failed: %d\n", failed_tests);
    printf("═══════════════════════════════════════════════════════\n");
    
    return failed_tests > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}

