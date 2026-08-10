/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amostash <amostash@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 19:27:37 by amostash          #+#    #+#             */
/*   Updated: 2026/08/01 18:45:13 by amostash         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	set_last_compile_begin(t_coder *coder, long long value)
{
	pthread_mutex_lock(&coder->field_mutex);
	coder->last_compile_start = value;
	pthread_mutex_unlock(&coder->field_mutex);
}

static long long	get_last_compile_begin(t_coder *coder)
{
	long long	value;

	pthread_mutex_lock(&coder->field_mutex);
	value = coder->last_compile_start;
	pthread_mutex_unlock(&coder->field_mutex);
	return (value);
}

static long long	compute_deadline(t_coder *coder)
{
	if (coder->sim->cfg.scheduler == SCHED_EDF_MODE)
		return (get_last_compile_begin(coder)
			+ coder->sim->cfg.time_to_burnout);
	return (0);
}

int	pick_both_dongles(t_coder *coder)
{
	t_dongle	*low_id;
	t_dongle	*high_id;
	long long	deadline;

	if (coder->right->id == coder->left->id)
		return (pick_dongle(coder->left, coder->sim,
				coder->id, compute_deadline(coder)));
	if (coder->right->id > coder->left->id)
	{
		high_id = coder->right;
		low_id = coder->left;
	}
	else
	{
		low_id = coder->right;
		high_id = coder->left;
	}
	deadline = compute_deadline(coder);
	if (pick_dongle(low_id, coder->sim, coder->id, deadline))
		return (1);
	deadline = compute_deadline(coder);
	if (pick_dongle(high_id, coder->sim, coder->id, deadline))
		return (release_dongle(low_id, coder->sim), 1);
	return (0);
}

void	release_both_dongles(t_coder *coder, t_sim	*simulation)
{
	release_dongle(coder->left, simulation);
	if (coder->right->id != coder->left->id)
		release_dongle(coder->right, simulation);
}
