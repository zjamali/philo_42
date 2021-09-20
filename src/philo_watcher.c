/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_watcher.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 08:53:25 by zjamali           #+#    #+#             */
/*   Updated: 2021/09/20 15:40:44 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosopher.h"

void	*watch_philo_routine(void *philo_data)
{
	t_philo			*philo;
	t_simulation	*simulation;

	philo = philo_data;
	simulation = philo->simulation;
	while (1)
	{
		if (simulation->is_times_to_eat)
			if (simulation->eating_times_for_all_philos == 0)
				pthread_mutex_unlock(&simulation->main_lock);
		if (philo->limit < get_current_time())
		{
			print_to_terminal("\033[0;31m DIE \033[0m\n", simulation,
				philo->philo_id, 1);
		}
		usleep(500);
	}
	return (NULL);
}