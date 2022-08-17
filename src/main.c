/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 14:55:17 by arowe             #+#    #+#             */
/*   Updated: 2022/07/06 10:44:56 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static bool	should_exit(t_philo *philo, pthread_t tid)
{
	return (philo->data->anydead
		|| philo->deaththread != tid
		|| philo->data->full_philos >= philo->data->amount_of_philo);
}

bool	death_loop(t_philo *philo, struct timeval *curr,
	struct timeval *start, pthread_t tid)
{
	while (get_timestamp(*curr, *start) < philo->data->time_to_die)
	{
		if (should_exit(philo, tid))
			return (true);
		psleep(10);
		gettimeofday(curr, NULL);
	}
	return (false);
}

void	*check_death(void *input)
{
	t_philo			*philo;
	pthread_t		tid;
	struct timeval	start;
	struct timeval	curr;

	philo = (t_philo *)input;
	tid = philo->deaththread;
	philo->times_eaten++;
	gettimeofday(&start, NULL);
	gettimeofday(&curr, NULL);
	death_loop(philo, &curr, &start, tid);
	if (should_exit(philo, tid))
		return (NULL);
	if (!philo->data->anydead)
	{
		pthread_mutex_lock(&philo->data->lock_print);
		printf("%ldms philosopher %d has died\n",
			get_timestamp(curr, philo->data->start_time), philo->num);
		pthread_mutex_unlock(&philo->data->lock_print);
	}
	philo->data->anydead = true;
	return (NULL);
}

void	*do_stuff_philo(void *input)
{
	t_philo	*philo;

	philo = (t_philo *)input;
	if (philo->data->amount_of_philo == 1)
		return (one_philo_die(philo));
	if (philo->num % 2 == 0)
		psleep(philo->data->time_to_eat);
	while (1)
	{
		if (eat(philo) || philo_sleep(philo) || think(philo))
			break ;
	}
	return (NULL);
}

int	main(int argc, char *argv[])
{
	t_data			data;
	t_philo			*philos;
	int				i;

	if (init_data(&data, argc, argv))
		return (1);
	philos = (t_philo *)malloc(sizeof(t_philo) * data.amount_of_philo);
	i = 0;
	while (i < data.amount_of_philo)
		pthread_mutex_init(&data.forks[i++], NULL);
	pthread_mutex_init(&data.lock_print, NULL);
	pthread_mutex_init(&data.death_lock, NULL);
	i = 0;
	philos[i].avail_forks[0] = &data.forks[data.amount_of_philo - 1];
	while (i < data.amount_of_philo)
	{
		init_philo(philos, &data, i);
		pthread_create(&philos[i].philthread, NULL, do_stuff_philo, &philos[i]);
		i++;
	}
	join_all(philos, &data);
	return (0);
}
