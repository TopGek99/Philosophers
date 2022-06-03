/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arowe <arowe@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 14:55:17 by arowe             #+#    #+#             */
/*   Updated: 2022/06/03 15:28:32 by arowe            ###   ########.fr       */
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
	pthread_t		philo_thread;
	pthread_mutex_t	fork;
	struct timeval	start_time;
	t_philo			philo;
	int				i;

	if (argc < 5 || argc > 6)
	{
		printf("Error\n");
		return (0);
	}
	i = 0;
	if (argc == 5)
		philo.amount_to_eat = -1;
	else
		philo.amount_to_eat = argv[5];
	philo.time_to_die = ph_atoi(argv[2]);
	philo.time_to_eat = ph_atoi(argv[3]);
	philo.time_to_sleep = ph_atoi(argv[4]);
	philo.fork = &fork;
	gettimeofday(&start_time, NULL);
	philo.start_time = &start_time;
	pthread_mutex_init(philo.fork, NULL);
	i = 0;
	while (i < ph_atoi(argv[1]))
	{
		philo.num = ++i;
		pthread_create(&philo_thread, NULL, odd_philo, &philo);
		philo.num = ++i;
	}
	return (0);
}
