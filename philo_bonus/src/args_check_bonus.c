/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_check_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 16:33:13 by zjamali           #+#    #+#             */
/*   Updated: 2021/09/23 10:28:49 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosopher_bonus.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_check_is_digit(char *argument)
{
	char	*arg;

	arg = argument;
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
	return (0);
}

int	ft_check_arguments(int ac, char	**av)
{
	int		i;

	i = 1;
	if (ac != 5 && ac != 6)
	{
		(void)av;
		printf("error: missing or many arguments\n");
		return (1);
	}
	while (i < ac)
	{
		if (ft_check_is_digit(av[i]))
			return (1);
		i++;
	}
	return (0);
}

int	check_arguments_options(int num, int ac)
{
	if (ac == 1 && num > 200)
	{
		printf("error: number of philosopher must be less than 200\n");
		return (1);
	}
	if (ac == 1 && num == 0)
	{
		printf("error: number of philosopher must be more than zero\n");
		return (1);
	}
	if (num < 60 && ac > 1 && ac < 5)
	{
		printf("error: time_to_die or time_to_eat or");
		printf("time_to_sleep under 60 ms\n");
		return (1);
	}
	if (INT_MAX < num || num < INT_MIN)
	{
		printf("error: number of philosopher or times is more ");
		printf("than max or less than min\n");
		return (1);
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
		if (check_arguments_options(num, i))
			return (0);
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
