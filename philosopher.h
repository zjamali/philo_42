/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 17:54:01 by zjamali           #+#    #+#             */
/*   Updated: 2021/09/18 17:55:10 by zjamali          ###   ########.fr       */
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
	int				is_live;

	int				is_times_to_eat;
	int				times_to_eat;

	long			limit;
	t_simulation	*simulation;
	pthread_mutex_t	is_eating;
}	t_philo;

struct s_simulation
{
	int				number_of_philos;
	long			start_time;

	//t_philo			*philo;
	int				time_to_die;
	int				time_to_eat;
	int				eating_times;
	int				time_to_sleep;
	int				is_times_to_eat;

	//pthread_mutex_t	forks[NUMBER_OF_PHILOS];
	pthread_mutex_t	*forks;
	pthread_mutex_t	message;
	pthread_mutex_t	main_lock;
};

/*
* printing function 
*/
int		ft_atoi(const char *str);

#endif