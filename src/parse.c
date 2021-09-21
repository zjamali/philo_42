/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 08:47:01 by zjamali           #+#    #+#             */
/*   Updated: 2021/09/21 16:34:30 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosopher.h"

void	parse_philosopher_eating_times(t_simulation **simulation, char *arg)
{
	(*simulation)->is_times_to_eat = 1;
	(*simulation)->eating_times = ft_atoi(arg);
	(*simulation)->eating_times_for_all_philos = (*simulation)->eating_times
		* (*simulation)->number_of_philos;
}

t_simulation	*ft_parse_args(int ac, char **av)
{
	t_simulation	*simulation;

	simulation = (t_simulation *)malloc(sizeof(t_simulation));
	if (!simulation)
		return (NULL);
	simulation->number_of_philos = ft_atoi(av[1]);
	simulation->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* simulation->number_of_philos);
	if (!simulation->forks)
		return (NULL);
	simulation->time_to_die = ft_atoi(av[2]);
	simulation->time_to_eat = ft_atoi(av[3]);
	simulation->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		parse_philosopher_eating_times(&simulation, av[5]);
	else
	{
		simulation->is_times_to_eat = 0;
		simulation->eating_times = 1;
	}
	return (simulation);
}
