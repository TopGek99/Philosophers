/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 14:55:17 by arowe             #+#    #+#             */
/*   Updated: 2022/07/05 12:20:15 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
	pthread_mutex_lock(&philo->data->lock_print);
	if (!philo->data->anydead)
		printf("%ldms philosopher %d has died\n",
			get_timestamp(curr, philo->data->start_time), philo->num);
	pthread_mutex_unlock(&philo->data->lock_print);
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
	struct timeval	time;
	t_data			data;
	t_philo			*philos;
	int				i;

	if (argc == 6)
		data.amount_to_eat = ph_atoi(argv[5]);
	else if (argc == 5)
		data.amount_to_eat = -1;
	else
	{
		printf("Error\n");
		return (1);
	}
	data.amount_of_philo = ph_atoi(argv[1]);
	data.time_to_die = ph_atoi(argv[2]);
	data.time_to_eat = ph_atoi(argv[3]);
	data.time_to_sleep = ph_atoi(argv[4]);
	gettimeofday(&time, NULL);
	data.start_time = time;
	data.anydead = false;
	philos = (t_philo *)malloc(sizeof(t_philo) * data.amount_of_philo);
	data.forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * data.amount_of_philo);
	i = 0;
	while (i < data.amount_of_philo)
		pthread_mutex_init(&data.forks[i++], NULL);
	pthread_mutex_init(&data.lock_print, NULL);
	pthread_mutex_init(&data.death_lock, NULL);
	i = 0;
	philos[i].avail_forks[0] = &data.forks[data.amount_of_philo - 1];
	while (i < data.amount_of_philo)
	{
		if (i != 0)
			philos[i].avail_forks[0] = &data.forks[i - 1];
		if (i < data.amount_of_philo - 1)
			philos[i].next = &philos[i+1];
		else
			philos[i].next = &philos[0];
		philos[i].avail_forks[1] = &data.forks[i];
		philos[i].data = &data;
		philos[i].num = i + 1;
		philos[i].times_eaten = 0;
		pthread_create(&philos[i].philthread, NULL, do_stuff_philo, &philos[i]);
		i++;
	}
	i = 0;
	while (i < data.amount_of_philo)
	{
		pthread_join(philos[i].philthread, NULL);
		pthread_join(philos[i].deaththread, NULL);
		i++;
	}
	return (0);
}
