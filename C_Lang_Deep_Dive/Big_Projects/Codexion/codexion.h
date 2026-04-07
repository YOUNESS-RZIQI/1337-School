#ifndef CODEXION_H
#define CODEXION_H

#include <pthread.h>
#include <sys/time.h>
#include <stddef.h>
#include <string.h>

typedef struct s_args
{
    long long number_of_coders;
    long long time_to_burnout;
    long long time_to_compile;
    long long time_to_debug;
    long long time_to_refactor;
    long long number_of_compiles_required;
    long long dongle_cooldown;
    char *scheduler;
} t_args;

typedef enum e_coder_state
{
    STATE_WAITING,
    STATE_COMPILE,
    STATE_DEBUG,
    STATE_REFACTOR,
    STATE_BURNOUT,
    STATE_DONE
} t_coder_state;

typedef struct s_dongle
{
    int             id;
    pthread_mutex_t mutex;
    int             is_available;
    long long       release_time;
} t_dongle;

typedef struct s_shared t_shared;

typedef struct s_coder
{
    long long       number;
    long long       compile_count;
    long long       last_compile_start;
    long long       deadline;
    t_coder_state   state;
    int             left_dongle;
    int             right_dongle;
    pthread_t       thread;
    t_shared        *shared;
} t_coder;

struct s_shared
{
    t_args          args;
    t_dongle        *dongles;
    t_coder         *coders;
    int             running;
    pthread_mutex_t log_mutex;
    pthread_mutex_t state_mutex;
    int             finished_count;
};

short           is_full_digit(char *s);
long long       ft_atoi(const char *s);
t_args          convert_args(int argc, char **argv);
short           is_empty_args(t_args args);
long long       get_timestamp(void);
void            log_message(t_shared *shared, long long coder_num, const char *msg);
int             init_simulation(t_shared *shared, t_args *args);
void            *run_simulation(void *arg);
void            cleanup_simulation(t_shared *shared);
int             is_dongle_available(t_dongle *dongle, long long now);
int             try_acquire_dongle(t_dongle *dongle, long long now);
void            release_dongle(t_dongle *dongle, long long now, long long cooldown);
int             get_left_dongle_id(t_coder *coder);
int             get_right_dongle_id(t_coder *coder, long long total);
int             acquire_both_dongles(t_coder *coder, t_shared *shared, long long now);
void            release_both_dongles(t_coder *coder, t_shared *shared, long long now);
int             check_burnout(t_coder *coder, long long now);
void            do_compile(t_coder *coder, t_shared *shared, long long now);
void            do_debug(t_coder *coder, t_shared *shared, long long now);
void            do_refactor(t_coder *coder, t_shared *shared, long long now);

#endif