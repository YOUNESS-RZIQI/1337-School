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

void heap_init(t_heap *heap, const char *scheduler)
{
    heap->elements = malloc(2 * sizeof(t_coder*));
    if (heap->elements == NULL)
    {
        heap->capacity = 0;
        heap->size = 0;
        return;
    }
    heap->size = 0;
    heap->capacity = 2;
    heap->scheduler = scheduler[0];  // 'f' or 'e'
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
    creat_dongle_2(number, dongle, sim);
    dongle->next = NULL;
    dongle->waiting_coders = malloc(sizeof(t_heap));
    if (dongle->waiting_coders)
        heap_init(dongle->waiting_coders, sim->args.scheduler);
    else
        return (free(dongle), NULL);
    if (dongle->waiting_coders->capacity == 0)
        return (free(dongle), NULL);

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






#include <stdlib.h>

void *run_simulation(void *sim)
{
    t_simulation    *sim_data_ad;
    int             i;

    sim_data_ad = (t_simulation*)sim;
    pthread_mutex_lock(&sim_data_ad->mutex_lock);
    sim_data_ad->threads_at_barrier++;
    while (sim_data_ad->threads_at_barrier < sim_data_ad->args.number_of_coders)
        pthread_cond_wait(&sim_data_ad->cond_lock, &sim_data_ad->mutex_lock);
    if (sim_data_ad->threads_at_barrier == sim_data_ad->args.number_of_coders)
        pthread_cond_broadcast(&sim_data_ad->cond_lock);
    pthread_mutex_unlock(&sim_data_ad->mutex_lock);    
    
    
    // // lock unlock times : coders/2

    // // alwasy the top ones N   & N = coders/2






    pthread_mutex_lock(&(*sim_data_ad).mutex_lock);
    t_dongle *head = sim_data_ad->dongles;
    system("clear");
    while (head)
    {
        printf("Number: %d\n", head->number);
        printf("left coder: %d\n", head->left_coder);
        printf("right coder: %d\n", head->right_coder);
        printf("\n");
        head = head->next;
    }
    printf("\n");
    
    pthread_mutex_unlock(&(*sim_data_ad).mutex_lock);

    return NULL;
}

pthread_t *creat_n_threads_and_start_sim(int num, t_simulation *sim)
{
    pthread_t *p_th;
    t_coder *coder;
    int i;


    p_th = malloc(num * sizeof(pthread_t));
    if (p_th == NULL)
        return NULL;
    i = creat_dongels_list(sim);
    if (i == 1)
        return (NULL);
    while (i++ < num)
    {
        coder = create_coder(i, sim);
        if (!coder)
            return (free(p_th), NULL);
        add_coder_to_list(coder, sim);
        pthread_create(&p_th[i], NULL, run_simulation, (void*)sim);
    }
    i = 0;
    while (i < num)
        pthread_join(p_th[i++], NULL);
    return p_th;
}
