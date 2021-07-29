/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/13 14:12:21 by zjamali           #+#    #+#             */
/*   Updated: 2021/07/17 11:40:54 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void	*print_message(void *message)
{
	printf("<-- %s\n",message);
	printf("thread id = %d\n",pthread_self());
	sleep(5);
	printf("process id = %d\n",getpid());
	printf("end %s-->\n",message);
	return (NULL);
}

int	main()
{
	pthread_t t1;
	pthread_t t2;
	pthread_t t3;
	pthread_t t4;

	pthread_create(&t1, NULL, print_message,"t1 message");
	pthread_create(&t2, NULL, print_message,"t2 message");
	pthread_create(&t3, NULL, print_message,"t3 message");
	pthread_create(&t4, NULL, print_message,"t4 message");
	
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_join(t3,NULL);
	pthread_join(t4,NULL);
	return (0);
}