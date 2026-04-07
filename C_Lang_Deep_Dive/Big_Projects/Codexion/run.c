#include "codexion.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * run_simulation - Main coder thread function
 * Why: This is the entry point for each coder thread
 * Problem solved: Implements state machine and burnout detection
 */
void *run_simulation(void *arg)
{
    t_coder     *coder;
    t_shared    *shared;
    long long   now;
    int         acquired;

    coder = (t_coder *)arg;
    shared = coder->shared;
    now = get_timestamp();
    coder->last_compile_start = now;
    coder->deadline = now + shared->args.time_to_burnout;
    while (shared->running && coder->state != STATE_BURNOUT)
    {
        if (coder->compile_count >= shared->args.number_of_compiles_required)
        {
            pthread_mutex_lock(&shared->state_mutex);
            shared->finished_count++;
            pthread_mutex_unlock(&shared->state_mutex);
            coder->state = STATE_DONE;
            break;
        }
        now = get_timestamp();
        if (check_burnout(coder, now))
        {
            coder->state = STATE_BURNOUT;
            pthread_mutex_lock(&shared->state_mutex);
            shared->running = 0;
            pthread_mutex_unlock(&shared->state_mutex);
            log_message(shared, coder->number, "burned out");
            break;
        }
        acquired = acquire_both_dongles(coder, shared, now);
        if (acquired)
        {
            log_message(shared, coder->number, "has taken a dongle");
            log_message(shared, coder->number, "has taken a dongle");
            now = get_timestamp();
            coder->last_compile_start = now;
            coder->deadline = now + shared->args.time_to_burnout;
            do_compile(coder, shared, now);
            release_both_dongles(coder, shared, now);
            coder->compile_count++;
            if (coder->compile_count >= shared->args.number_of_compiles_required)
            {
                pthread_mutex_lock(&shared->state_mutex);
                shared->finished_count++;
                if (shared->finished_count >= shared->args.number_of_coders)
                    shared->running = 0;
                pthread_mutex_unlock(&shared->state_mutex);
            }
            do_debug(coder, shared, get_timestamp());
            do_refactor(coder, shared, get_timestamp());
        }
        else
            usleep(100);
    }
    return (NULL);
}