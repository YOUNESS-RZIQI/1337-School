#ifndef THREADS_H
# define THREADS_H

# include "codexion.h"
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>

// Global variables for cleanup (simplistic)
static t_shared *g_shared = NULL;
static t_coder *g_coders = NULL;

// Helper to get current time in ms
static long long current_time_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000 + (long long)tv.tv_usec / 1000;
}

pthread_t **creat_n_threads(int num, t_args *args)
{
    pthread_t **threads;
    t_shared *shared;
    t_coder *coders;
    long long now;
    int i;
    
    // Allocate threads array (pointers to pthread_t)
    threads = malloc(num * sizeof(pthread_t *));
    if (!threads) return NULL;
    
    // Create shared simulation context
    shared = malloc(sizeof(t_shared));
    if (!shared) {
        free(threads);
        return NULL;
    }
    shared->args = *args;
    shared->running = 1;
    shared->finished_count = 0;
    shared->dongles = NULL;
    shared->coders = NULL;
    if (pthread_mutex_init(&shared->log_mutex, NULL) != 0) {
        free(shared);
        free(threads);
        return NULL;
    }
    if (pthread_mutex_init(&shared->state_mutex, NULL) != 0) {
        pthread_mutex_destroy(&shared->log_mutex);
        free(shared);
        free(threads);
        return NULL;
    }
    
    // Allocate coders array
    shared->coders = malloc(num * sizeof(t_coder));
    if (!shared->coders) {
        pthread_mutex_destroy(&shared->log_mutex);
        pthread_mutex_destroy(&shared->state_mutex);
        free(shared);
        free(threads);
        return NULL;
    }
    
    // Allocate dongles array
    shared->dongles = malloc(num * sizeof(t_dongle));
    if (!shared->dongles) {
        free(shared->coders);
        pthread_mutex_destroy(&shared->log_mutex);
        pthread_mutex_destroy(&shared->state_mutex);
        free(shared);
        free(threads);
        return NULL;
    }
    // Initialize dongles
    for (i = 0; i < num; i++) {
        shared->dongles[i].id = i;
        shared->dongles[i].is_held = 0;
        shared->dongles[i].available_at = 0;
        pq_init(&shared->dongles[i].queue);
        pthread_mutex_init(&shared->dongles[i].mutex, NULL);
        pthread_cond_init(&shared->dongles[i].cond, NULL);
    }
    
    // Get start time
    struct timeval tv;
    gettimeofday(&tv, NULL);
    shared->start_time = (long long)tv.tv_sec * 1000 + (long long)tv.tv_usec / 1000;
    
    coders = shared->coders;
    now = shared->start_time;
    
    // Initialize coders and create threads
    for (i = 0; i < num; i++) {
        threads[i] = malloc(sizeof(pthread_t));
        if (!threads[i]) {
            // Cleanup previously allocated resources
            for (int j = 0; j < i; j++) free(threads[j]);
            free(threads);
            // Cleanup shared
            for (int j = 0; j < num; j++) {
                pthread_mutex_destroy(&shared->dongles[j].mutex);
                pthread_cond_destroy(&shared->dongles[j].cond);
            }
            free(shared->dongles);
            free(shared->coders);
            pthread_mutex_destroy(&shared->log_mutex);
            pthread_mutex_destroy(&shared->state_mutex);
            free(shared);
            return NULL;
        }
        
        // Initialize coder
        coders[i].number = i + 1;
        coders[i].compile_count = 0;
        coders[i].last_compile_start = 0;
        coders[i].deadline = now + args->ttb;  // ttb = time_to_burnout
        coders[i].left_id = get_left_dongle_id(&coders[i]);
        coders[i].right_id = get_right_dongle_id(&coders[i], num);
        if (coders[i].left_id <= coders[i].right_id) {
            coders[i].first_id = coders[i].left_id;
            coders[i].second_id = coders[i].right_id;
        } else {
            coders[i].first_id = coders[i].right_id;
            coders[i].second_id = coders[i].left_id;
        }
        coders[i].shared = shared;
        coders[i].thread = threads[i];
        
// Create thread
        pthread_create(threads[i], NULL, run_simulation, &coders[i]);
        // Save thread ID in coder struct
        coders[i].thread = *threads[i];
    
    return threads;
}

// Cleanup function to be called after threads finish
void cleanup_created_threads(pthread_t **threads, int num)
{
    if (!threads || !g_shared) return;
    
    // Stop simulation
    g_shared->running = 0;
    
    // Join threads
    for (int i = 0; i < num; i++) {
        if (threads[i]) {
            pthread_join(*threads[i], NULL);
            free(threads[i]);
        }
    }
    free(threads);
    
    // Cleanup shared resources
    if (g_shared) {
        for (int i = 0; i < num; i++) {
            pthread_mutex_destroy(&g_shared->dongles[i].mutex);
            pthread_cond_destroy(&g_shared->dongles[i].cond);
        }
        free(g_shared->dongles);
        free(g_shared->coders);
        pthread_mutex_destroy(&g_shared->log_mutex);
        pthread_mutex_destroy(&g_shared->state_mutex);
        free(g_shared);
        g_shared = NULL;
    }
}

#endif