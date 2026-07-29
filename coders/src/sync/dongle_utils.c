/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amostash <amostash@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 19:47:32 by amostash          #+#    #+#             */
/*   Updated: 2026/07/27 17:24:11 by amostash         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	stop_condition(t_dongle *dongle, int coder_id)
{
	remove_coder_heap(&dongle->queue, coder_id);
	pthread_mutex_unlock(&dongle->mutex);
	return (1);
}

int	can_pick_dongle(t_dongle *dongle,
				long long current_time,
				int coder_id)
{
	t_heap_node	top;

	if (!heap_peek_min(&dongle->queue, &top))
		return (0);
	heap_peek_min(&dongle->queue, &top);
	return (!dongle->taken
		&& current_time >= dongle->available_at
		&& top.coder_id == coder_id);
}

int	choose_best_dongle(t_dongle *dongle,
				t_sim *simulation,
				int coder_id)
{
	heap_pop_min(&dongle->queue);
	dongle->taken = 1;
	pthread_mutex_unlock(&dongle->mutex);
	log_msg(simulation, coder_id, "has taken a dongle");
	return (0);
}

void	get_absolute_ts(t_sim *simulation,
				long long target_ms,
				struct timespec *ts)
{
	long long		sec;
	long long		nano_sec;

	sec = simulation->start_tv.tv_sec + target_ms / 1000;
	nano_sec = (long long)simulation->start_tv.tv_usec * 1000
		+ (target_ms % 1000) * 1000000;
	sec += nano_sec / 1000000000;
	nano_sec = nano_sec % 1000000000;
	ts->tv_sec = (time_t)sec;
	ts->tv_nsec = (long)nano_sec;
}
