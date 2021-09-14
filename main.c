/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/13 14:12:21 by zjamali           #+#    #+#             */
/*   Updated: 2021/09/14 18:29:14 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
/*
int main() {
  struct timeval current_time;
  gettimeofday(&current_time, NULL);
  printf("seconds : %ld\nmicro seconds : %ld",
    current_time.tv_sec, current_time.tv_usec);

  return 0;
}
*/

#define NUMBER_OF_PHILOS 5

typedef struct s_philo_data
{
	int philo_id;
	int right_hand;
	int left_hand;
} t_philo;

pthread_mutex_t forks[NUMBER_OF_PHILOS];
pthread_mutex_t message;

void *philo_routine(void *philo_data)
{
	t_philo *philo = philo_data;

	/// DIETHREADIN
	//printf("philosopher %d : is begin it's routine - right/left hand  : %d/%d\n",philo->philo_id, philo->right_hand, philo->left_hand);
	while (1)
	{
		pthread_mutex_lock(&message);
		printf("**PHIL %d THINKING**\n", philo->philo_id);
		usleep(1000000);
		pthread_mutex_unlock(&message);
		pthread_mutex_lock(&forks[philo->philo_id - 1]);
		pthread_mutex_lock(&forks[philo->philo_id % NUMBER_OF_PHILOS]);
		pthread_mutex_lock(&message);
		printf("**PHIL %d EATING**\n", philo->philo_id);
		pthread_mutex_unlock(&message);
		usleep(1000000);
		pthread_mutex_unlock(&forks[philo->philo_id - 1]);
		pthread_mutex_unlock(&forks[philo->philo_id % NUMBER_OF_PHILOS]);
		pthread_mutex_lock(&message);
		printf("**PHIL %d SLEEPING**\n", philo->philo_id);
		usleep(1000000);
		pthread_mutex_unlock(&message);
	}
	return(NULL);
}

int main()
{
	pthread_t philos[NUMBER_OF_PHILOS];
	t_philo philos_data[NUMBER_OF_PHILOS];
	int i;

	pthread_mutex_init(&message, NULL);
	i = 0;
	// create threads
	while (i < NUMBER_OF_PHILOS)
	{
		pthread_mutex_init(&forks[i], NULL);
		philos_data[i].right_hand = 0;
		philos_data[i].left_hand = 0;
		philos_data[i].philo_id = i + 1;
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
