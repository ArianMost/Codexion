/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_coder.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amostash <amostash@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 12:40:25 by amostash          #+#    #+#             */
/*   Updated: 2026/07/31 18:25:53 by amostash         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	init_coder(t_coder *coder, int index, t_sim *simulation)
{
	int	n;

	n = simulation->cfg.number_of_coders;
	coder->id = index + 1;
	pthread_mutex_init(&coder->field_mutex, NULL);
	coder->last_compile_start = 0;
	coder->compiles_done = 0;
	coder->burned_out = 0;
	coder->left = &simulation->dongles[index];
	coder->right = &simulation->dongles[(index + n - 1) % n];
	coder->sim = simulation;
}

void	init_coders(t_sim *simulation, int coders_count)
{
	int	i;

	i = 0;
	while (i < coders_count)
	{
		init_coder(&simulation->coders[i], i, simulation);
		i++;
	}
}
