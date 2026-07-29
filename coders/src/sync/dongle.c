/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amostash <amostash@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 16:49:04 by amostash          #+#    #+#             */
/*   Updated: 2026/07/27 17:23:25 by amostash         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	pick_dongle(t_dongle *dongle,
		t_sim *simulation,
		int coder_id,
		long long deadline)
{
	long long		ticket;
	long long		current_time;
	long long		wait_until;
	struct timespec	ts;

	pthread_mutex_lock(&dongle->mutex);
	ticket = dongle->queue_counter++;
	heap_push(&dongle->queue, coder_id, deadline, ticket);
	while (1)
	{
		if (is_stopped(simulation))
			return (stop_condition(dongle, coder_id));
		current_time = elapsed_ms(simulation);
		if (can_pick_dongle(dongle, current_time, coder_id))
			return (choose_best_dongle(dongle, simulation, coder_id));
		wait_until = dongle->available_at;
		if (wait_until <= current_time)
			wait_until = current_time + 50;
		get_absolute_ts(simulation, wait_until, &ts);
		pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
	}
}

void	release_dongle(t_dongle *dongle, t_sim *simulation)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->taken = 0;
	dongle->available_at = elapsed_ms(simulation)
		+ simulation->cfg.dongle_cooldown;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}

void	notify_all_dongles(t_sim *simulation)
{
	t_dongle	*dongle;
	int			i;

	i = 0;
	while (i < simulation->cfg.number_of_coders)
	{
		dongle = &simulation->dongles[i];
		pthread_mutex_lock(&dongle->mutex);
		pthread_cond_broadcast(&dongle->cond);
		pthread_mutex_unlock(&dongle->mutex);
		i++;
	}
}
