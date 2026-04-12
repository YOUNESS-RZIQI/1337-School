/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrziqi <yrziqi@student.42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 07:25:48 by yrziqi             #+#    #+#             */
/*   Updated: 2026/04/12 08:30:00 by yrziqi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>
#include <unistd.h>

// Global simulation context
t_simulation_context *g_sim_ctx = NULL;

short simulation(t_args data)
{
    t_args *args = malloc(sizeof(t_args));
    if (!args) return null_error_message();
    *args = data;
    
    g_sim_ctx = create_simulation(args);
    if (!g_sim_ctx) {
        free(args);
        return null_error_message();
    }
    
    // TODO: Start monitor thread and wait for simulation to complete
    // For now, sleep briefly then cleanup
    sleep(1);
    
    destroy_simulation(g_sim_ctx);
    return 0;
}

int main(int argc, char **argv)
{
    t_args data;
    
    data = convert_args(argc, argv);
    
    if (is_empty_args(data))
        return input_error_message();
    
    return simulation(data);
}