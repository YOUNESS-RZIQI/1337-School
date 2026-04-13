#include "codexion.h"

void sorted_add_of_coders_and_full_initialized(t_order_list *order_list, char *schedular)
{
    int i;

    i = 0;
    // complit initializing : 1) left & right (left = N -1 , right = N + 1)     2) time variables
    // creat a linked list size of it is (coders) that have 1 value coder num & next, it add if not exist, &  sort if exist  


}

void *run_simulation(void *sim)
{
    t_simulation *sim_data_ad = (t_simulation*)sim;

    // if hes order.
    pthread_mutex_lock(&sim_data_ad->mutex_lock);
    sorted_addition_of_coders(sim_data_ad->order_list, sim_data_ad->data->scheduler);
    pthread_mutex_unlock(&sim_data_ad->mutex_lock);

    // lock unlock times : coders/2

    // alwasy the top ones N   & N = coders/2

    return NULL;
}

t_coder *create_coder(int i, t_simulation *sim)
{
    t_coder *coder;

    coder = malloc(sizeof(t_coder));
    if (!coder)
        return (NULL);
    coder->next = NULL;
    coder->coder_number = i + 1;
    coder->last_compile_time = 0;
    coder->time_to_burnout = sim->args.time_to_burnout;
    coder->deadline = coder->last_compile_time + coder->time_to_burnout;
    coder->creation_time = /* Use sys/time the function gettimeofday() */ ;
    coder->status = WAITING;
    
    return (coder);
}

void    add_coder_to_list(t_coder *coder, t_simulation *sim)
{

    coder->next = NULL;
    if (sim->coders == NULL)
    {
        sim->coders = coder;
    }
    else
    {
        t_coder *current = sim->coders;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = coder;
    }
}

pthread_t **creat_n_threads_and_start_sim(int num, t_simulation *sim)
{
    pthread_t **p_th;
    t_coder *coder;
    int i;
    
    p_th = (pthread_t **)malloc(num * sizeof(*p_th));

    if (p_th == NULL)
        return NULL;
    i = 0;
    while (i++ < num)
    {
        coder = create_coder(i, sim);
        if (!coder)
            return (NULL);
        add_coder_to_list(coder, sim);
        pthread_create(&p_th[i], NULL, run_simulation, (void*)sim);
    }
    pthread_join(p_th[0], NULL);

    return p_th;
}
/*
        coder.coder_number = i + 1;
        coder.creation_time;
        coder.last_compile_time = 0;
        coder.deadline = sim_data_address->coder.last_compile_time + sim_data_address->data->time_to_burnout;
        coder.dongle.is_available = 1;
        coder.dongle.left_dongle = NULL;
        coder.dongle.right_dongle = NULL;
        coder.status = WAITING;
	    coder.is_usb_avilable = 1;

*/
