/**
 * @file utils.h
 * @brief Utility functions for input/output and error handling
 * 
 * This module provides helper functions for:
 * - Input sanitization and validation
 * - Error handling and reporting
 * - String manipulation utilities
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * @brief Maximum length for user input strings
 */
#define MAX_INPUT_LEN 256

/**
 * @brief Maximum length for error messages
 */
#define MAX_ERROR_MSG_LEN 512

/**
 * @brief Error codes used throughout the application
 */
typedef enum {
    UTILS_SUCCESS = 0,
    UTILS_ERROR_NULL_POINTER,
    UTILS_ERROR_INVALID_INPUT,
    UTILS_ERROR_BUFFER_OVERFLOW,
    UTILS_ERROR_IO_FAILED
} UtilsError;

/**
 * @brief Read a line from stdin with bounds checking
 * 
 * @param buffer Buffer to store the input
 * @param size Size of the buffer
 * @return UtilsError UTILS_SUCCESS on success, error code otherwise
 */
UtilsError read_input(char *buffer, size_t size);

/**
 * @brief Sanitize input string by trimming whitespace
 * 
 * @param str String to sanitize (modified in place)
 * @return UtilsError UTILS_SUCCESS on success
 */
UtilsError sanitize_input(char *str);

/**
 * @brief Check if a string is a valid integer
 * 
 * @param str String to check
 * @param value Pointer to store the parsed integer (optional, can be NULL)
 * @return int 1 if valid integer, 0 otherwise
 */
int is_valid_integer(const char *str, int *value);

/**
 * @brief Print an error message to stderr
 * 
 * @param format printf-style format string
 * @param ... Variable arguments
 */
void print_error(const char *format, ...);

/**
 * @brief Print a success message to stdout
 * 
 * @param format printf-style format string
 * @param ... Variable arguments
 */
void print_success(const char *format, ...);

/**
 * @brief Clear the terminal screen (cross-platform)
 */
void clear_screen(void);

/**
 * @brief Wait for user to press Enter
 */
void wait_for_enter(void);

#endif /* UTILS_H */

