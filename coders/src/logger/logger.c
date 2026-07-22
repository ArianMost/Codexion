/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amostash <amostash@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 18:55:32 by amostash          #+#    #+#             */
/*   Updated: 2026/07/22 18:55:34 by amostash         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


void	log_msg(t_sim *simulation, int coder_id, const char *msg)
{
	long long	ts;

	pthread_mutex_lock(&simulation->log_mutex);
	ts = elapsed_ms(simulation);

	printf("%lld %d %s\n", ts, coder_id, msg);

	pthread_mutex_unlock(&simulation->log_mutex);
}
