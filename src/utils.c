/**
 * @file utils.c
 * @brief Implementation of utility functions
 */

#include "utils.h"
#include <stdarg.h>
#include <limits.h>

UtilsError read_input(char *buffer, size_t size) {
    if (buffer == NULL) {
        return UTILS_ERROR_NULL_POINTER;
    }
    
    if (size == 0) {
        return UTILS_ERROR_INVALID_INPUT;
    }
    
    if (fgets(buffer, (int)size, stdin) == NULL) {
        return UTILS_ERROR_IO_FAILED;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    return UTILS_SUCCESS;
}

UtilsError sanitize_input(char *str) {
    if (str == NULL) {
        return UTILS_ERROR_NULL_POINTER;
    }
    
    char *start = str;
    while (isspace((unsigned char)*start)) {
        start++;
    }
    
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
    
    size_t len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        str[len - 1] = '\0';
        len--;
    }
    
    return UTILS_SUCCESS;
}

int is_valid_integer(const char *str, int *value) {
    if (str == NULL || strlen(str) == 0) {
        return 0;
    }
    
    char *endptr;
    long num = strtol(str, &endptr, 10);
    
    if (*endptr != '\0' || num < INT_MIN || num > INT_MAX) {
        return 0;
    }
    
    if (value != NULL) {
        *value = (int)num;
    }
    
    return 1;
}

void print_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "[ERROR] ");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);
}

void print_success(const char *format, ...) {
    va_list args;
    va_start(args, format);
    printf("[SUCCESS] ");
    vprintf(format, args);
    printf("\n");
    va_end(args);
}

void clear_screen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void wait_for_enter(void) {
    printf("\nPress Enter to continue...");
    while (getchar() != '\n');
}

