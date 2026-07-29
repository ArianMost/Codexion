/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_dongle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amostash <amostash@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 12:39:00 by amostash          #+#    #+#             */
/*   Updated: 2026/07/27 16:51:18 by amostash         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	heap_init(t_heap *queue)
{
	queue->requests = NULL;
	queue->capacity_allocated = 0;
	queue->used_capacity = 0;
}

static void	init_dongle(t_dongle *dongle, int id)
{
	dongle->id = id;
	pthread_mutex_init(&dongle->mutex, NULL);
	pthread_cond_init(&dongle->cond, NULL);
	dongle->taken = 0;
	dongle->available_at = 0;
	dongle->queue_counter = 0;
	heap_init(&dongle->queue);
}

void	init_dongles(t_sim *simulation, int coders_count)
{
	int	i;

	i = 0;
	while (i < coders_count)
	{
		init_dongle(&simulation->dongles[i], i);
		i++;
	}
}
