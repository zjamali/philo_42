/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/13 14:12:21 by zjamali           #+#    #+#             */
/*   Updated: 2021/09/18 20:03:25 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

long	get_current_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

void	print_to_terminal(char *output, t_simulation *simulation, int philo_id,
					   int is_philo_die)
{
	pthread_mutex_lock(&simulation->message);
	if (is_philo_die)
	{
		printf("%ld\t%d%s", get_current_time() - simulation->start_time,
			philo_id, output);
		pthread_mutex_unlock(&simulation->main_lock);
	}
	else
	{
		printf("%ld\t%d%s", get_current_time() - simulation->start_time,
			philo_id, output);
		pthread_mutex_unlock(&simulation->message);
	}
}

void	philo_taken_forks(t_philo *current_philo)
{
	t_philo			*philo;
	t_simulation	*simulation;

	philo = current_philo;
	simulation = philo->simulation;

	pthread_mutex_lock(&simulation->forks[philo->philo_id - 1]);
	print_to_terminal("\t taken fork\n", simulation, philo->philo_id, 0);
	pthread_mutex_lock(
		&simulation->forks[philo->philo_id % simulation->number_of_philos]);
	print_to_terminal("\t taken fork\n", simulation, philo->philo_id, 0);
}

void	philo_is_eating(t_philo *current_philo)
{
	t_philo			*philo;
	t_simulation	*simulation;

	philo = current_philo;
	simulation = philo->simulation;
	pthread_mutex_lock(&philo->is_eating);
	philo->limit = get_current_time() + philo->time_to_die;
	print_to_terminal("\t\033[0;32m EATING \033[0m\n", simulation,
		philo->philo_id, 0);
	usleep(philo->time_to_eat * 1000);
	pthread_mutex_unlock(&philo->is_eating);
}

void	philo_start_sleeping(t_philo *current_philo)
{
	t_philo			*philo;
	t_simulation	*simulation;

	philo = current_philo;
	simulation = philo->simulation;
	pthread_mutex_unlock(&simulation->forks[philo->philo_id - 1]);
	pthread_mutex_unlock(
		&simulation->forks[philo->philo_id % simulation->number_of_philos]);
	print_to_terminal("\t\033[0;34m SLEEPING \033[0m\n", simulation,
		philo->philo_id, 0);
	usleep(philo->time_to_sleep * 1000);
}

void	*watch_philo_routine(void *philo_data)
{
	t_philo			*philo;
	t_simulation	*simulation;

	philo = philo_data;
	simulation = philo->simulation;
	while (philo->is_live)
	{
		pthread_mutex_lock(&philo->is_eating);
		if (philo->limit < get_current_time())
		{
			philo->is_live = 0;
			print_to_terminal("\033[0;31m DIE \033[0m\n", simulation,
				philo->philo_id, 1);
		}
		pthread_mutex_unlock(&philo->is_eating);
		usleep(1000);
	}
	return (NULL);
}

void	philo_start_thinking(t_philo *current_philo)
{
	t_philo			*philo;
	t_simulation	*simulation;

	philo = current_philo;
	simulation = philo->simulation;
	print_to_terminal("\t\033[0;33m THINKING \033[0m\n", simulation,
		philo->philo_id, 0);
}

void	*philo_routine(void *philo_data)
{
	pthread_t		philo_watcher;
	t_philo			*philo;
	t_simulation	*simulation;

	philo = philo_data;
	simulation = philo->simulation;
	if (philo)
	{
		pthread_create(&philo_watcher, NULL, watch_philo_routine, philo);
		pthread_detach(philo_watcher);
	}
	while (philo->is_live && philo->eating_times)
	{
		philo_taken_forks(philo);
		philo_is_eating(philo);
		if (philo->is_times_to_eat)
			philo->eating_times--;
		else
			philo->eating_times = 1;
		philo_start_sleeping(philo);
		philo_start_thinking(philo);
	}
	return (NULL);
}

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

t_simulation 	*ft_parse_args(int ac, char **av)
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
	}
	else
	{
		simulation->is_times_to_eat = 0;
		simulation->eating_times = 1;
	}
	return (simulation);
}

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
	init_simulation_mutex(simulation);
	while (i < simulation->number_of_philos)
	{
		pthread_mutex_init(&simulation->forks[i], NULL);
		philos_data[i].philo_id = i + 1;
		philos_data[i].is_live = 1;
		philos_data[i].time_to_die = simulation->time_to_die;
		philos_data[i].time_to_eat = simulation->time_to_eat;
		philos_data[i].time_to_sleep = simulation->time_to_sleep;
		philos_data[i].is_times_to_eat = simulation->is_times_to_eat;
		philos_data[i].eating_times = simulation->eating_times;
		philos_data[i].simulation = simulation;
		pthread_mutex_init(&philos_data[i].is_eating, NULL);
		philos_data[i].limit = get_current_time()
			+ philos_data[i].time_to_die;
		i++;
	}
	return (philos_data);
}

void	destroy_simulation(t_simulation *simulation,t_philo *philos_data)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&simulation->main_lock);
	pthread_mutex_unlock(&simulation->main_lock);
	pthread_mutex_unlock(&simulation->main_lock);
	while (i < simulation->number_of_philos)
	{
		pthread_mutex_destroy(&simulation->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&simulation->main_lock);
	pthread_mutex_destroy(&simulation->message);
	free(simulation);
	free(philos_data);
}

int	main(int ac, char **av)
{
	int				i;
	t_simulation	*simulation;
	pthread_t		*philos_threads;
	t_philo			*philos_data;

	i = 0;
	if (ac >= 5 && check_args(ac, av))
	{
		simulation = ft_parse_args(ac, av);
		pthread_mutex_init(&simulation->message, NULL);
		philos_data = init_simaulation_philos(simulation);
		philos_threads = (pthread_t *)malloc(sizeof(pthread_t)
				* simulation->number_of_philos);
		while (i < simulation->number_of_philos)
		{
			pthread_create(&philos_threads[i], NULL,
				philo_routine, &philos_data[i]);
			pthread_detach(philos_threads[i]);
			usleep(10);
			i++;
		}
		destroy_simulation(simulation, philos_data);
	}
	return (0);
}
