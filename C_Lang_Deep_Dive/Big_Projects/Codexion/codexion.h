#ifndef CODEXION_H
#define CODEXION_H

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
// #include <string.h>
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

typedef struct s_order_list
{
    int                 order;
    struct s_order_list *next;
} t_order_list;

typedef struct s_dongle
{
    t_order_list        *order_list;
    short               is_available;
    struct s_dongle     *left_dongle;
    struct s_dongle     *right_dongle;

} t_dongle;

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

typedef struct s_simulation
{
    t_args          args;
    t_coder         *coders;
    pthread_mutex_t *mutex_lock;
    
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
