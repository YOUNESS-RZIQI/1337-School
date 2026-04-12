// #ifndef CODEXION_H
// #define CODEXION_H

// #include <stdio.h>
// #include <pthread.h>
// #include <string.h>
// #include <sys/time.h>


// typedef struct s_args
// {
//     int number_of_coders;
//     long long time_to_burnout;
//     long long time_to_compile;
//     long long time_to_debug;
//     long long time_to_refactor;
//     int number_of_compiles_required;
//     long long dongle_cooldown;
//     char *scheduler;
// } t_args;

// typedef enum e_coder_state
// {
//     COMPILING,
//     DEBUGGING,
//     REFACTORING,
//     WAITING

// } t_coder_state;

// typedef struct s_coder
// {

//     int coder_number;
//     long deadlline;
//     long last_compile_time;
//     struct s_coder *left;  
//     struct s_coder *right;
//     t_coder_state status;

// } t_coder;

// // short           is_full_digit(char *s);
// // long long       ft_atoi(const char *s);
// // t_args          convert_args(int argc, char **argv);
// // short           is_empty_args(t_args args);

// #include "utils_zero.h"
// #include "pars_args.h"

// #endif
