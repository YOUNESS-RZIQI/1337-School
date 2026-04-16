
#include "codexion.h"

short simulation(t_args data)
{
    pthread_t *p_th;
	t_simulation sim;

	p_th = NULL;
	sim.args = data;
	sim.coders = NULL;
	sim.dongles = NULL;
	sim.threads_at_barrier = 0;
    pthread_mutex_init(&sim.mutex_lock, NULL);
	pthread_cond_init(&sim.cond_lock, NULL);

	p_th = creat_n_threads_and_start_sim(data.number_of_coders, &sim);

    if (!p_th)
        return (null_error_message());

	free(p_th);
    return 0;
}

int	main(int argc, char **argv)
{
	t_args	data;

	data = convert_args(argc, argv);

	if (is_empty_args(data))
		return (input_error_message());

    return (simulation(data));

}
/*

	heap_extract_min
	heap_is_empty
	creat_dongels_list (Edited)
	free_dongles_list (Edited)

	what is index ?
	what is dead lock ?
*/