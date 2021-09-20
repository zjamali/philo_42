/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 08:47:01 by zjamali           #+#    #+#             */
/*   Updated: 2021/09/20 15:38:12 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosopher.h"

int	check_args(int ac, char **av)
{
	int	num;
	int	i;

	i = 1;
	num = 0;
	while (i < ac)
	{	
		num = ft_atoi(av[i]);
		if (num <= 0)
		{
			printf("error: number of philosopher or times is negative \n");
			return (0);
		}
		if (INT_MAX < num || num < INT_MIN)
		{
			printf("error: number of philosopher or times is more \
					than max or less than min  \n");
			return (0);
		}
		i++;
	}
	return (1);
}

t_simulation	*ft_parse_args(int ac, char **av)
{
	t_simulation	*simulation;

	simulation = (t_simulation *)malloc(sizeof(t_simulation));
	simulation->number_of_philos = ft_atoi(av[1]);
	simulation->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* simulation->number_of_philos);
	simulation->time_to_die = ft_atoi(av[2]);
	simulation->time_to_eat = ft_atoi(av[3]);
	simulation->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
	{
		simulation->is_times_to_eat = 1;
		simulation->eating_times = ft_atoi(av[5]);
		simulation->eating_times_for_all_philos = simulation->eating_times
			* simulation->number_of_philos;
	}
	else
	{
		simulation->is_times_to_eat = 0;
		simulation->eating_times = 1;
	}
	return (simulation);
}
