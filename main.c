/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/13 14:12:21 by zjamali           #+#    #+#             */
/*   Updated: 2021/09/17 14:03:24 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#define NUMBER_OF_PHILOS 4
#define TIME_TO_DIE 410
#define TIME_TO_EAT 200
#define TIME_TO_SLEEP 200
#define TIMES_TO_EAT 8

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
	int is_times_to_eat;
	int times_to_eat;
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
pthread_mutex_t die;

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
	t_simulation *simulation;

	simulation = philo->simulation;
	while (philo->is_live)
	{
		pthread_mutex_lock(&philo->is_eating);
		//pthread_mutex_lock(&philo->limit_lock);
		if (philo->limit < get_current_time())
		{
			philo->is_live = 0;
			pthread_mutex_lock(&message);
			printf("%ld\t%d\t\033[0;31m DIE \033[0m\n", get_current_time() - simulation->start_time, philo->philo_id);
			//pthread_mutex_unlock(&message);
			pthread_mutex_unlock(&die);
		}
		//pthread_mutex_unlock(&philo->limit_lock);
		pthread_mutex_unlock(&philo->is_eating);
		usleep(500);
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
		printf("%ld\t%d\t\033[0;33m THINKING \033[0m\n", (get_current_time() - simulation->start_time), philo->philo_id);
		pthread_mutex_unlock(&message);

		// taking forks
		pthread_mutex_lock(&forks[philo->philo_id - 1]);
		philo->right_hand = 1;
		pthread_mutex_lock(&message);
		printf("%ld\t%d\t taken fork\n", get_current_time() - simulation->start_time, philo->philo_id);
		pthread_mutex_unlock(&message);
		pthread_mutex_lock(&forks[philo->philo_id % NUMBER_OF_PHILOS]);
		philo->left_hand = 1;
		pthread_mutex_lock(&message);
		printf("%ld\t%d\t taken fork\n", get_current_time() - simulation->start_time, philo->philo_id);
		pthread_mutex_unlock(&message);

		/// begin eating
		pthread_mutex_lock(&philo->is_eating);
		philo->limit = get_current_time() + philo->time_to_die;
		pthread_mutex_lock(&message);
		printf("%ld\t%d\t\033[0;32m EATING \033[0m\n", get_current_time() - simulation->start_time, philo->philo_id);
		//pthread_mutex_lock(&philo->limit_lock);
		//pthread_mutex_unlock(&philo->limit_lock);
		pthread_mutex_unlock(&message);
		usleep(philo->time_to_eat * 1000);
		pthread_mutex_unlock(&philo->is_eating);
		//// stop eating

		// droping forks
		pthread_mutex_unlock(&forks[philo->philo_id - 1]);
		philo->right_hand = 0;
		pthread_mutex_unlock(&forks[philo->philo_id % NUMBER_OF_PHILOS]);
		philo->left_hand = 0;
		// start sleeping
		pthread_mutex_lock(&message);
		printf("%ld\t%d\t\033[0;34m SLEEPING \033[0m\n", get_current_time() - simulation->start_time, philo->philo_id);
		pthread_mutex_unlock(&message);
		usleep(philo->time_to_sleep * 1000);
	}
	//pthread_mutex_lock(&message);
	//printf("%ld\t%d\t\033[0;31m DIE \033[0m\n", get_current_time() - simulation->start_time, philo->philo_id);
	////pthread_mutex_unlock(&message);
	//pthread_mutex_unlock(&die);
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
	pthread_mutex_init(&die, NULL);

	pthread_mutex_lock(&die);
	while (i < NUMBER_OF_PHILOS)
	{
		pthread_mutex_init(&forks[i], NULL);
		philos_data[i].right_hand = 0;
		philos_data[i].left_hand = 0;
		philos_data[i].philo_id = i + 1;
		philos_data[i].is_live = 1;
		philos_data[i].time_to_die = TIME_TO_DIE;
		philos_data[i].time_to_eat = TIME_TO_EAT;
		philos_data[i].time_to_sleep = TIME_TO_SLEEP;
		/// number_of_times_each_philosopher_must_eat
		philos_data[i].is_times_to_eat = 1;
		philos_data[i].eating_times = TIMES_TO_EAT;
		///////
		pthread_mutex_init(&philos_data[i].is_eating, NULL);
		pthread_mutex_init(&philos_data[i].limit_lock, NULL);
		philos_data[i].limit = get_current_time() + philos_data[i].time_to_die;
		philos_data[i].simulation = &simulation;
		pthread_create(&philos[i], NULL, philo_routine, &philos_data[i]);
		pthread_detach(philos[i]);
		usleep(10);
		i++;
	}
	pthread_mutex_lock(&die);
	pthread_mutex_unlock(&die);
	i = 0;
	while (i < NUMBER_OF_PHILOS)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	pthread_mutex_destroy(&die);
	pthread_mutex_destroy(&message);
	return (0);
}
