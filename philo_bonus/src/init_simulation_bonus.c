/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_simulation_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 09:05:04 by zjamali           #+#    #+#             */
/*   Updated: 2021/09/24 12:26:44 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosopher_bonus.h"

void	init_simulation_semaphores(t_simulation *simulation)
{
	sem_unlink("forks");
	sem_unlink("main");
	sem_unlink("message");
	sem_unlink("is_eating");
	sem_unlink("eating_times");
	simulation->forks = sem_open("forks", O_CREAT, 644,
			simulation->number_of_philos);
	simulation->main_lock = sem_open("main", O_CREAT, 644, 1);
	sem_wait(simulation->main_lock);
	simulation->start_time = get_current_time();
	simulation->message = sem_open("message", O_CREAT, 644, 1);
	if (simulation->is_times_to_eat)
	{
		simulation->eating_times_count = sem_open("eating_times",
				O_CREAT, 644, 0);
	}
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
	init_simulation_semaphores(simulation);
	simulation->pid = (int *)malloc(sizeof(int) * simulation->number_of_philos);
	if (!simulation->pid)
		return (NULL);
	while (i < simulation->number_of_philos)
	{
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
