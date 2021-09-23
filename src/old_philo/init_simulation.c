/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_simulation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 09:05:04 by zjamali           #+#    #+#             */
/*   Updated: 2021/09/21 11:24:54 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosopher.h"

void	init_simulation_mutex(t_simulation *simulation)
{
	pthread_mutex_init(&simulation->main_lock, NULL);
	pthread_mutex_lock(&simulation->main_lock);
	simulation->start_time = get_current_time();
	pthread_mutex_init(&simulation->message, NULL);
}

t_philo	*init_simaulation_philos(t_simulation *simulation)
{
	t_philo			*philos_data;
	int				i;

	i = 0;
	philos_data = (t_philo *)malloc(sizeof(t_philo)
			* simulation->number_of_philos);
	if (!philos_data)
		return (NULL);
	init_simulation_mutex(simulation);
	while (i < simulation->number_of_philos)
	{
		pthread_mutex_init(&simulation->forks[i], NULL);
		pthread_mutex_init(&philos_data[i].is_eating, NULL);
		philos_data[i].philo_id = i + 1;
		philos_data[i].time_to_die = simulation->time_to_die;
		philos_data[i].time_to_eat = simulation->time_to_eat;
		philos_data[i].time_to_sleep = simulation->time_to_sleep;
		philos_data[i].is_times_to_eat = simulation->is_times_to_eat;
		philos_data[i].eating_times = simulation->eating_times;
		philos_data[i].simulation = simulation;
		i++;
	}
	return (philos_data);
}
