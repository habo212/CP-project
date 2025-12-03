/**
 * @file questions.h
 * @brief Question data structure and loading functionality
 * 
 * This module handles:
 * - Question data structure definition
 * - Loading questions from JSON file
 * - Memory management for questions
 */

#ifndef QUESTIONS_H
#define QUESTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Maximum length for question text
 */
#define MAX_QUESTION_LEN 512

/**
 * @brief Maximum length for answer text
 */
#define MAX_ANSWER_LEN 256

/**
 * @brief Maximum number of options per question
 */
#define MAX_OPTIONS 4

/**
 * @brief Difficulty levels for questions
 */
typedef enum {
    DIFFICULTY_EASY = 0,
    DIFFICULTY_MEDIUM,
    DIFFICULTY_HARD,
    DIFFICULTY_COUNT
} Difficulty;

/**
 * @brief Question categories
 */
typedef enum {
    CATEGORY_GENERAL = 0,
    CATEGORY_SCIENCE,
    CATEGORY_HISTORY,
    CATEGORY_SPORTS,
    CATEGORY_ENTERTAINMENT,
    CATEGORY_COUNT
} Category;

/**
 * @brief Structure representing a trivia question
 */
typedef struct {
    char question[MAX_QUESTION_LEN];      /**< The question text */
    char options[MAX_OPTIONS][MAX_ANSWER_LEN]; /**< Array of answer options */
    int correct_answer;                   /**< Index of correct answer (0-3) */
    Difficulty difficulty;                /**< Difficulty level */
    Category category;                    /**< Question category */
} Question;

/**
 * @brief Structure to hold a collection of questions
 */
typedef struct {
    Question *questions;                  /**< Array of questions */
    size_t count;                         /**< Number of questions */
    size_t capacity;                      /**< Current capacity of array */
} QuestionBank;

/**
 * @brief Initialize an empty question bank
 * 
 * @param bank Pointer to QuestionBank to initialize
 * @return int 0 on success, -1 on error
 */
int question_bank_init(QuestionBank *bank);

/**
 * @brief Add a question to the question bank
 * 
 * @param bank Pointer to QuestionBank
 * @param question Question to add
 * @return int 0 on success, -1 on error
 */
int question_bank_add(QuestionBank *bank, const Question *question);

/**
 * @brief Load questions from a JSON file
 * 
 * @param bank Pointer to QuestionBank to populate
 * @param filename Path to JSON file
 * @return int Number of questions loaded, -1 on error
 */
int question_bank_load_from_json(QuestionBank *bank, const char *filename);

/**
 * @brief Free all memory associated with a question bank
 * 
 * @param bank Pointer to QuestionBank to free
 */
void question_bank_free(QuestionBank *bank);

/**
 * @brief Get a random question from the bank
 * 
 * @param bank Pointer to QuestionBank
 * @param difficulty Optional difficulty filter (use -1 for any)
 * @return Question* Pointer to question, or NULL if none found
 */
Question* question_bank_get_random(QuestionBank *bank, int difficulty);

/**
 * @brief Get difficulty name as string
 * 
 * @param difficulty Difficulty enum value
 * @return const char* String representation
 */
const char* difficulty_to_string(Difficulty difficulty);

/**
 * @brief Get category name as string
 * 
 * @param category Category enum value
 * @return const char* String representation
 */
const char* category_to_string(Category category);

#endif /* QUESTIONS_H */

