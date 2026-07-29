/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amostash <amostash@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 14:25:14 by amostash          #+#    #+#             */
/*   Updated: 2026/07/29 17:59:16 by amostash         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>
# include <limits.h>

# define SCHED_FIFO_MODE 0
# define SCHED_EDF_MODE  1

typedef struct s_sim	t_sim;
typedef struct s_dongle	t_dongle;

typedef struct s_config
{
	int			number_of_coders;
	long long	time_to_burnout;
	long long	time_to_compile;
	long long	time_to_debug;
	long long	time_to_refactor;
	long long	required;
	long long	dongle_cooldown;
	int			scheduler;
}	t_config;

typedef struct s_heap_node
{
	int			coder_id;
	long long	key_sort;
	long long	proirity_seq;
}	t_heap_node;

typedef struct s_heap
{
	t_heap_node	*requests;
	int			used_capacity;
	int			capacity_allocated;
}	t_heap;

typedef struct s_dongle
{
	int				id;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				taken;
	long long		available_at;
	long long		queue_counter;
	t_heap			queue;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	pthread_mutex_t	field_mutex;
	long long		last_compile_start;
	int				compiles_done;
	int				burned_out;
	t_dongle		*left;
	t_dongle		*right;
	t_sim			*sim;
}	t_coder;

typedef struct s_sim
{
	t_config		cfg;
	t_dongle		*dongles;
	t_coder			*coders;
	struct timeval	start_tv;
	pthread_mutex_t	log_mutex;
	pthread_mutex_t	stop_mutex;
	int				stopped;
	int				burnout_id;
	pthread_t		monitor_thread;
}	t_sim;

long long	elapsed_ms(t_sim *simulation);
int			parse_args(int argc, char **argv, t_config *config);
void		set_stopped(t_sim *simulation, int burnout_id);
int			is_stopped(t_sim *simulation);
void		log_msg(t_sim *simulation, int coder_id, const char *msg);
void		*coder_lifecycle(void *arg);
void		set_last_compile_begin(t_coder *coder, long long value);
void		release_both_dongles(t_coder *coder, t_sim	*simulation);
int			stop_condition(t_dongle *dongle, int coder_id);
int			can_pick_dongle(t_dongle *d, long long ct, int c_id);
int			choose_best_dongle(t_dongle *d, t_sim *sim, int c_id);
int			pick_dongle(t_dongle *d, t_sim *sim, int c_id, long long dl);
int			pick_both_dongles(t_coder *coder);
void		bubble_up(t_heap *queue, int i);
void		bubble_down(t_heap *queue, int i);
void		grow_heap(t_heap *queue);
void		heap_push(t_heap *q, int c_id, long long key, long long seq);
void		heap_clean(t_heap *heap);
void		remove_coder_heap(t_heap *h, int coder_id);
int			heap_peek_min(t_heap *queue, t_heap_node *out);
void		heap_pop_min(t_heap *queue);
void		release_dongle(t_dongle *dongle, t_sim *simulation);
void		sleep_ms_until_stop(t_sim *simulation, long long time_ms);
void		get_absolute_ts(t_sim *sim, long long t_ms, struct timespec *ts);
int			all_compiled_successfully(t_sim *simulation);
void		notify_all_dongles(t_sim *simulation);
int			check_burnout(t_sim *simulation, long long current_time);
void		handle_burnout(t_sim *simulation, int id);
void		*monitor_simulation(void *arg);
void		init_coders(t_sim *simulation, int coders_count);
void		init_dongles(t_sim *simulation, int coders_count);
int			init_simulation(t_sim *simulation, t_config *config);
void		cleanup_simulation(t_sim *simulation);

#endif
