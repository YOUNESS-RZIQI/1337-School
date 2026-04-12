#include "codexion.h"

void sorted_add_of_coders_and_full_initialized(t_order_list *order_list, char *schedular)
{
    int i;

    i = 0;
    // complit initializing : 1) left & right (left = N -1 , right = N + 1)     2) time variables
    // creat a linked list size of it is (coders) that have 1 value coder num & next, it add if not exist, &  sort if exist  


}

void *run_simulation(void *sim_data_address)
{
    t_simulation *sim_data_ad = (t_simulation*)sim_data_address;

    // if hes order.
    pthread_mutex_lock(&sim_data_ad->mutex_lock);
    sorted_addition_of_coders(sim_data_ad->order_list, sim_data_ad->data->scheduler);
    pthread_mutex_unlock(&sim_data_ad->mutex_lock);

    // lock unlock times : coders/2

    // alwasy the top ones N   & N = coders/2

    return NULL;
}

pthread_t **creat_n_threads_and_start_sim(int num, t_simulation *sim_data_address)
{
    pthread_t **p_th;
    int i;
    
    p_th = (pthread_t **)malloc(num * sizeof(*p_th));

    if (p_th == NULL)
        return NULL;
    i = 0;
    while (i++ < num)
    {
        sim_data_address->coder.coder_number = i + 1;
        sim_data_address->coder.creation_time;
        sim_data_address->coder.last_compile_time = 0;
        sim_data_address->coder.deadline = sim_data_address->coder.last_compile_time + sim_data_address->data->time_to_burnout;
        sim_data_address->coder.dongle.is_available = 1;
        sim_data_address->coder.dongle.left_dongle = NULL;
        sim_data_address->coder.dongle.right_dongle = NULL;
        sim_data_address->coder.status = WAITING;
	    sim_data_address->coder.is_usb_avilable = 1;
        
        pthread_create(&p_th[i], NULL, run_simulation, sim_data_address);
    }
    pthread_join(p_th[0], NULL);

    return p_th;
}
