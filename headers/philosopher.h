/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 17:54:01 by zjamali           #+#    #+#             */
/*   Updated: 2021/09/21 11:29:04 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H
# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# include <limits.h>
# include <stdlib.h>

typedef struct s_simulation	t_simulation;

typedef struct s_philo_data
{
	int				philo_id;
	int				time_to_sleep;
	int				time_to_die;
	int				time_to_eat;
	int				eating_times;

	int				is_times_to_eat;
	int				times_to_eat;

	long			limit;
	t_simulation	*simulation;
	pthread_mutex_t	is_eating;
}	t_philo;

struct s_simulation
{
	int				number_of_philos;
	int				eating_times_for_all_philos;
	long			start_time;

	int				time_to_die;
	int				time_to_eat;
	int				eating_times;
	int				time_to_sleep;
	int				is_times_to_eat;

	pthread_mutex_t	*forks;
	pthread_mutex_t	message;
	pthread_mutex_t	main_lock;
};

/*
* libft function 
*/
int				ft_atoi(const char *str);

/*
* parse functions
*/
int				check_args(int ac, char **av);
t_simulation	*ft_parse_args(int ac, char **av);

/*
* philo routine
*/
void			*watch_philo_routine(void *philo_data);

/*
*	phlo routine 
*/
void			*philo_routine(void *philo_data);

/*
*
*/
long			get_current_time(void);

/*
*   print to terminal
*/
void			print_to_terminal(char *output, t_simulation *simulation,
					int philo_id, int is_philo_die);
/*
*	init simulation
*/
t_philo			*init_simaulation_philos(t_simulation *simulation);

/*
*
*/
int				handle_errors(void);
#endif