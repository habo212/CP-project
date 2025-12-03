/**
 * @file test_questions.c
 * @brief Unit tests for question loading and management
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/questions.h"

/**
 * @brief Test question bank initialization
 * 
 * @return int 0 on success, non-zero on failure
 */
static int test_question_bank_init(void) {
    QuestionBank bank;
    
    if (question_bank_init(&bank) != 0) {
        printf("  ❌ test_question_bank_init: Failed to initialize\n");
        return -1;
    }
    
    if (bank.count != 0 || bank.capacity == 0 || bank.questions == NULL) {
        printf("  ❌ test_question_bank_init: Invalid initial state\n");
        question_bank_free(&bank);
        return -1;
    }
    
    question_bank_free(&bank);
    printf("  ✅ test_question_bank_init: PASSED\n");
    return 0;
}

/**
 * @brief Test adding questions to bank
 * 
 * @return int 0 on success, non-zero on failure
 */
static int test_question_bank_add(void) {
    QuestionBank bank;
    if (question_bank_init(&bank) != 0) {
        return -1;
    }
    
    Question q;
    strncpy(q.question, "Test question?", sizeof(q.question) - 1);
    strncpy(q.options[0], "Option 1", sizeof(q.options[0]) - 1);
    strncpy(q.options[1], "Option 2", sizeof(q.options[1]) - 1);
    strncpy(q.options[2], "Option 3", sizeof(q.options[2]) - 1);
    strncpy(q.options[3], "Option 4", sizeof(q.options[3]) - 1);
    q.correct_answer = 0;
    q.difficulty = DIFFICULTY_EASY;
    q.category = CATEGORY_GENERAL;
    
    if (question_bank_add(&bank, &q) != 0) {
        printf("  ❌ test_question_bank_add: Failed to add question\n");
        question_bank_free(&bank);
        return -1;
    }
    
    if (bank.count != 1) {
        printf("  ❌ test_question_bank_add: Count mismatch\n");
        question_bank_free(&bank);
        return -1;
    }
    
    if (strcmp(bank.questions[0].question, "Test question?") != 0) {
        printf("  ❌ test_question_bank_add: Question text mismatch\n");
        question_bank_free(&bank);
        return -1;
    }
    
    question_bank_free(&bank);
    printf("  ✅ test_question_bank_add: PASSED\n");
    return 0;
}

/**
 * @brief Test difficulty and category string conversion
 * 
 * @return int 0 on success, non-zero on failure
 */
static int test_difficulty_category_strings(void) {
    const char *easy = difficulty_to_string(DIFFICULTY_EASY);
    const char *medium = difficulty_to_string(DIFFICULTY_MEDIUM);
    const char *hard = difficulty_to_string(DIFFICULTY_HARD);
    
    if (strcmp(easy, "Easy") != 0 || 
        strcmp(medium, "Medium") != 0 || 
        strcmp(hard, "Hard") != 0) {
        printf("  ❌ test_difficulty_category_strings: Difficulty string mismatch\n");
        return -1;
    }
    
    const char *general = category_to_string(CATEGORY_GENERAL);
    const char *science = category_to_string(CATEGORY_SCIENCE);
    
    if (strcmp(general, "General") != 0 || 
        strcmp(science, "Science") != 0) {
        printf("  ❌ test_difficulty_category_strings: Category string mismatch\n");
        return -1;
    }
    
    printf("  ✅ test_difficulty_category_strings: PASSED\n");
    return 0;
}

/**
 * @brief Test question bank free
 * 
 * @return int 0 on success, non-zero on failure
 */
static int test_question_bank_free(void) {
    QuestionBank bank;
    if (question_bank_init(&bank) != 0) {
        return -1;
    }
    
    Question q;
    strncpy(q.question, "Test", sizeof(q.question) - 1);
    strncpy(q.options[0], "A", sizeof(q.options[0]) - 1);
    q.correct_answer = 0;
    q.difficulty = DIFFICULTY_EASY;
    q.category = CATEGORY_GENERAL;
    
    question_bank_add(&bank, &q);
    question_bank_free(&bank);
    
    // Check that memory was freed (pointer should be NULL)
    if (bank.questions != NULL) {
        printf("  ❌ test_question_bank_free: Memory not properly freed\n");
        return -1;
    }
    
    printf("  ✅ test_question_bank_free: PASSED\n");
    return 0;
}

/**
 * @brief Run all questions tests
 * 
 * @return int 0 if all tests pass, non-zero otherwise
 */
int test_questions(void) {
    int failures = 0;
    
    failures += test_question_bank_init();
    failures += test_question_bank_add();
    failures += test_difficulty_category_strings();
    failures += test_question_bank_free();
    
    return failures;
}

