/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 17:54:01 by zjamali           #+#    #+#             */
/*   Updated: 2021/09/17 18:53:22 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H
# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# define NUMBER_OF_PHILOS 4
# define TIME_TO_DIE 410
# define TIME_TO_EAT 200
# define TIME_TO_SLEEP 200
# define TIMES_TO_EAT 10

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
	long			start_time;
	t_philo			*philo;
	pthread_mutex_t	forks[NUMBER_OF_PHILOS];
	pthread_mutex_t	message;
	pthread_mutex_t	main_lock;
};

/*
* printing function 
*/
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);

#endif