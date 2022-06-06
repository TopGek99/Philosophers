/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 14:55:17 by arowe             #+#    #+#             */
/*   Updated: 2022/06/06 19:56:16 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// args[0] = number_of_philosophers
// args[1] = time_to_die
// args[2] = time_to_eat
// args[3] = time_to_sleep
// args[4] = number_of_times_each_philosopher_must_eat (-1 if not given)
int	main(int argc, char *argv[])
{
	struct timeval	time;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	lock;
	t_data			data;
	int				i;

	if (argc < 5 || argc > 6)
	{
		printf("Error\n");
		return (0);
	}
	
	i = 0;
	if (argc == 5)
		data.amount_to_eat = -1;
	else
		data.amount_to_eat = ph_atoi(argv[5]);
	pthread_mutex_init(&lock, NULL);
	data.lock = &lock;
	data.amount_of_philo = ph_atoi(argv[1]);
	data.time_to_die = ph_atoi(argv[2]);
	data.time_to_eat = ph_atoi(argv[3]);
	data.time_to_sleep = ph_atoi(argv[4]);
	gettimeofday(&time, NULL);
	data.start_time = time.tv_usec;
	philos = (t_philo *)malloc(sizeof(t_philo) * data.amount_of_philo);
	forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * data.amount_of_philo);
	i = 0;
	while (i < data.amount_of_philo)
		pthread_mutex_init(&forks[i++], NULL);
	i = 0;
	philos[i].forks[0] = forks[data.amount_of_philo - 1];
	while (i < data.amount_of_philo)
	{
		if (i != 0)
			philos[i].forks[0] = forks[i - 1];
		philos[i].forks[1] = forks[i];
		philos[i].num = i + 1;
		i++;
	}
	i = 0;
	while (i < data.amount_of_philo)
	{
		data.philo = (t_philo *)malloc(sizeof(t_philo));
		*(data.philo) = philos[i];
		pthread_create(&data.philo->philthread, NULL, &do_stuff_philo, &data);
		i++;
	}
	i = 0;
	while(i < data.amount_of_philo)
		pthread_join(philos[i++].philthread, NULL);
	return (0);
}
