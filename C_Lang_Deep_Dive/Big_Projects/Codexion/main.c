/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrziqi <yrziqi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 07:25:48 by yrziqi            #+#    #+#             */
/*   Updated: 2026/04/12 12:04:18 by yrziqi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

short simulation(t_args data)
{
    pthread_t **p_th;
	t_simulation sim_data;

	p_th = NULL;
	sim_data.data = &data;
	
	
	sim_data.order_list = NULL;
    pthread_mutex_init(&sim_data.mutex_lock, NULL);

    p_th = creat_n_threads_and_start_sim(data.number_of_coders, &sim_data);

    if (!p_th)
        return (null_error_message());

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
