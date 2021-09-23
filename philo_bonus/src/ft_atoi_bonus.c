/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 02:19:30 by zjamali           #+#    #+#             */
/*   Updated: 2021/09/23 10:28:58 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosopher_bonus.h"

static int	check(int s, int counter, int result)
{
	if (counter > 19)
	{
		if (s > 0)
			return (-1);
		else
			return (0);
	}
	return (s * result);
}

int	ft_atoi(const char *str)
{
	int		i;
	int		s;
	int		result;
	int		counter;

	counter = 0;
	i = 0;
	s = 1;
	result = 0;
	while (str[i] == '\t' || str[i] == '\n' || str[i] == ' '
		|| str[i] == '\v' || str[i] == '\r' || str[i] == '\f')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			s = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
		counter++;
	}
	return (check(s, counter, result));
}
