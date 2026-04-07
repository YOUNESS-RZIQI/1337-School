#include "codexion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

static int init_dongles(t_shared *shared)
{
    long long   i;
    long long   count;

    count = shared->args.number_of_coders;
    shared->dongles = malloc(sizeof(t_dongle) * count);
    if (!shared->dongles)
        return (0);
    i = 0;
    while (i < count)
    {
        shared->dongles[i].id = i;
        shared->dongles[i].is_available = 1;
        shared->dongles[i].release_time = 0;
        if (pthread_mutex_init(&shared->dongles[i].mutex, NULL) != 0)
        {
            while (--i >= 0)
                pthread_mutex_destroy(&shared->dongles[i].mutex);
            free(shared->dongles);
            return (0);
        }
        i++;
    }
    return (1);
}

static int init_coders(t_shared *shared)
{
    long long   i;
    long long   count;

    count = shared->args.number_of_coders;
    shared->coders = malloc(sizeof(t_coder) * count);
    if (!shared->coders)
        return (0);
    i = 0;
    while (i < count)
    {
        shared->coders[i].number = i + 1;
        shared->coders[i].compile_count = 0;
        shared->coders[i].last_compile_start = 0;
        shared->coders[i].deadline = shared->args.time_to_burnout;
        shared->coders[i].state = STATE_WAITING;
        shared->coders[i].left_dongle = -1;
        shared->coders[i].right_dongle = -1;
        shared->coders[i].shared = shared;
        i++;
    }
    return (1);
}

int init_simulation(t_shared *shared, t_args *args)
{
    shared->args = *args;
    shared->running = 1;
    shared->finished_count = 0;
    shared->dongles = NULL;
    shared->coders = NULL;
    if (pthread_mutex_init(&shared->log_mutex, NULL) != 0)
        return (0);
    if (pthread_mutex_init(&shared->state_mutex, NULL) != 0)
    {
        pthread_mutex_destroy(&shared->log_mutex);
        return (0);
    }
    if (!init_dongles(shared))
    {
        pthread_mutex_destroy(&shared->log_mutex);
        pthread_mutex_destroy(&shared->state_mutex);
        return (0);
    }
    if (!init_coders(shared))
    {
        pthread_mutex_destroy(&shared->log_mutex);
        pthread_mutex_destroy(&shared->state_mutex);
        free(shared->dongles);
        return (0);
    }
    return (1);
}

void cleanup_simulation(t_shared *shared)
{
    long long   i;
    long long   count;

    if (shared->dongles)
    {
        count = shared->args.number_of_coders;
        i = 0;
        while (i < count)
        {
            pthread_mutex_destroy(&shared->dongles[i].mutex);
            i++;
        }
        free(shared->dongles);
    }
    if (shared->coders)
        free(shared->coders);
    pthread_mutex_destroy(&shared->log_mutex);
    pthread_mutex_destroy(&shared->state_mutex);
}