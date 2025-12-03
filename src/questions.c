/**
 * @file questions.c
 * @brief Implementation of question loading and management
 */

#include "questions.h"
#include "utils.h"
#include <time.h>
#include <ctype.h>

// Simple JSON parser for our specific format
// Note: This is a simplified parser for educational purposes
// In production, you'd use a proper JSON library like cJSON

static int parse_json_question(const char *json_line, Question *q) {
    if (json_line == NULL || q == NULL) {
        return -1;
    }
    
    // Simple JSON parsing - looks for key-value pairs
    // Format: {"question":"...", "options":["...","..."], "correct":0, "difficulty":"easy"}
    
    char line[1024];
    strncpy(line, json_line, sizeof(line) - 1);
    line[sizeof(line) - 1] = '\0';
    
    // Extract question
    char *q_start = strstr(line, "\"question\"");
    if (q_start == NULL) return -1;
    q_start = strchr(q_start, ':');
    if (q_start == NULL) return -1;
    q_start = strchr(q_start, '"');
    if (q_start == NULL) return -1;
    q_start++;
    char *q_end = strchr(q_start, '"');
    if (q_end == NULL) return -1;
    *q_end = '\0';
    strncpy(q->question, q_start, MAX_QUESTION_LEN - 1);
    q->question[MAX_QUESTION_LEN - 1] = '\0';
    *q_end = '"';
    
    // Extract options
    char *opt_start = strstr(line, "\"options\"");
    if (opt_start == NULL) return -1;
    opt_start = strchr(opt_start, '[');
    if (opt_start == NULL) return -1;
    opt_start++;
    
    int opt_idx = 0;
    while (opt_idx < MAX_OPTIONS) {
        char *opt_begin = strchr(opt_start, '"');
        if (opt_begin == NULL) break;
        opt_begin++;
        char *opt_end = strchr(opt_begin, '"');
        if (opt_end == NULL) break;
        *opt_end = '\0';
        strncpy(q->options[opt_idx], opt_begin, MAX_ANSWER_LEN - 1);
        q->options[opt_idx][MAX_ANSWER_LEN - 1] = '\0';
        *opt_end = '"';
        opt_idx++;
        opt_start = opt_end + 1;
        if (*opt_start == ']') break;
    }
    
    // Extract correct answer
    char *corr_start = strstr(line, "\"correct\"");
    if (corr_start == NULL) return -1;
    corr_start = strchr(corr_start, ':');
    if (corr_start == NULL) return -1;
    corr_start++;
    while (isspace(*corr_start)) corr_start++;
    q->correct_answer = atoi(corr_start);
    if (q->correct_answer < 0 || q->correct_answer >= opt_idx) {
        return -1;
    }
    
    // Extract difficulty
    char *diff_start = strstr(line, "\"difficulty\"");
    if (diff_start == NULL) {
        q->difficulty = DIFFICULTY_EASY;
    } else {
        diff_start = strchr(diff_start, ':');
        if (diff_start == NULL) {
            q->difficulty = DIFFICULTY_EASY;
        } else {
            diff_start = strchr(diff_start, '"');
            if (diff_start == NULL) {
                q->difficulty = DIFFICULTY_EASY;
            } else {
                diff_start++;
                if (strncmp(diff_start, "easy", 4) == 0) {
                    q->difficulty = DIFFICULTY_EASY;
                } else if (strncmp(diff_start, "medium", 6) == 0) {
                    q->difficulty = DIFFICULTY_MEDIUM;
                } else if (strncmp(diff_start, "hard", 4) == 0) {
                    q->difficulty = DIFFICULTY_HARD;
                } else {
                    q->difficulty = DIFFICULTY_EASY;
                }
            }
        }
    }
    
    // Default category
    q->category = CATEGORY_GENERAL;
    
    return 0;
}

int question_bank_init(QuestionBank *bank) {
    if (bank == NULL) {
        return -1;
    }
    
    bank->capacity = 10;
    bank->count = 0;
    bank->questions = (Question*)malloc(bank->capacity * sizeof(Question));
    
    if (bank->questions == NULL) {
        print_error("Failed to allocate memory for question bank");
        return -1;
    }
    
    return 0;
}

int question_bank_add(QuestionBank *bank, const Question *question) {
    if (bank == NULL || question == NULL) {
        return -1;
    }
    
    // Resize if needed
    if (bank->count >= bank->capacity) {
        size_t new_capacity = bank->capacity * 2;
        Question *new_questions = (Question*)realloc(bank->questions, 
                                                      new_capacity * sizeof(Question));
        if (new_questions == NULL) {
            print_error("Failed to reallocate memory for question bank");
            return -1;
        }
        bank->questions = new_questions;
        bank->capacity = new_capacity;
    }
    
    bank->questions[bank->count] = *question;
    bank->count++;
    
    return 0;
}

int question_bank_load_from_json(QuestionBank *bank, const char *filename) {
    if (bank == NULL || filename == NULL) {
        return -1;
    }
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        print_error("Failed to open questions file: %s", filename);
        return -1;
    }
    
    char line[2048];
    int loaded = 0;
    int in_array = 0;
    
    // Read file line by line
    while (fgets(line, sizeof(line), file) != NULL) {
        // Skip empty lines and comments
        if (line[0] == '\n' || line[0] == '#') {
            continue;
        }
        
        // Check if we're entering the questions array
        if (strstr(line, "[") != NULL) {
            in_array = 1;
            continue;
        }
        
        if (!in_array) {
            continue;
        }
        
        // Check if we're leaving the array
        if (strstr(line, "]") != NULL) {
            break;
        }
        
        // Try to parse a question
        Question q;
        if (parse_json_question(line, &q) == 0) {
            if (question_bank_add(bank, &q) == 0) {
                loaded++;
            }
        }
    }
    
    fclose(file);
    return loaded;
}

void question_bank_free(QuestionBank *bank) {
    if (bank != NULL && bank->questions != NULL) {
        free(bank->questions);
        bank->questions = NULL;
        bank->count = 0;
        bank->capacity = 0;
    }
}

Question* question_bank_get_random(QuestionBank *bank, int difficulty) {
    if (bank == NULL || bank->count == 0) {
        return NULL;
    }
    
    // Filter by difficulty if specified
    int valid_count = 0;
    int *valid_indices = NULL;
    
    if (difficulty >= 0 && difficulty < DIFFICULTY_COUNT) {
        valid_indices = (int*)malloc(bank->count * sizeof(int));
        if (valid_indices == NULL) {
            return NULL;
        }
        
        for (size_t i = 0; i < bank->count; i++) {
            if (bank->questions[i].difficulty == (Difficulty)difficulty) {
                valid_indices[valid_count] = (int)i;
                valid_count++;
            }
        }
        
        if (valid_count == 0) {
            free(valid_indices);
            return NULL;
        }
    } else {
        valid_count = (int)bank->count;
    }
    
    // Get random index
    srand((unsigned int)time(NULL));
    int random_idx = rand() % valid_count;
    
    int actual_idx;
    if (valid_indices != NULL) {
        actual_idx = valid_indices[random_idx];
        free(valid_indices);
    } else {
        actual_idx = random_idx;
    }
    
    return &bank->questions[actual_idx];
}

const char* difficulty_to_string(Difficulty difficulty) {
    switch (difficulty) {
        case DIFFICULTY_EASY: return "Easy";
        case DIFFICULTY_MEDIUM: return "Medium";
        case DIFFICULTY_HARD: return "Hard";
        default: return "Unknown";
    }
}

const char* category_to_string(Category category) {
    switch (category) {
        case CATEGORY_GENERAL: return "General";
        case CATEGORY_SCIENCE: return "Science";
        case CATEGORY_HISTORY: return "History";
        case CATEGORY_SPORTS: return "Sports";
        case CATEGORY_ENTERTAINMENT: return "Entertainment";
        default: return "Unknown";
    }
}

