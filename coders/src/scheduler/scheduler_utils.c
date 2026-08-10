/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amostash <amostash@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 19:18:55 by amostash          #+#    #+#             */
/*   Updated: 2026/08/01 16:31:51 by amostash         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	higher_priority(t_heap_node *a, t_heap_node *b, int scheduler)
{
	if (a->key_sort != b->key_sort)
		return (b->key_sort > a->key_sort);
	if (scheduler == SCHED_EDF_MODE)
		return (a->coder_id > b->coder_id);
	return (b->proirity_seq > a->proirity_seq);
}

static void	swap_nodes(t_heap_node *a, t_heap_node *b)
{
	t_heap_node	tmp;

	tmp = *b;
	*b = *a;
	*a = tmp;
}

void	bubble_up(t_heap *queue, int i)
{
	int	parent;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (!higher_priority(&queue->requests[i], &queue->requests[parent], queue->scheduler))
			break ;
		swap_nodes(&queue->requests[i], &queue->requests[parent]);
		i = parent;
	}
}

void	bubble_down(t_heap *queue, int i)
{
	int	left;
	int	right;
	int	smallest;

	while (1)
	{
		smallest = i;
		left = 2 * i + 1;
		right = 2 * i + 2;
		if (left < queue->used_capacity
			&& higher_priority(&queue->requests[left],
				&queue->requests[smallest], queue->scheduler))
			smallest = left;
		if (right < queue->used_capacity
			&& higher_priority(&queue->requests[right],
				&queue->requests[smallest], queue->scheduler))
			smallest = right;
		if (smallest == i)
			break ;
		swap_nodes(&queue->requests[i], &queue->requests[smallest]);
		i = smallest;
	}
}

void	grow_heap(t_heap *queue)
{
	t_heap_node	*new_request;
	int			new_cap;
	int			i;

	if (queue->used_capacity < queue->capacity_allocated)
		return ;
	if (queue->capacity_allocated == 0)
		new_cap = 4;
	else
		new_cap = queue->capacity_allocated * 2;
	new_request = malloc(sizeof(t_heap_node) * (size_t)new_cap);
	i = 0;
	while (i < queue->used_capacity)
	{
		new_request[i] = queue->requests[i];
		i++;
	}
	free(queue->requests);
	queue->requests = new_request;
	queue->capacity_allocated = new_cap;
}
