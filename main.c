/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/13 14:12:21 by zjamali           #+#    #+#             */
/*   Updated: 2021/09/16 13:34:04 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#define NUMBER_OF_PHILOS 4

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
	pthread_mutex_t is_eating;
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
	long	rest;
	philo = philo_data;
	t_simulation *simulation;

	simulation = philo->simulation;
	rest = 100;
	while (philo->is_live)
	{
		pthread_mutex_lock(&philo->is_eating);
		pthread_mutex_lock(&philo->limit_lock);
		//pthread_mutex_lock(&message);
		//pthread_mutex_unlock(&message);
		rest = philo->limit - get_current_time();
		//printf("%ld  %d lock limit \n", get_current_time() - simulation->start_time,philo->philo_id);
		pthread_mutex_unlock(&philo->limit_lock);
		pthread_mutex_lock(&message);
		//printf("%ld  %d/////////////////////////////////////////////////////// limits: %ld |   the rest :%ld \n", get_current_time() - simulation->start_time, philo->philo_id, philo->limit, rest);
		pthread_mutex_unlock(&message);
		if (rest < 0)
		{
			printf("%ld  %d is will *************** DIE   ************ \n", get_current_time() - simulation->start_time,philo->philo_id);
			philo->is_live =  0;
			if (philo->right_hand)
				pthread_mutex_unlock(&forks[philo->philo_id - 1]);
			if (philo->left_hand)
				pthread_mutex_unlock(&forks[philo->philo_id % NUMBER_OF_PHILOS]);
		}
		pthread_mutex_unlock(&philo->is_eating);
		usleep(100);
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
	{
		pthread_create(&philo_watcher, NULL, watch_philo_routine, philo);
		pthread_detach(philo_watcher);
	}
	while (philo->is_live)
	{
		// begin thinking
		pthread_mutex_lock(&message);
		printf("%ld PHIL %d \033[0;33m THINKING \033[0m**   rest: %ld \n", (get_current_time() - simulation->start_time), philo->philo_id, philo->limit - get_current_time());
		pthread_mutex_unlock(&message);
		
		// taking forks
		pthread_mutex_lock(&forks[philo->philo_id - 1]);
		philo->right_hand = 1;
		//pthread_mutex_lock(&message);
		//printf("%ld philo %d : take fork  %d rest : %ld\n", get_current_time() - simulation->start_time, philo->philo_id, philo->philo_id - 1, philo->limit - get_current_time());
		//pthread_mutex_unlock(&message);
		pthread_mutex_lock(&forks[philo->philo_id % NUMBER_OF_PHILOS]);
		philo->left_hand = 1;
		//pthread_mutex_lock(&message);
		//printf("%ld philo %d : take fork %d rest : %ld\n", get_current_time() - simulation->start_time, philo->philo_id, (philo->philo_id % NUMBER_OF_PHILOS), philo->limit - get_current_time());
		//pthread_mutex_unlock(&message);
	
		/// begin eating
		pthread_mutex_lock(&philo->is_eating);
		philo->limit = get_current_time() + philo->time_to_die;
		pthread_mutex_lock(&message);
		printf("%ld PHILO %d  \033[0;32m EATING \033[0m** rest : %ld\n", get_current_time() - simulation->start_time, philo->philo_id, philo->limit - get_current_time());
		pthread_mutex_unlock(&message);
		pthread_mutex_lock(&philo->limit_lock);
		pthread_mutex_unlock(&philo->limit_lock);
		usleep(philo->time_to_eat * 1000);
		pthread_mutex_unlock(&philo->is_eating);
		
		//// stop eating

		// droping forks
		pthread_mutex_unlock(&forks[philo->philo_id - 1]);
		philo->right_hand = 0;
		pthread_mutex_lock(&message);
		printf("%ld philo %d : drop fork %d rest : %ld\n", get_current_time() - simulation->start_time, philo->philo_id, philo->philo_id - 1, philo->limit - get_current_time());
		pthread_mutex_unlock(&message);
		pthread_mutex_unlock(&forks[philo->philo_id % NUMBER_OF_PHILOS]);
		philo->left_hand = 0;
		pthread_mutex_lock(&message);
		printf("%ld philo %d : drop fork %d rest : %ld\n", get_current_time() - simulation->start_time, philo->philo_id, (philo->philo_id % NUMBER_OF_PHILOS), philo->limit - get_current_time());
		pthread_mutex_unlock(&message);
		// start sleeping
		pthread_mutex_lock(&message);
		printf("%ld PHILO %d \033[0;34m SLEEPING \033[0m** rest : %ld\n", get_current_time() - simulation->start_time, philo->philo_id, philo->limit - get_current_time());
		pthread_mutex_unlock(&message);
		usleep(philo->time_to_sleep * 1000);
	
		pthread_mutex_lock(&message);
		printf("%ld %d ********** rest : %ld ***** limits:  %ld   ****\n", get_current_time() - simulation->start_time, philo->philo_id, philo->limit - get_current_time(), philo->limit);
		pthread_mutex_unlock(&message);
	}
	printf("%ld PHILO %d \033[0;31m die \033[0m**\n", get_current_time() - simulation->start_time, philo->philo_id);
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
		philos_data[i].time_to_die = 410;
		philos_data[i].time_to_eat = 200;
		philos_data[i].time_to_sleep = 200;

		pthread_mutex_init(&philos_data[i].is_eating, NULL);
		pthread_mutex_init(&philos_data[i].limit_lock, NULL);
		philos_data[i].limit = get_current_time() + philos_data[i].time_to_die;
		philos_data[i].simulation = &simulation;
		pthread_create(&philos[i], NULL, philo_routine, &philos_data[i]);
		usleep(10);
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
