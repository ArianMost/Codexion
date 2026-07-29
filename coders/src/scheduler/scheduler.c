/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amostash <amostash@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 14:59:59 by amostash          #+#    #+#             */
/*   Updated: 2026/07/27 16:51:33 by amostash         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	heap_push(t_heap *queue, int coder_id, long long key, long long seq)
{
	grow_heap(queue);
	queue->requests[queue->used_capacity].coder_id = coder_id;
	queue->requests[queue->used_capacity].key_sort = key;
	queue->requests[queue->used_capacity].proirity_seq = seq;
	queue->used_capacity++;
	bubble_up(queue, queue->used_capacity - 1);
}

void	remove_coder_heap(t_heap *queue, int coder_id)
{
	int	i;

	i = 0;
	while (i < queue->used_capacity)
	{
		if (queue->requests[i].coder_id == coder_id)
		{
			queue->requests[i] = queue->requests[queue->used_capacity - 1];
			queue->used_capacity--;
			bubble_down(queue, i);
			bubble_up(queue, i);
			return ;
		}
		i++;
	}
}

int	heap_peek_min(t_heap *queue, t_heap_node *out)
{
	if (queue->used_capacity == 0)
		return (0);
	*out = queue->requests[0];
	return (1);
}

void	heap_pop_min(t_heap *queue)
{
	if (queue->used_capacity == 0)
		return ;
	queue->requests[0] = queue->requests[queue->used_capacity - 1];
	queue->used_capacity--;
	if (queue->used_capacity > 0)
		bubble_down(queue, 0);
}

void	heap_clean(t_heap *heap)
{
	free(heap->requests);
	heap->requests = NULL;
	heap->used_capacity = 0;
	heap->capacity_allocated = 0;
}
