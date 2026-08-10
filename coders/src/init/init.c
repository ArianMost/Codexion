#include "codexion.h"

static int	allocate_simulation(t_sim *simulation, int coders_count)
{
	simulation->dongles = malloc(sizeof(t_dongle) * (size_t)coders_count);
	simulation->coders = malloc(sizeof(t_coder) * (size_t)coders_count);
	if (!simulation->dongles || !simulation->coders)
	{
		free(simulation->dongles);
		free(simulation->coders);
		return (0);
	}
	return (1);
}

static int	init_simulation_state(t_sim *simulation, t_config *config)
{
	simulation->cfg = *config;
	simulation->stopped = 0;
	simulation->burnout_id = -1;
	gettimeofday(&simulation->start_tv, NULL);
	if (pthread_mutex_init(&simulation->log_mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&simulation->stop_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&simulation->log_mutex);
		return (0);
	}
	return (1);
}

int	init_simulation(t_sim *simulation, t_config *config)
{
	int	count;

	count = config->number_of_coders;
	if (!allocate_simulation(simulation, count))
		return (0);
	if (!init_simulation_state(simulation, config))
	{
		free(simulation->dongles);
		free(simulation->coders);
		return (0);
	}
	init_dongles(simulation, count);
	init_coders(simulation, count);
	return (1);
}

void	cleanup_simulation(t_sim *simulation)
{
	int	i;

	i = 0;
	while (i < simulation->cfg.number_of_coders)
	{
		pthread_mutex_destroy(&simulation->dongles[i].mutex);
		pthread_cond_destroy(&simulation->dongles[i].cond);
		heap_clean(&simulation->dongles[i].queue);
		i++;
	}
	i = 0;
	while (i < simulation->cfg.number_of_coders)
	{
		pthread_mutex_destroy(&simulation->coders[i].field_mutex);
		i++;
	}
	pthread_mutex_destroy(&simulation->stop_mutex);
	pthread_mutex_destroy(&simulation->log_mutex);
	free(simulation->dongles);
	free(simulation->coders);
}
