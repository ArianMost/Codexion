/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amostash <amostash@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 19:25:57 by amostash          #+#    #+#             */
/*   Updated: 2026/07/27 17:22:06 by amostash         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	compile(t_coder *coder)
{
	t_sim	*simulation;

	simulation = coder->sim;
	if (pick_both_dongles(coder))
		return (1);
	if (is_stopped(simulation))
	{
		release_both_dongles(coder, simulation);
		return (1);
	}
	set_last_compile_begin(coder, elapsed_ms(simulation));
	log_msg(simulation, coder->id, "is compiling");
	sleep_ms_until_stop(simulation, simulation->cfg.time_to_compile);
	release_both_dongles(coder, simulation);
	return (is_stopped(simulation));
}

static int	finish_compile(t_coder *coder)
{
	t_sim	*simulation;

	simulation = coder->sim;
	pthread_mutex_lock(&coder->field_mutex);
	coder->compiles_done++;
	pthread_mutex_unlock(&coder->field_mutex);
	if (all_compiled_successfully(simulation))
	{
		set_stopped(simulation, -1);
		notify_all_dongles(simulation);
		return (1);
	}
	return (0);
}

static int	rest(t_coder *coder)
{
	t_sim	*simulation;

	simulation = coder->sim;
	log_msg(simulation, coder->id, "is debugging");
	sleep_ms_until_stop(simulation, simulation->cfg.time_to_debug);
	if (is_stopped(simulation))
		return (1);
	log_msg(simulation, coder->id, "is refactoring");
	sleep_ms_until_stop(simulation, simulation->cfg.time_to_refactor);
	return (is_stopped(simulation));
}

void	*coder_lifecycle(void *arg)
{
	t_coder	*coder;

	coder = arg;
	set_last_compile_begin(coder, elapsed_ms(coder->sim));
	while (!is_stopped(coder->sim))
	{
		if (compile(coder))
			break ;
		if (finish_compile(coder))
			break ;
		if (rest(coder))
			break ;
	}
	return (NULL);
}
