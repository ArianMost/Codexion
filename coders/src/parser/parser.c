/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amostash <amostash@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 18:58:07 by amostash          #+#    #+#             */
/*   Updated: 2026/07/22 18:58:10 by amostash         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


static int	parse_number(const char *s, long long *res)
{
	long long	number;
	int			i;

	i = 0;
	number = 0;

	if (s == NULL || s[0] == '\0')
		return (0);

	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		if (number > (LLONG_MAX - (s[i] - '0')) / 10)
			return (0);
		number = number * 10 + (s[i] - '0');
		i++;
	}
	*res = number;

	return (1);
}


int	parse_args(int argc, char **argv, t_config *config)
{
	long long	number_of_coders;

	if (argc != 9)
	{
		fprintf(stderr, "Right usage: %s number_of_coders time_to_burnout "
			"time_to_compile time_to_debug time_to_refactor "
			"number_of_compiles_required dongle_cooldown scheduler\n",
			argv[0]);
		return (0);
	}

	if (!parse_number(argv[1], &number_of_coders) || number_of_coders < 1)
		return (fprintf(stderr, "invalid number_of_coders\n"), 0);

	config->number_of_coders = (int)number_of_coders;
	if (!parse_number(argv[2], &config->time_to_burnout) || config->time_to_burnout < 1)
		return (fprintf(stderr, "invalid time_to_burnout\n"), 0);
	if (!parse_number(argv[3], &config->time_to_compile))
		return (fprintf(stderr, "invalid time_to_compile\n"), 0);
	if (!parse_number(argv[4], &config->time_to_debug))
		return (fprintf(stderr, "invalid time_to_debug\n"), 0);
	if (!parse_number(argv[5], &config->time_to_refactor))
		return (fprintf(stderr, "invalid time_to_refactor\n"), 0);
	if (!parse_number(argv[6], &config->required))
		return (fprintf(stderr, "invalid number_of_compiles_required\n"), 0);
	if (!parse_number(argv[7], &config->dongle_cooldown))
		return (fprintf(stderr, "invalid dongle_cooldown\n"), 0);
	if (!parse_scheduler(argv[8], &config->scheduler))
		return (fprintf(stderr, "scheduler must be 'fifo' or 'edf'\n"), 0);

	return (1);
}
