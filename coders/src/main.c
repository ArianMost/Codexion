/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amostash <amostash@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 18:39:20 by amostash          #+#    #+#             */
/*   Updated: 2026/07/29 17:59:24 by amostash         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	join_coders(t_sim *sim, int created)
{
	while (created > 0)
	{
		created--;
		pthread_join(sim->coders[created].thread, NULL);
	}
}

static int	thread_error(t_sim *sim, int created, char *msg)
{
	fprintf(stderr, "%s\n", msg);
	set_stopped(sim, -1);
	join_coders(sim, created);
	return (0);
}

static int	create_threads(t_sim *sim)
{
	int	i;
	int	created;

	i = 0;
	created = 0;
	while (i < sim->cfg.number_of_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL,
				coder_lifecycle, &sim->coders[i]) != 0)
			return (thread_error(sim, created,
					"codexion: pthread_create failed"));
		created++;
		i++;
	}
	if (pthread_create(&sim->monitor_thread, NULL,
			monitor_simulation, sim) != 0)
		return (thread_error(sim, created,
				"codexion: pthread_create (monitor) failed"));
	return (1);
}

static void	join_all_threads(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->cfg.number_of_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	pthread_join(sim->monitor_thread, NULL);
}

int	main(int argc, char **argv)
{
	t_config	cfg;
	t_sim		sim;

	if (!parse_args(argc, argv, &cfg))
		return (1);
	if (!init_simulation(&sim, &cfg))
	{
		fprintf(stderr, "codexion: allocation failed\n");
		return (1);
	}
	if (!create_threads(&sim))
	{
		cleanup_simulation(&sim);
		return (1);
	}
	join_all_threads(&sim);
	cleanup_simulation(&sim);
	return (0);
}
