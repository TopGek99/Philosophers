/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 09:40:57 by alex              #+#    #+#             */
/*   Updated: 2022/07/06 10:43:55 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_data(t_data *data, int ac, char *av[])
{
	struct timeval	time;

	if (ac == 6)
		data->amount_to_eat = ph_atoi(av[5]);
	else if (ac == 5)
		data->amount_to_eat = -1;
	else
	{
		printf("Error\n");
		return (1);
	}
	data->amount_of_philo = ph_atoi(av[1]);
	data->time_to_die = ph_atoi(av[2]);
	data->time_to_eat = ph_atoi(av[3]);
	data->time_to_sleep = ph_atoi(av[4]);
	gettimeofday(&time, NULL);
	data->start_time = time;
	data->anydead = false;
	data->forks
		= (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* data->amount_of_philo);
	return (0);
}

void	init_philo(t_philo *philos, t_data *data, int i)
{
	if (i != 0)
		philos[i].avail_forks[0] = &data->forks[i - 1];
	if (i < data->amount_of_philo - 1)
		philos[i].next = &philos[i + 1];
	else
		philos[i].next = &philos[0];
	philos[i].avail_forks[1] = &data->forks[i];
	philos[i].data = data;
	philos[i].num = i + 1;
	philos[i].times_eaten = 0;
}
