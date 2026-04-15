#include "codexion.h"

void *run_simulation(void *sim)
{
    t_simulation    *sim_data_ad = (t_simulation*)sim;
    int             i;

    pthread_mutex_lock(&sim_data_ad->mutex_lock);
    sim_data_ad->threads_at_barrier++;
    while (sim_data_ad->threads_at_barrier < sim_data_ad->args.number_of_coders)
        pthread_cond_wait(&sim_data_ad->cond_lock, &sim_data_ad->mutex_lock);
    if (sim_data_ad->threads_at_barrier == sim_data_ad->args.number_of_coders)
        pthread_cond_broadcast(&sim_data_ad->cond_lock);
    pthread_mutex_unlock(&sim_data_ad->mutex_lock);    
    
    
    // // lock unlock times : coders/2

    // // alwasy the top ones N   & N = coders/2













    // pthread_mutex_lock(&(*sim_data_ad).mutex_lock);
    // t_dongle *head = sim_data_ad->dongles;
    // while (head)
    // {
    //     printf("%lld", head->how_much_to_rest);
    //     head = head->next;
    // }
    // printf("\n");
    
    // pthread_mutex_unlock(&(*sim_data_ad).mutex_lock);

    return NULL;
}

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
    coder->head = 1;
    coder->tail = sim->args.number_of_coders;
    
    return (coder);
}

void    add_coder_to_list(t_coder *coder, t_simulation *sim)
{
    coder->head = 0;
    coder->tail = 0;
    coder->next = NULL;

    if (sim->coders == NULL)
    {
        coder->head = 1;
        coder->tail = 1;
        sim->coders = coder;
    }
    else
    {
        t_coder *current = sim->coders;
        while (current->next != NULL)
            current = current->next;
        
        current->tail = 0;
        coder->tail = 1;
        current->next = coder;
    }
}

void    complit_creat_dongle(int    number, t_dongle   *dongle, t_simulation   *sim)
{
    if (number == sim->args.number_of_coders)
    {
        dongle->left_coder = number - 1;
        dongle->right_coder = 1;
    }
    else if (number == 1)
    {
        dongle->left_coder = sim->args.number_of_coders;
        dongle->right_coder = number + 1;
    }
    else
    {
        dongle->left_coder = number - 1;
        dongle->right_coder = number + 1;
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
    complit_creat_dongle(number, dongle, sim);
    dongle->next = NULL;
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

void    creat_dongels_list(t_simulation *sim)
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
            return (free_dongles_list(sim->dongles));
        if (!sim->dongles)
            sim->dongles = dongle;
        else
            prev->next = dongle;
        prev = dongle;
    }
}


// void    add_coder_to_list(t_coder *coder, t_simulation *sim)
// {
// 
    // coder->next = NULL;
    // if (sim->coders == NULL)
    // {
        // sim->coders = coder;
    // }
    // else
    // {
        // t_coder *current = sim->coders;
        // while (current->next != NULL)
        // {
            // current = current->next;
        // }
        // current->next = coder;
    // }
// }

pthread_t *creat_n_threads_and_start_sim(int num, t_simulation *sim)
{
    pthread_t *p_th;
    t_coder *coder;
    int i;

    p_th = malloc(num * sizeof(pthread_t));
    if (p_th == NULL)
        return NULL;
    i = 0;
    while (i++ < num)
    {
        coder = create_coder(i, sim);
        if (!coder)
            return (free(p_th), NULL);
        add_coder_to_list(coder, sim);
        pthread_create(&p_th[i], NULL, run_simulation, (void*)sim);
    }
    creat_dongels_list(sim);
    pthread_cond_broadcast(&sim->cond_lock);
    i = 0;
    while (i < num)
        pthread_join(p_th[i++], NULL);
    return p_th;
}
