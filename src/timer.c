/**
 * @file timer.c
 * @brief Implementation of timer thread functionality
 */

#include "timer.h"
#include <unistd.h>
#include <stdlib.h>

/**
 * @brief Timer thread function
 * 
 * This function runs in a separate thread and counts down the timer.
 * It demonstrates the use of pthreads for concurrent operations.
 * 
 * @param arg Pointer to Timer structure
 * @return void* Always NULL
 */
static void* timer_thread_func(void *arg) {
    Timer *timer = (Timer*)arg;
    if (timer == NULL) {
        return NULL;
    }
    
    pthread_mutex_lock(&timer->mutex);
    timer->running = true;
    timer->expired = false;
    timer->remaining = timer->seconds;
    pthread_mutex_unlock(&timer->mutex);
    
    // Countdown loop
    while (timer->remaining > 0) {
        sleep(1);
        
        pthread_mutex_lock(&timer->mutex);
        if (!timer->running) {
            pthread_mutex_unlock(&timer->mutex);
            break;
        }
        timer->remaining--;
        pthread_mutex_unlock(&timer->mutex);
    }
    
    // Mark as expired if we reached zero
    pthread_mutex_lock(&timer->mutex);
    if (timer->remaining == 0) {
        timer->expired = true;
    }
    timer->running = false;
    pthread_mutex_unlock(&timer->mutex);
    
    return NULL;
}

int timer_init(Timer *timer, int seconds) {
    if (timer == NULL) {
        return -1;
    }
    
    if (seconds <= 0) {
        return -1;
    }
    
    timer->seconds = seconds;
    timer->remaining = seconds;
    timer->running = false;
    timer->expired = false;
    
    if (pthread_mutex_init(&timer->mutex, NULL) != 0) {
        return -1;
    }
    
    return 0;
}

int timer_start(Timer *timer) {
    if (timer == NULL) {
        return -1;
    }
    
    pthread_mutex_lock(&timer->mutex);
    if (timer->running) {
        pthread_mutex_unlock(&timer->mutex);
        return -1; // Already running
    }
    pthread_mutex_unlock(&timer->mutex);
    
    // Create thread
    if (pthread_create(&timer->thread, NULL, timer_thread_func, timer) != 0) {
        return -1;
    }
    
    return 0;
}

int timer_stop(Timer *timer) {
    if (timer == NULL) {
        return -1;
    }
    
    pthread_mutex_lock(&timer->mutex);
    timer->running = false;
    pthread_mutex_unlock(&timer->mutex);
    
    // Wait for thread to finish
    pthread_join(timer->thread, NULL);
    
    return 0;
}

int timer_get_remaining(Timer *timer) {
    if (timer == NULL) {
        return -1;
    }
    
    pthread_mutex_lock(&timer->mutex);
    int remaining = timer->remaining;
    pthread_mutex_unlock(&timer->mutex);
    
    return remaining;
}

bool timer_is_expired(Timer *timer) {
    if (timer == NULL) {
        return false;
    }
    
    pthread_mutex_lock(&timer->mutex);
    bool expired = timer->expired;
    pthread_mutex_unlock(&timer->mutex);
    
    return expired;
}

int timer_reset(Timer *timer, int seconds) {
    if (timer == NULL || seconds <= 0) {
        return -1;
    }
    
    // Stop current timer if running
    if (timer->running) {
        timer_stop(timer);
    }
    
    pthread_mutex_lock(&timer->mutex);
    timer->seconds = seconds;
    timer->remaining = seconds;
    timer->expired = false;
    pthread_mutex_unlock(&timer->mutex);
    
    return 0;
}

void timer_cleanup(Timer *timer) {
    if (timer == NULL) {
        return;
    }
    
    // Stop timer if running
    if (timer->running) {
        timer_stop(timer);
    }
    
    pthread_mutex_destroy(&timer->mutex);
}

