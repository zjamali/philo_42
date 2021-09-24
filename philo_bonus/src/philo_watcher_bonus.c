/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_watcher_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 08:53:25 by zjamali           #+#    #+#             */
/*   Updated: 2021/09/24 11:06:12 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosopher_bonus.h"

void	*watch_eating_times(void *simulation_data)
{
	t_simulation	*simulation;
	int				counter;

	counter = 0;
	simulation = simulation_data;
	while (1)
	{
		sem_wait(simulation->eating_times_count);
		counter++;
		if (counter == simulation->number_of_philos)
			sem_post(simulation->main_lock);
		usleep(500);
	}
	return (NULL);
}

void	*watch_philo_routine(void *philo_data)
{
	t_philo			*philo;
	t_simulation	*simulation;

	philo = philo_data;
	simulation = philo->simulation;
	while (1)
	{
		sem_wait(philo->is_eating);
		if (philo->limit < get_current_time())
		{
			print_to_terminal("\t\033[0;31mdied \033[0m\n", simulation,
				philo->philo_id, 1);
		}
		sem_post(philo->is_eating);
		usleep(500);
	}
	return (NULL);
}
