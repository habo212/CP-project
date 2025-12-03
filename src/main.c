/**
 * @file main.c
 * @brief Main entry point for Terminal Trivia Game
 * 
 * This program implements a terminal-based trivia game that:
 * - Loads questions from a JSON file
 * - Uses threads for timer functionality
 * - Tracks scores and statistics
 * - Provides a clean, interactive user experience
 * 
 * Design Notes:
 * - First design attempt: Single file with all functionality
 *   Changed to: Modular design with separate files for better organization
 * - First design attempt: Simple array for questions
 *   Changed to: Dynamic QuestionBank structure for scalability
 * - First design attempt: No timer
 *   Changed to: Thread-based timer for better user experience
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "questions.h"
#include "utils.h"

/**
 * @brief Default questions file path
 */
#define DEFAULT_QUESTIONS_FILE "data/questions.json"

/**
 * @brief Get number of players from user
 * 
 * @return int Number of players (1-4), or 0 on error
 */
static int get_num_players(void) {
    clear_screen();
    printf("\n═══════════════════════════════════════════════════════\n");
    printf("              SELECT NUMBER OF PLAYERS\n");
    printf("═══════════════════════════════════════════════════════\n\n");
    printf("  1. Single Player\n");
    printf("  2. Two Players\n");
    printf("  3. Three Players\n");
    printf("  4. Four Players\n");
    printf("\n  Enter your choice: ");
    
    char input[MAX_INPUT_LEN];
    int choice = 0;
    
    if (read_input(input, sizeof(input)) == UTILS_SUCCESS) {
        sanitize_input(input);
        if (is_valid_integer(input, &choice)) {
            if (choice >= 1 && choice <= 4) {
                return choice;
            }
        }
    }
    
    return 0;
}

/**
 * @brief Get player names for multiplayer mode
 * 
 * @param state Pointer to GameState
 * @return int 0 on success, -1 on error
 */
static int get_player_names(GameState *state) {
    if (state == NULL || state->config.num_players <= 1) {
        return 0;
    }
    
    clear_screen();
    printf("\n═══════════════════════════════════════════════════════\n");
    printf("              ENTER PLAYER NAMES\n");
    printf("═══════════════════════════════════════════════════════\n\n");
    
    for (int i = 0; i < state->config.num_players; i++) {
        printf("  Enter name for Player %d: ", i + 1);
        char input[MAX_INPUT_LEN];
        if (read_input(input, sizeof(input)) == UTILS_SUCCESS) {
            sanitize_input(input);
            if (strlen(input) > 0) {
                strncpy(state->players[i].name, input, sizeof(state->players[i].name) - 1);
                state->players[i].name[sizeof(state->players[i].name) - 1] = '\0';
            } else {
                snprintf(state->players[i].name, sizeof(state->players[i].name), 
                        "Player %d", i + 1);
            }
        } else {
            snprintf(state->players[i].name, sizeof(state->players[i].name), 
                    "Player %d", i + 1);
        }
    }
    
    return 0;
}

/**
 * @brief Display main menu and get user choice
 * 
 * @return int User's menu choice
 */
static int display_menu(void) {
    clear_screen();
    printf("\n═══════════════════════════════════════════════════════\n");
    printf("           TERMINAL TRIVIA GAME - MAIN MENU\n");
    printf("═══════════════════════════════════════════════════════\n\n");
    printf("  1. Start New Game (Easy)\n");
    printf("  2. Start New Game (Medium)\n");
    printf("  3. Start New Game (Hard)\n");
    printf("  4. Start New Game (Mixed Difficulty)\n");
    printf("  5. Exit\n");
    printf("\n  Enter your choice: ");
    
    char input[MAX_INPUT_LEN];
    int choice = 0;
    
    if (read_input(input, sizeof(input)) == UTILS_SUCCESS) {
        sanitize_input(input);
        if (is_valid_integer(input, &choice)) {
            return choice;
        }
    }
    
    return 0;
}


/**
 * @brief Main function
 * 
 * @param argc Number of command line arguments
 * @param argv Command line arguments
 * @return int Exit code (0 on success)
 */
int main(int argc, char *argv[]) {
    // Parse command line arguments
    char questions_file[256] = DEFAULT_QUESTIONS_FILE;
    
    if (argc > 1) {
        strncpy(questions_file, argv[1], sizeof(questions_file) - 1);
        questions_file[sizeof(questions_file) - 1] = '\0';
    }
    
    // Initialize question bank
    QuestionBank bank;
    if (question_bank_init(&bank) != 0) {
        print_error("Failed to initialize question bank");
        return EXIT_FAILURE;
    }
    
    // Load questions
    printf("Loading questions from: %s\n", questions_file);
    int loaded = question_bank_load_from_json(&bank, questions_file);
    
    if (loaded <= 0) {
        print_error("Failed to load questions or no questions found");
        print_error("Please ensure the questions file exists and is properly formatted");
        question_bank_free(&bank);
        return EXIT_FAILURE;
    }
    
    print_success("Loaded %d questions", loaded);
    
    // Main game loop
    bool running = true;
    while (running) {
        int choice = display_menu();
        
        if (choice == 5) {
            running = false;
            continue;
        }
        
        if (choice < 1 || choice > 4) {
            printf("\nInvalid choice. Please try again.\n");
            wait_for_enter();
            continue;
        }
        
        // Get number of players
        int num_players = get_num_players();
        if (num_players == 0) {
            printf("\nInvalid choice. Returning to main menu.\n");
            wait_for_enter();
            continue;
        }
        
        // Configure game based on choice
        GameConfig config;
        config.questions_per_game = 5;
        config.time_per_question = 30;
        config.use_timer = true;
        config.num_players = num_players;
        
        switch (choice) {
            case 1:
                config.difficulty = DIFFICULTY_EASY;
                break;
            case 2:
                config.difficulty = DIFFICULTY_MEDIUM;
                break;
            case 3:
                config.difficulty = DIFFICULTY_HARD;
                break;
            case 4:
                config.difficulty = -1; // Any difficulty
                break;
        }
        
        // Initialize and run game
        GameState game;
        if (game_init(&game, &bank, &config) != 0) {
            print_error("Failed to initialize game");
            wait_for_enter();
            continue;
        }
        
        // Get player names if multiplayer
        if (config.num_players > 1) {
            get_player_names(&game);
        }
        
        game_run(&game);
        game_cleanup(&game);
        
        wait_for_enter();
    }
    
    // Cleanup
    question_bank_free(&bank);
    
    printf("\nThank you for playing Terminal Trivia Game!\n");
    return EXIT_SUCCESS;
}

