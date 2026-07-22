/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amostash <amostash@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 14:41:50 by amostash          #+#    #+#             */
/*   Updated: 2026/07/22 18:53:51 by amostash         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	handle_completion(t_sim *simulation)
{
	set_stopped(simulation, -1);
	notify_all_dongles(simulation);
}

void	*monitor_simulation(void *arg)
{
	t_sim		*simulation;
	long long	current_time;
	int			burnedout;

	simulation = (t_sim *)arg;
	while (!is_stopped(simulation))
	{
		current_time = elapsed_ms(simulation);
		burnedout = check_burnout(simulation, current_time);
		if (burnedout != -1)
		{
			handle_burnout(simulation, burnedout);
			break ;
		}
		if (all_compiled_successfully(simulation))
		{
			handle_completion(simulation);
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}

