#ifndef CODEXION_H
#define CODEXION_H

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>


typedef struct s_args
{
    int number_of_coders;
    long long time_to_burnout;
    long long time_to_compile;
    long long time_to_debug;
    long long time_to_refactor;
    int number_of_compiles_required;
    long long dongle_cooldown;
    char *scheduler;
} t_args;

typedef enum e_coder_state
{
    COMPILING,
    DEBUGGING,
    REFACTORING,
    WAITING

} t_coder_state;

typedef struct s_coder
{
    struct s_coder  *next;

    int             coder_number;
    long long       last_compile_time;
    long long       deadline;
    long long       creation_time;
    long long       time_to_burnout;
    t_coder_state   status;
    
} t_coder;

typedef struct s_heap {
    t_coder **elements;
    int size;
    int capacity;
    char scheduler;  // 'f' for fifo, 'e' for edf
} t_heap;

typedef struct s_dongle
{
    int             number;
    short           is_rested;
    long long       how_much_to_rest;
    long long       toked_at;
    long long       toked_by;
    short           left_coder;
    short           right_coder;
    struct s_dongle *next;
    t_heap          *waiting_coders;

} t_dongle;

typedef struct s_simulation
{
    t_args          args;
    t_dongle        *dongles;
    t_coder         *coders;
    pthread_mutex_t mutex_lock;
    pthread_cond_t  cond_lock;
    int             threads_at_barrier;
    
} t_simulation;

// short           is_full_digit(char *s);
// long long       ft_atoi(const char *s);
// t_args          convert_args(int argc, char **argv);
// short           is_empty_args(t_args args);

#include "utils_zero.h"
#include "pars_args.h"
#include "threads.h"
#include "errors.h"

#endif
