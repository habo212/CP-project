/**
 * @file game.c
 * @brief Implementation of game logic
 */

#include "game.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdbool.h>

int game_init(GameState *state, QuestionBank *bank, const GameConfig *config) {
    if (state == NULL || bank == NULL || config == NULL) {
        return -1;
    }
    
    state->question_bank = bank;
    state->config = *config;
    state->game_active = false;
    state->current_player = 0;
    state->players = NULL;
    
    // Initialize stats (for single-player mode)
    state->stats.total_questions = 0;
    state->stats.correct_answers = 0;
    state->stats.wrong_answers = 0;
    state->stats.timeouts = 0;
    state->stats.score = 0;
    
    // Initialize players if multiplayer
    if (config->num_players > 1) {
        if (game_init_players(state) != 0) {
            return -1;
        }
    }
    
    // Initialize timer if enabled
    if (config->use_timer) {
        if (timer_init(&state->timer, config->time_per_question) != 0) {
            print_error("Failed to initialize timer");
            return -1;
        }
    }
    
    return 0;
}

int game_init_players(GameState *state) {
    if (state == NULL || state->config.num_players < 2) {
        return -1;
    }
    
    state->players = (Player*)malloc(state->config.num_players * sizeof(Player));
    if (state->players == NULL) {
        print_error("Failed to allocate memory for players");
        return -1;
    }
    
    // Initialize all players
    for (int i = 0; i < state->config.num_players; i++) {
        state->players[i].score = 0;
        state->players[i].correct_answers = 0;
        state->players[i].wrong_answers = 0;
        state->players[i].timeouts = 0;
        state->players[i].name[0] = '\0';
    }
    
    return 0;
}

Player* game_get_current_player(GameState *state) {
    if (state == NULL || state->config.num_players <= 1) {
        return NULL;
    }
    
    if (state->current_player >= 0 && state->current_player < state->config.num_players) {
        return &state->players[state->current_player];
    }
    
    return NULL;
}

void game_next_player(GameState *state) {
    if (state == NULL || state->config.num_players <= 1) {
        return;
    }
    
    state->current_player = (state->current_player + 1) % state->config.num_players;
}

int game_calculate_score(bool correct, int time_remaining, Difficulty difficulty) {
    if (!correct) {
        return 0;
    }
    
    // Base points by difficulty
    int base_points = 0;
    switch (difficulty) {
        case DIFFICULTY_EASY:
            base_points = 10;
            break;
        case DIFFICULTY_MEDIUM:
            base_points = 20;
            break;
        case DIFFICULTY_HARD:
            base_points = 30;
            break;
        default:
            base_points = 10;
    }
    
    // Bonus for answering quickly
    int time_bonus = time_remaining / 2;
    
    return base_points + time_bonus;
}

int game_ask_question(GameState *state, const Question *question) {
    if (state == NULL || question == NULL) {
        return -1;
    }
    
    clear_screen();
    
    // Display question
    printf("\n═══════════════════════════════════════════════════════\n");
    if (state->config.num_players > 1) {
        Player *player = game_get_current_player(state);
        if (player != NULL) {
            printf("  Player: %s\n", player->name[0] != '\0' ? player->name : 
                   (player == &state->players[0] ? "Player 1" : 
                    player == &state->players[1] ? "Player 2" : "Player"));
        }
    }
    printf("  %s\n", question->question);
    printf("  Difficulty: %s\n", difficulty_to_string(question->difficulty));
    printf("═══════════════════════════════════════════════════════\n\n");
    
    // Display options
    for (int i = 0; i < MAX_OPTIONS && strlen(question->options[i]) > 0; i++) {
        printf("  %d. %s\n", i + 1, question->options[i]);
    }
    printf("\n");
    
    // Start timer if enabled
    if (state->config.use_timer) {
        timer_reset(&state->timer, state->config.time_per_question);
        timer_start(&state->timer);
        printf("  Time remaining: %d seconds\n", state->config.time_per_question);
    }
    
    printf("  Enter your answer (1-%d) or 'q' to quit: ", 
           MAX_OPTIONS);
    fflush(stdout);
    
    char input[MAX_INPUT_LEN];
    int answer = 0;
    
    // Poll for input while timer runs
    while (state->config.use_timer && timer_get_remaining(&state->timer) > 0) {
        if (timer_is_expired(&state->timer)) {
            timer_stop(&state->timer);
            printf("\n\n⏰ Time's up!\n");
            return 0; // Timeout
        }
        
        // Check if input is available (non-blocking check)
        fd_set readfds;
        struct timeval timeout;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        timeout.tv_sec = 0;
        timeout.tv_usec = 100000; // 0.1 seconds
        
        if (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout) > 0) {
            if (read_input(input, sizeof(input)) == UTILS_SUCCESS) {
                sanitize_input(input);
                
                if (input[0] == 'q' || input[0] == 'Q') {
                    timer_stop(&state->timer);
                    return -1; // Quit
                }
                
                if (is_valid_integer(input, &answer)) {
                    if (answer >= 1 && answer <= MAX_OPTIONS) {
                        timer_stop(&state->timer);
                        return answer;
                    }
                }
                
                printf("  Invalid input. Enter 1-%d: ", MAX_OPTIONS);
                fflush(stdout);
            }
        }
        
        // Update timer display
        int remaining = timer_get_remaining(&state->timer);
        printf("\r  Time remaining: %d seconds   ", remaining);
        fflush(stdout);
    }
    
    // If timer is not used, just read input normally
    if (!state->config.use_timer) {
        if (read_input(input, sizeof(input)) == UTILS_SUCCESS) {
            sanitize_input(input);
            
            if (input[0] == 'q' || input[0] == 'Q') {
                return -1; // Quit
            }
            
            if (is_valid_integer(input, &answer)) {
                if (answer >= 1 && answer <= MAX_OPTIONS) {
                    return answer;
                }
            }
        }
    }
    
    return 0; // Timeout or invalid
}

int game_run(GameState *state) {
    if (state == NULL || state->question_bank == NULL) {
        return -1;
    }
    
    state->game_active = true;
    
    clear_screen();
    printf("═══════════════════════════════════════════════════════\n");
    printf("           WELCOME TO TERMINAL TRIVIA GAME!\n");
    printf("═══════════════════════════════════════════════════════\n\n");
    
    printf("Game Configuration:\n");
    printf("  Questions: %d\n", state->config.questions_per_game);
    printf("  Time per question: %d seconds\n", state->config.time_per_question);
    printf("  Difficulty: %s\n", 
           state->config.difficulty >= 0 ? 
           difficulty_to_string(state->config.difficulty) : "Any");
    printf("  Players: %d\n", state->config.num_players);
    printf("\n");
    
    wait_for_enter();
    
    // Calculate total rounds (questions per player in multiplayer)
    int total_rounds = state->config.questions_per_game;
    if (state->config.num_players > 1) {
        total_rounds = state->config.questions_per_game * state->config.num_players;
    }
    
    // Game loop
    for (int i = 0; i < total_rounds; i++) {
        // Get random question
        int difficulty_filter = state->config.difficulty >= 0 ? 
                               state->config.difficulty : -1;
        Question *question = question_bank_get_random(state->question_bank, 
                                                       difficulty_filter);
        
        if (question == NULL) {
            print_error("No questions available");
            break;
        }
        
        // Ask question
        int user_answer = game_ask_question(state, question);
        
        if (user_answer == -1) {
            printf("\nGame quit by user.\n");
            break;
        }
        
        // Check answer
        bool correct = (user_answer == question->correct_answer + 1);
        int time_remaining = state->config.use_timer ? 
                            timer_get_remaining(&state->timer) : 
                            state->config.time_per_question;
        
        // Update stats for current player
        if (state->config.num_players > 1) {
            Player *player = game_get_current_player(state);
            if (player != NULL) {
                if (user_answer == 0) {
                    // Timeout
                    player->timeouts++;
                    printf("\n❌ Time's up! The correct answer was: %d. %s\n",
                           question->correct_answer + 1,
                           question->options[question->correct_answer]);
                } else if (correct) {
                    player->correct_answers++;
                    int points = game_calculate_score(true, time_remaining, 
                                                    question->difficulty);
                    player->score += points;
                    printf("\n✅ Correct! +%d points\n", points);
                } else {
                    player->wrong_answers++;
                    printf("\n❌ Wrong! The correct answer was: %d. %s\n",
                           question->correct_answer + 1,
                           question->options[question->correct_answer]);
                }
            }
        } else {
            // Single-player mode
            state->stats.total_questions++;
            if (user_answer == 0) {
                // Timeout
                state->stats.timeouts++;
                printf("\n❌ Time's up! The correct answer was: %d. %s\n",
                       question->correct_answer + 1,
                       question->options[question->correct_answer]);
            } else if (correct) {
                state->stats.correct_answers++;
                int points = game_calculate_score(true, time_remaining, 
                                                question->difficulty);
                state->stats.score += points;
                printf("\n✅ Correct! +%d points\n", points);
            } else {
                state->stats.wrong_answers++;
                printf("\n❌ Wrong! The correct answer was: %d. %s\n",
                       question->correct_answer + 1,
                       question->options[question->correct_answer]);
            }
        }
        
        // Show current scores in multiplayer
        if (state->config.num_players > 1) {
            printf("\nCurrent Scores:\n");
            for (int p = 0; p < state->config.num_players; p++) {
                const char *name = state->players[p].name[0] != '\0' ? 
                                  state->players[p].name : 
                                  (p == 0 ? "Player 1" : p == 1 ? "Player 2" : "Player");
                printf("  %s: %d points\n", name, state->players[p].score);
            }
        }
        
        wait_for_enter();
        
        // Move to next player in multiplayer mode
        if (state->config.num_players > 1) {
            game_next_player(state);
        }
    }
    
    state->game_active = false;
    game_display_stats(state);
    
    return state->stats.score;
}

void game_display_stats(const GameState *state) {
    if (state == NULL) {
        return;
    }
    
    clear_screen();
    printf("\n═══════════════════════════════════════════════════════\n");
    printf("                    GAME STATISTICS\n");
    printf("═══════════════════════════════════════════════════════\n\n");
    
    if (state->config.num_players > 1) {
        // Multiplayer stats
        printf("Final Scores:\n\n");
        for (int i = 0; i < state->config.num_players; i++) {
            const char *name = state->players[i].name[0] != '\0' ? 
                              state->players[i].name : 
                              (i == 0 ? "Player 1" : i == 1 ? "Player 2" : "Player");
            int total_answered = state->players[i].correct_answers + 
                                state->players[i].wrong_answers;
            double accuracy = 0.0;
            if (total_answered > 0) {
                accuracy = (double)state->players[i].correct_answers / 
                          total_answered * 100.0;
            }
            
            printf("  %s:\n", name);
            printf("    Score: %d points\n", state->players[i].score);
            printf("    Correct: %d\n", state->players[i].correct_answers);
            printf("    Wrong: %d\n", state->players[i].wrong_answers);
            printf("    Timeouts: %d\n", state->players[i].timeouts);
            if (total_answered > 0) {
                printf("    Accuracy: %.1f%%\n", accuracy);
            }
            printf("\n");
        }
        
        // Find winner
        int max_score = state->players[0].score;
        int winner_idx = 0;
        bool tie = false;
        for (int i = 1; i < state->config.num_players; i++) {
            if (state->players[i].score > max_score) {
                max_score = state->players[i].score;
                winner_idx = i;
                tie = false;
            } else if (state->players[i].score == max_score) {
                tie = true;
            }
        }
        
        if (!tie) {
            const char *winner = state->players[winner_idx].name[0] != '\0' ? 
                               state->players[winner_idx].name : 
                               (winner_idx == 0 ? "Player 1" : 
                                winner_idx == 1 ? "Player 2" : "Player");
            printf("🏆 Winner: %s with %d points!\n", winner, max_score);
        } else {
            printf("🤝 It's a tie!\n");
        }
    } else {
        // Single-player stats
        printf("  Total Questions: %d\n", state->stats.total_questions);
        printf("  Correct Answers: %d\n", state->stats.correct_answers);
        printf("  Wrong Answers: %d\n", state->stats.wrong_answers);
        printf("  Timeouts: %d\n", state->stats.timeouts);
        printf("  Final Score: %d points\n", state->stats.score);
        
        if (state->stats.total_questions > 0) {
            double accuracy = (double)state->stats.correct_answers / 
                              state->stats.total_questions * 100.0;
            printf("  Accuracy: %.1f%%\n", accuracy);
        }
    }
    
    printf("\n═══════════════════════════════════════════════════════\n\n");
}

void game_cleanup(GameState *state) {
    if (state == NULL) {
        return;
    }
    
    if (state->config.use_timer) {
        timer_cleanup(&state->timer);
    }
    
    if (state->players != NULL) {
        free(state->players);
        state->players = NULL;
    }
    
    state->game_active = false;
}

