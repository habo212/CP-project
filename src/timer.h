/**
 * @file timer.h
 * @brief Timer thread functionality for question time limits
 * 
 * This module implements a timer using pthreads that counts down
 * for each question, demonstrating the use of threads in C.
 */

#ifndef TIMER_H
#define TIMER_H

#include <pthread.h>
#include <stdbool.h>
#include <time.h>

/**
 * @brief Timer structure for managing countdown
 */
typedef struct {
    pthread_t thread;          /**< Thread handle */
    int seconds;               /**< Total seconds for timer */
    int remaining;             /**< Remaining seconds */
    bool running;              /**< Whether timer is running */
    bool expired;              /**< Whether timer has expired */
    pthread_mutex_t mutex;     /**< Mutex for thread-safe access */
} Timer;

/**
 * @brief Initialize a timer
 * 
 * @param timer Pointer to Timer structure
 * @param seconds Number of seconds for the timer
 * @return int 0 on success, -1 on error
 */
int timer_init(Timer *timer, int seconds);

/**
 * @brief Start the timer countdown
 * 
 * @param timer Pointer to Timer structure
 * @return int 0 on success, -1 on error
 */
int timer_start(Timer *timer);

/**
 * @brief Stop the timer
 * 
 * @param timer Pointer to Timer structure
 * @return int 0 on success, -1 on error
 */
int timer_stop(Timer *timer);

/**
 * @brief Get remaining seconds
 * 
 * @param timer Pointer to Timer structure
 * @return int Remaining seconds, or -1 on error
 */
int timer_get_remaining(Timer *timer);

/**
 * @brief Check if timer has expired
 * 
 * @param timer Pointer to Timer structure
 * @return bool true if expired, false otherwise
 */
bool timer_is_expired(Timer *timer);

/**
 * @brief Reset the timer
 * 
 * @param timer Pointer to Timer structure
 * @param seconds New number of seconds
 * @return int 0 on success, -1 on error
 */
int timer_reset(Timer *timer, int seconds);

/**
 * @brief Clean up timer resources
 * 
 * @param timer Pointer to Timer structure
 */
void timer_cleanup(Timer *timer);

#endif /* TIMER_H */

