/**
 * @file game.h
 * @brief Game logic and state management
 * 
 * This module handles:
 * - Game state management
 * - Question presentation and answer validation
 * - Score tracking
 * - Game flow control
 */

#ifndef GAME_H
#define GAME_H

#include "questions.h"
#include "timer.h"

/**
 * @brief Game configuration structure
 */
typedef struct {
    int questions_per_game;        /**< Number of questions per game */
    int time_per_question;         /**< Time limit per question in seconds */
    Difficulty difficulty;          /**< Difficulty filter (-1 for any) */
    bool use_timer;                /**< Whether to use timer */
    int num_players;               /**< Number of players (1 for single-player) */
} GameConfig;

/**
 * @brief Player structure for multiplayer support
 */
typedef struct {
    char name[64];                 /**< Player name */
    int score;                     /**< Player's total score */
    int correct_answers;           /**< Number of correct answers */
    int wrong_answers;             /**< Number of wrong answers */
    int timeouts;                  /**< Number of timeouts */
} Player;

/**
 * @brief Game statistics (for single-player mode)
 */
typedef struct {
    int total_questions;           /**< Total questions answered */
    int correct_answers;           /**< Number of correct answers */
    int wrong_answers;             /**< Number of wrong answers */
    int timeouts;                  /**< Number of timeouts */
    int score;                     /**< Current score */
} GameStats;

/**
 * @brief Game state structure
 */
typedef struct {
    QuestionBank *question_bank;   /**< Pointer to question bank */
    GameConfig config;             /**< Game configuration */
    GameStats stats;               /**< Game statistics (single-player) */
    Player *players;               /**< Array of players (multiplayer) */
    int current_player;            /**< Current player index (0-based) */
    Timer timer;                   /**< Timer for questions */
    bool game_active;              /**< Whether game is currently active */
} GameState;

/**
 * @brief Initialize game state
 * 
 * @param state Pointer to GameState to initialize
 * @param bank Pointer to QuestionBank
 * @param config Game configuration
 * @return int 0 on success, -1 on error
 */
int game_init(GameState *state, QuestionBank *bank, const GameConfig *config);

/**
 * @brief Run a single game session
 * 
 * @param state Pointer to GameState
 * @return int Final score, or -1 on error
 */
int game_run(GameState *state);

/**
 * @brief Display a question and get user answer
 * 
 * @param state Pointer to GameState
 * @param question Pointer to Question to display
 * @return int User's answer (1-4), 0 on timeout/error, -1 on skip
 */
int game_ask_question(GameState *state, const Question *question);

/**
 * @brief Display game statistics
 * 
 * @param state Pointer to GameState
 */
void game_display_stats(const GameState *state);

/**
 * @brief Initialize players for multiplayer mode
 * 
 * @param state Pointer to GameState
 * @return int 0 on success, -1 on error
 */
int game_init_players(GameState *state);

/**
 * @brief Get current player
 * 
 * @param state Pointer to GameState
 * @return Player* Pointer to current player, or NULL if single-player
 */
Player* game_get_current_player(GameState *state);

/**
 * @brief Advance to next player's turn
 * 
 * @param state Pointer to GameState
 */
void game_next_player(GameState *state);

/**
 * @brief Clean up game state
 * 
 * @param state Pointer to GameState
 */
void game_cleanup(GameState *state);

/**
 * @brief Calculate score based on answer correctness and time
 * 
 * @param correct Whether answer was correct
 * @param time_remaining Remaining time in seconds
 * @param difficulty Question difficulty
 * @return int Points awarded
 */
int game_calculate_score(bool correct, int time_remaining, Difficulty difficulty);

#endif /* GAME_H */

