/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amostash <amostash@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 14:58:06 by amostash          #+#    #+#             */
/*   Updated: 2026/07/27 18:49:04 by amostash         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	elapsed_ms(t_sim *simulation)
{
	struct timeval	current_time;
	long long		sec_diff;
	long long		usec_diff;

	gettimeofday(&current_time, NULL);
	sec_diff = (long long)(current_time.tv_sec
			- simulation->start_tv.tv_sec);
	usec_diff = (long long)(current_time.tv_usec
			- simulation->start_tv.tv_usec);
	return (sec_diff * 1000 + usec_diff / 1000);
}

void	set_stopped(t_sim *simulation, int burnout_id)
{
	pthread_mutex_lock(&simulation->stop_mutex);
	if (!simulation->stopped)
	{
		simulation->stopped = 1;
		simulation->burnout_id = burnout_id;
	}
	pthread_mutex_unlock(&simulation->stop_mutex);
}

int	is_stopped(t_sim *simulation)
{
	int	val;

	pthread_mutex_lock(&simulation->stop_mutex);
	val = simulation->stopped;
	pthread_mutex_unlock(&simulation->stop_mutex);
	return (val);
}

void	sleep_ms_until_stop(t_sim *simulation, long long time_ms)
{
	while (time_ms > 2 && !is_stopped(simulation))
	{
		usleep(2000);
		time_ms -= 2;
	}
	if (!is_stopped(simulation) && time_ms > 0)
		usleep(time_ms * 1000);
}

int	all_compiled_successfully(t_sim *simulation)
{
	int	done;
	int	i;

	i = 0;
	done = 1;
	while (i < simulation->cfg.number_of_coders && done)
	{
		pthread_mutex_lock(&simulation->coders[i].field_mutex);
		if (simulation->coders[i].compiles_done < simulation->cfg.required)
			done = 0;
		pthread_mutex_unlock(&simulation->coders[i].field_mutex);
		i++;
	}
	return (done);
}
