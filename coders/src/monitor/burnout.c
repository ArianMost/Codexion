/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   burnout.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amostash <amostash@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 15:45:20 by amostash          #+#    #+#             */
/*   Updated: 2026/07/22 16:50:35 by amostash         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_burnout(t_sim *simulation, long long current_time)
{
	long long	start_time;
	int			i;

	i = 0;
	while (i < simulation->cfg.number_of_coders)
	{
		pthread_mutex_lock(&simulation->coders[i].field_mutex);
		start_time = simulation->coders[i].last_compile_start;
		if (!simulation->coders[i].burned_out
			&& current_time - start_time > simulation->cfg.time_to_burnout)
		{
			simulation->coders[i].burned_out = 1;
			pthread_mutex_unlock(&simulation->coders[i].field_mutex);
			return (simulation->coders[i].id);
		}
		pthread_mutex_unlock(&simulation->coders[i].field_mutex);
		i++;
	}
	return (-1);
}

static void	handle_burnout(t_sim *simulation, int id)
{
	set_stopped(simulation, id);
	notify_all_dongles(simulation);
	log_msg(simulation, id, "burned out");
}
