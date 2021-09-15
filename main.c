/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/13 14:12:21 by zjamali           #+#    #+#             */
/*   Updated: 2021/09/15 18:51:51 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#define NUMBER_OF_PHILOS 5

typedef struct s_simulation t_simulation;

typedef struct s_philo_data
{
	int philo_id;
	int right_hand;
	int left_hand;
	int time_to_sleep;
	int time_to_die;
	int time_to_eat;
	int eating_times;
	int is_live;
	int start;
	long limit;
	pthread_mutex_t limit_lock;
	t_simulation *simulation;
} t_philo;

struct s_simulation
{
	long start_time;
	t_philo *philo;
};


pthread_mutex_t forks[NUMBER_OF_PHILOS];
pthread_mutex_t message;

long get_current_time()
{	
	struct timeval current_time;
	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

void *watch_philo_routine(void *philo_data)
{
	t_philo *philo;

	philo = philo_data;
	
	while (1)
	{
		//printf("philo time : %ld | current time : %ld | rest time : %ld\n",philo->limit, get_current_time(),philo->limit - get_current_time());
		if (philo->limit && philo->limit - get_current_time() <= 0)
		{
			philo->is_live =  0;
		}
	}
	return (NULL);
}

void *philo_routine(void *philo_data)
{
	pthread_t philo_watcher;
	t_philo *philo;
	philo = philo_data;
	t_simulation *simulation;

	simulation = philo->simulation;
	if (philo)
		pthread_create(&philo_watcher, NULL, watch_philo_routine, philo);
	while (philo->is_live)
	{
		printf("%ld PHIL %d \033[0;33m THINKING \033[0m**\n",(get_current_time() - simulation->start_time), philo->philo_id);
		pthread_mutex_lock(&forks[philo->philo_id - 1]);
		printf("%ld philo %d : take fork %d\n",get_current_time() - simulation->start_time, philo->philo_id, philo->philo_id - 1);
		pthread_mutex_lock(&forks[philo->philo_id % NUMBER_OF_PHILOS]);
		printf("%ld philo %d : take fork %d\n",get_current_time() - simulation->start_time, philo->philo_id, (philo->philo_id % NUMBER_OF_PHILOS));
		printf("%ld PHILO %d  \033[0;32m EATING \033[0m**\n",get_current_time() - simulation->start_time, philo->philo_id);
		pthread_mutex_lock(&philo->limit_lock);
		philo->limit = get_current_time() + philo->time_to_die;
		pthread_mutex_unlock(&philo->limit_lock);
		usleep(philo->time_to_eat * 1000);
		pthread_mutex_unlock(&forks[philo->philo_id - 1]);
		printf("%ld philo %d : drop fork %d\n",get_current_time() - simulation->start_time, philo->philo_id, philo->philo_id - 1);
		pthread_mutex_unlock(&forks[philo->philo_id % NUMBER_OF_PHILOS]);
		printf("%ld philo %d : drop fork %d\n",get_current_time() - simulation->start_time, philo->philo_id, (philo->philo_id % NUMBER_OF_PHILOS));
		printf("%ld PHILO %d \033[0;34m SLEEPING \033[0m**\n",get_current_time() - simulation->start_time, philo->philo_id);
		usleep(philo->time_to_sleep * 1000);
		printf("%d ********** %ld *********\n",philo->philo_id, philo->limit - get_current_time());
	}
	printf("%ld PHILO %d \033[0;31m die \033[0m**\n",get_current_time() - simulation->start_time, philo->philo_id);
	return (NULL);
}

int main()
{
	pthread_t philos[NUMBER_OF_PHILOS];
	t_philo philos_data[NUMBER_OF_PHILOS];
	int i;
	t_simulation simulation;

	pthread_mutex_init(&message, NULL);
	i = 0;
	// create threads
	simulation.start_time = 0;
	simulation.start_time = get_current_time();
	simulation.philo = philos_data;
	while (i < NUMBER_OF_PHILOS)
	{
		pthread_mutex_init(&forks[i], NULL);
		philos_data[i].right_hand = 0;
		philos_data[i].left_hand = 0;
		philos_data[i].philo_id = i + 1;
		philos_data[i].is_live = 1;
		philos_data[i].time_to_die = 350;
		philos_data[i].time_to_eat = 100;
		philos_data[i].time_to_sleep = 100;
		pthread_mutex_init(&philos_data[i].limit_lock, NULL);
		philos_data[i].limit = get_current_time() + philos_data[i].time_to_die;
		philos_data[i].simulation = &simulation;
		//usleep(1);
		pthread_create(&philos[i], NULL, philo_routine, &philos_data[i]);
		i++;
	}
	i = 0;
	// wait threads
	while (i < NUMBER_OF_PHILOS)
	{
		pthread_join(philos[i], NULL);
		i++;
	}
	i = 0;
	while (i < NUMBER_OF_PHILOS)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	return (0);
}
