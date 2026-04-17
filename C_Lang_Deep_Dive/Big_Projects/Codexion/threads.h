#include "codexion.h"

// Start Coder

t_coder *create_coder(int coder_number, t_simulation *sim)
{
    t_coder *coder;
    struct timeval tv;

    coder = malloc(sizeof(t_coder));
    if (!coder)
        return (NULL);
    coder->next = NULL;
    coder->coder_number = coder_number;
    coder->last_compile_time = 0;
    coder->time_to_burnout = sim->args.time_to_burnout;
    coder->deadline = coder->last_compile_time + coder->time_to_burnout;
    gettimeofday(&tv, NULL);
    coder->creation_time = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);
    coder->status = WAITING;
    
    return (coder);
}

void    add_coder_to_list(t_coder *coder, t_simulation *sim)
{
    // coder->head = 0;
    // coder->tail = 0;
    coder->next = NULL;

    if (sim->coders == NULL)
    {
        // coder->head = 1;
        // coder->tail = 1;
        sim->coders = coder;
    }
    else
    {
        t_coder *current = sim->coders;
        while (current->next != NULL)
            current = current->next;
        
        // current->tail = 0;
        // coder->tail = 1;
        current->next = coder;
    }
}

// End Coder



// Start Dongle

void    creat_dongle_2(int    number, t_dongle   *dongle, t_simulation   *sim)
{
    if (number == sim->args.number_of_coders)
    {
        dongle->left_coder = number - 1;
        dongle->right_coder = number;
    }
    else if (number == 1)
    {
        dongle->left_coder = sim->args.number_of_coders;
        dongle->right_coder = 1;
    }
    else
    {
        dongle->left_coder = number - 1;
        dongle->right_coder = number;
    }
}

t_dongle *create_dongle(int number, t_simulation *sim)
{
    t_dongle *dongle;

    dongle = malloc(sizeof(t_dongle));
    if (dongle == NULL)
        return NULL;
    dongle->number = number;
    dongle->is_rested = 1;
    dongle->how_much_to_rest = sim->args.dongle_cooldown;
    dongle->toked_at = 0;
    dongle->toked_by = 0;
    creat_dongle_2(number, dongle, sim);
    dongle->next = NULL;
    dongle->waiting_coder = NULL;  // Simple pointer - no heap needed!

    return dongle;
}

void    free_dongles_list(t_dongle *head)
{
    t_dongle *current = head;
    t_dongle *next;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
}

short    creat_dongels_list(t_simulation *sim)
{
    int i;
    int num_dongles;
    t_dongle *prev;
    t_dongle *dongle;

    i = 0;
    prev = NULL;
    num_dongles = sim->args.number_of_coders;
    while (i++ < num_dongles)
    {
        dongle = create_dongle(i, sim);
        if (!dongle)
            return (free_dongles_list(sim->dongles), 1);
        if (!sim->dongles)
            sim->dongles = dongle;
        else
            prev->next = dongle;
        prev = dongle;
    }
    return 0;
}

// End Dongle



// Simple Simulation Functions (no heap needed!)

long long get_current_time_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);
}

t_dongle *find_dongle_for_coder(t_simulation *sim, int coder_number, int left_or_right)
{
    t_dongle *dongle = sim->dongles;
    while (dongle != NULL)
    {
        if (left_or_right == 0 && dongle->left_coder == coder_number)
            return dongle;
        if (left_or_right == 1 && dongle->right_coder == coder_number)
            return dongle;
        dongle = dongle->next;
    }
    return NULL;
}

int try_acquire_dongle(t_dongle *dongle, t_coder *coder, t_simulation *sim)
{
    int result;
    
    pthread_mutex_lock(&sim->dongle_lock);
    if (dongle->is_rested == 1)
    {
        dongle->is_rested = 0;
        dongle->toked_by = coder->coder_number;
        result = 1;
    }
    else
    {
        if (dongle->waiting_coder == NULL)
            dongle->waiting_coder = coder;
        result = 0;
    }
    pthread_mutex_unlock(&sim->dongle_lock);
    return result;
}

void release_dongle(t_dongle *dongle, t_simulation *sim)
{
    pthread_mutex_lock(&sim->dongle_lock);
    dongle->toked_at = get_current_time_ms();
    
    if (dongle->waiting_coder != NULL)
    {
        dongle->toked_by = dongle->waiting_coder->coder_number;
        dongle->is_rested = 0;
        dongle->waiting_coder = NULL;
    }
    else
    {
        dongle->is_rested = 1;
        dongle->toked_by = 0;
    }
    pthread_mutex_unlock(&sim->dongle_lock);
}

void coder_work(t_coder *coder, t_simulation *sim)
{
    t_dongle *left_dongle;
    t_dongle *right_dongle;
    
    left_dongle = find_dongle_for_coder(sim, coder->coder_number, 0);
    right_dongle = find_dongle_for_coder(sim, coder->coder_number, 1);
    
    if (!left_dongle || !right_dongle)
        return;
    
    try_acquire_dongle(left_dongle, coder, sim);
    printf("%lld %d has taken a dongle\n", get_current_time_ms(), coder->coder_number);
    
    try_acquire_dongle(right_dongle, coder, sim);
    printf("%lld %d has taken a dongle\n", get_current_time_ms(), coder->coder_number);
    
    printf("%lld %d is compiling\n", get_current_time_ms(), coder->coder_number);
    usleep(sim->args.time_to_compile * 1000);
    
    coder->status = DEBUGGING;
    printf("%lld %d is debugging\n", get_current_time_ms(), coder->coder_number);
    usleep(sim->args.time_to_debug * 1000);
    
    coder->status = REFACTORING;
    printf("%lld %d is refactoring\n", get_current_time_ms(), coder->coder_number);
    usleep(sim->args.time_to_refactor * 1000);
    
    release_dongle(left_dongle, sim);
    release_dongle(right_dongle, sim);
    
    coder->last_compile_time = get_current_time_ms();
    coder->deadline = coder->last_compile_time + coder->time_to_burnout;
}

// Thread argument
typedef struct s_thread_arg
{
    t_simulation *sim;
    t_coder      *coder;
} t_thread_arg;

void *run_simulation(void *arg)
{
    t_thread_arg *thread_arg = (t_thread_arg*)arg;
    t_simulation *sim_data_ad = thread_arg->sim;
    t_coder *coder = thread_arg->coder;
    int compile_count = 0;

    pthread_mutex_lock(&sim_data_ad->mutex_lock);
    sim_data_ad->threads_at_barrier++;
    if (sim_data_ad->threads_at_barrier == sim_data_ad->args.number_of_coders)
    {
        pthread_cond_broadcast(&sim_data_ad->cond_lock);
    }
    else
    {
        while (sim_data_ad->threads_at_barrier < sim_data_ad->args.number_of_coders)
            pthread_cond_wait(&sim_data_ad->cond_lock, &sim_data_ad->mutex_lock);
    }
    pthread_mutex_unlock(&sim_data_ad->mutex_lock);

    while (compile_count < sim_data_ad->args.number_of_compiles_required)
    {
        coder_work(coder, sim_data_ad);
        compile_count++;
    }

    free(arg);
    return NULL;
}

pthread_t *creat_n_threads_and_start_sim(int num, t_simulation *sim)
{
    pthread_t *p_th;
    t_coder *coder;
    int i;
    t_thread_arg *args;

    p_th = malloc(num * sizeof(pthread_t));
    if (p_th == NULL)
        return NULL;
    
    if (creat_dongels_list(sim) == 1)
    {
        free(p_th);
        return NULL;
    }
    
    i = 0;
    while (i < num)
    {
        coder = create_coder(i + 1, sim);
        if (!coder)
        {
            free(p_th);
            return NULL;
        }
        add_coder_to_list(coder, sim);
        
        args = malloc(sizeof(t_thread_arg));
        if (!args)
        {
            free(p_th);
            return NULL;
        }
        args->sim = sim;
        args->coder = coder;
        
        pthread_create(&p_th[i], NULL, run_simulation, args);
        i++;
    }
    
    i = 0;
    while (i < num)
        pthread_join(p_th[i++], NULL);
    
    return p_th;
}
