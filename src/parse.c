/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 08:47:01 by zjamali           #+#    #+#             */
/*   Updated: 2021/09/21 15:15:48 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosopher.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_check_arguments(int ac, char	**av)
{
	int		i;
	char	*arg;

	i = 1;
	if (ac != 5 && ac != 6)
	{
		(void)av;
		printf("error: missing or many arguments\n");
		return (1);
	}
	while (i < ac)
	{
		arg = av[i];
		if (arg[0] == '-')
		{
			printf("error: arguments must be positive\n");
			return (1);
		}
		while (*arg)
		{
			if (ft_isdigit(*arg) == 0)
			{
				printf("error: non digit argument \n");
				return (1);
			}
			arg++;
		}
		i++;
	}
	return (0);
}

int	check_args(int ac, char **av)
{
	int	num;
	int	i;

	i = 1;
	num = 0;
	if (ft_check_arguments(ac, av))
		return (0);
	while (i < ac)
	{
		num = ft_atoi(av[i]);
		if (i == 1 && num > 200)
		{
			printf("error: number of philosopher must be less than 200\n");
			return (0);
		}
		if (num < 60 && i > 1 && i < 5)
		{
			printf("error: time_to_die or time_to_eat or");
			printf("time_to_sleep under 60 ms\n");
			return (0);
		}
		if (INT_MAX < num || num < INT_MIN)
		{
			printf("error: number of philosopher or times is more ");
			printf("than max or less than min\n");
			return (0);
		}
		i++;
	}
	if (ac == 6)
	{
		if (!ft_atoi(av[5]))
		{
			printf("error : eating times must be more than zero\n");
			return (0);
		}
	}
	return (1);
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
