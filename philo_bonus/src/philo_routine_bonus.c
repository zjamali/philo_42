/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 08:52:24 by zjamali           #+#    #+#             */
/*   Updated: 2021/09/23 13:27:47 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosopher_bonus.h"

void	philo_taken_forks(t_philo *current_philo)
{
	t_philo			*philo;
	t_simulation	*simulation;

	philo = current_philo;
	simulation = philo->simulation;
	sem_wait(simulation->forks);
	print_to_terminal("\thas taken a fork\n", simulation, philo->philo_id, 0);
	sem_wait(simulation->forks);
	print_to_terminal("\thas taken a fork\n", simulation, philo->philo_id, 0);
}

void	philo_is_eating(t_philo *current_philo)
{
	t_philo			*philo;
	t_simulation	*simulation;

	philo = current_philo;
	simulation = philo->simulation;
	sem_wait(philo->is_eating);
	print_to_terminal("\tis\033[0;32m eating\033[0m\n", simulation,
		philo->philo_id, 0);
	if (simulation->is_times_to_eat)
		sem_post(simulation->eating_times_count);
	philo->limit = get_current_time() + philo->time_to_die;
	usleep(philo->time_to_eat * 1000);
	sem_post(philo->is_eating);
}

void	philo_start_sleeping(t_philo *current_philo)
{
	t_philo			*philo;
	t_simulation	*simulation;

	philo = current_philo;
	simulation = philo->simulation;
	sem_post(simulation->forks);
	sem_post(simulation->forks);
	print_to_terminal("\tis\e[0;35m sleeping\033[0m\n", simulation,
		philo->philo_id, 0);
	usleep(philo->time_to_sleep * 1000);
}

void	philo_start_thinking(t_philo *current_philo)
{
	t_philo			*philo;
	t_simulation	*simulation;

	philo = current_philo;
	simulation = philo->simulation;
	print_to_terminal("\tis\033[0;33m thinking\033[0m\n", simulation,
		philo->philo_id, 0);
}

void	*philo_routine(t_philo *philo_data)
{
	pthread_t		philo_watcher;
	t_philo			*philo;
	t_simulation	*simulation;

	philo = philo_data;
	simulation = philo->simulation;
	pthread_create(&philo_watcher, NULL, watch_philo_routine, philo_data);
	pthread_detach(philo_watcher);
	while (philo->eating_times)
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
