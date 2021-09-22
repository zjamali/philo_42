/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_watcher_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 08:53:25 by zjamali           #+#    #+#             */
/*   Updated: 2021/09/22 13:20:21 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/philosopher_bonus.h"

void	*watch_philo_routine(void *philo_data)
{
	t_philo			*philo;
	t_simulation	*simulation;

	philo = philo_data;
	simulation = philo->simulation;
	while (1)
	{
		sem_wait(philo->is_eating);
		if (simulation->is_times_to_eat)
			if (simulation->eating_times_for_all_philos == 0)
				sem_post(simulation->main_lock);
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