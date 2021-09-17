/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/13 14:12:21 by zjamali           #+#    #+#             */
/*   Updated: 2021/09/17 20:01:46 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

long	get_current_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

void	print_to_terminal(char *output, t_simulation *simulation, int philo_id,
					   int is_philo_die)
{
	pthread_mutex_lock(&simulation->message);
	if (is_philo_die)
	{
		ft_putnbr_fd(get_current_time() - simulation->start_time, 1);
		ft_putstr_fd("\t", 1);
		ft_putnbr_fd(philo_id, 1);
		ft_putstr_fd(output, 1);
		pthread_mutex_unlock(&simulation->main_lock);
	}
	else
	{
		ft_putnbr_fd(get_current_time() - simulation->start_time, 1);
		ft_putstr_fd("\t", 1);
		ft_putnbr_fd(philo_id, 1);
		ft_putstr_fd(output, 1);
		pthread_mutex_unlock(&simulation->message);
	}
}

void	philo_taken_forks(t_philo *current_philo)
{
	t_philo			*philo;
	t_simulation	*simulation;

	philo = current_philo;
	simulation = philo->simulation;
	pthread_mutex_lock(&simulation->forks[philo->philo_id - 1]);
	print_to_terminal("\t taken fork\n", simulation, philo->philo_id, 0);
	pthread_mutex_lock(
		&simulation->forks[philo->philo_id % NUMBER_OF_PHILOS]);
	print_to_terminal("\t taken fork\n", simulation, philo->philo_id, 0);
}

void	philo_is_eating(t_philo *current_philo)
{
	t_philo			*philo;
	t_simulation	*simulation;

	philo = current_philo;
	simulation = philo->simulation;
	pthread_mutex_lock(&philo->is_eating);
	philo->limit = get_current_time() + philo->time_to_die;
	print_to_terminal("\t\033[0;32m EATING \033[0m\n", simulation,
		philo->philo_id, 0);
	usleep(philo->time_to_eat * 1000);
	pthread_mutex_unlock(&philo->is_eating);
}

void	philo_start_sleeping(t_philo *current_philo)
{
	t_philo			*philo;
	t_simulation	*simulation;

	philo = current_philo;
	simulation = philo->simulation;
	pthread_mutex_unlock(&simulation->forks[philo->philo_id - 1]);
	pthread_mutex_unlock(
		&simulation->forks[philo->philo_id % NUMBER_OF_PHILOS]);
	print_to_terminal("\t\033[0;34m SLEEPING \033[0m\n", simulation,
		philo->philo_id, 0);
	usleep(philo->time_to_sleep * 1000);
}

void	*watch_philo_routine(void *philo_data)
{
	t_philo			*philo;
	t_simulation	*simulation;

	philo = philo_data;
	simulation = philo->simulation;
	while (philo->is_live)
	{
		pthread_mutex_lock(&philo->is_eating);
		if (philo->limit < get_current_time())
		{
			philo->is_live = 0;
			print_to_terminal("\033[0;31m DIE \033[0m\n", simulation,
				philo->philo_id, 1);
		}
		pthread_mutex_unlock(&philo->is_eating);
		usleep(30);
	}
	return (NULL);
}

void	philo_start_thinking(t_philo *current_philo)
{
	t_philo			*philo;
	t_simulation	*simulation;

	philo = current_philo;
	simulation = philo->simulation;
	print_to_terminal("\t\033[0;33m THINKING \033[0m\n", simulation,
		philo->philo_id, 0);
}

void	*philo_routine(void *philo_data)
{
	pthread_t		philo_watcher;
	t_philo			*philo;
	t_simulation	*simulation;

	philo = philo_data;
	simulation = philo->simulation;
	if (philo)
	{
		pthread_create(&philo_watcher, NULL, watch_philo_routine, philo);
		pthread_detach(philo_watcher);
	}
	while (philo->is_live && philo->eating_times)
	{
		philo_taken_forks(philo);
		philo_is_eating(philo);
		if (philo->is_times_to_eat)
			philo->eating_times--;
		else
			philo->eating_times = 1;
		philo_start_sleeping(philo);
		philo_start_thinking(philo);
	}
	return (NULL);
}

int	main(void)
{
	pthread_t		philos[NUMBER_OF_PHILOS];
	t_philo			philos_data[NUMBER_OF_PHILOS];
	int				i;
	t_simulation	simulation;

	pthread_mutex_init(&simulation.message, NULL);
	simulation.start_time = get_current_time();
	simulation.philo = philos_data;
	pthread_mutex_init(&simulation.main_lock, NULL);
	pthread_mutex_lock(&simulation.main_lock);
	i = 0;
	while (i < NUMBER_OF_PHILOS)
	{
		pthread_mutex_init(&simulation.forks[i], NULL);
		philos_data[i].philo_id = i + 1;
		philos_data[i].is_live = 1;
		philos_data[i].time_to_die = TIME_TO_DIE;
		philos_data[i].time_to_eat = TIME_TO_EAT;
		philos_data[i].time_to_sleep = TIME_TO_SLEEP;
		philos_data[i].is_times_to_eat = 0;
		philos_data[i].eating_times = TIMES_TO_EAT;
		pthread_mutex_init(&philos_data[i].is_eating, NULL);
		philos_data[i].limit = get_current_time() + philos_data[i].time_to_die;
		philos_data[i].simulation = &simulation;
		pthread_create(&philos[i], NULL, philo_routine, &philos_data[i]);
		pthread_detach(philos[i]);
		usleep(10);
		i++;
	}
	pthread_mutex_lock(&simulation.main_lock);
	pthread_mutex_unlock(&simulation.main_lock);
	i = 0;
	while (i < NUMBER_OF_PHILOS)
	{
		pthread_mutex_destroy(&simulation.forks[i]);
		i++;
	}
	pthread_mutex_destroy(&simulation.main_lock);
	pthread_mutex_destroy(&simulation.message);
	return (0);
}
