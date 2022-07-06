/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 10:00:00 by alex              #+#    #+#             */
/*   Updated: 2022/07/06 10:45:34 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static bool	should_exit(t_philo *philo)
{
	if (philo->data->anydead
		|| philo->data->full_philos >= philo->data->amount_of_philo)
	{
		pthread_mutex_unlock(&philo->data->death_lock);
		pthread_mutex_unlock(philo->avail_forks[0]);
		pthread_mutex_unlock(philo->avail_forks[1]);
		return (true);
	}
	return (false);
}

void	print_eat(t_philo *philo, struct timeval t)
{
	pthread_mutex_lock(&philo->data->lock_print);
	printf("%ldms philosopher %d has taken a fork\n",
		get_timestamp(t, philo->data->start_time), philo->num);
	printf("%ldms philosopher %d is eating\n",
		get_timestamp(t, philo->data->start_time), philo->num);
	pthread_mutex_unlock(&philo->data->lock_print);
}

int	eat(t_philo *philo)
{
	struct timeval	t;

	pthread_mutex_lock(philo->avail_forks[0]);
	pthread_mutex_lock(philo->avail_forks[1]);
	pthread_create(&philo->deaththread, NULL, check_death, philo);
	gettimeofday(&t, NULL);
	pthread_mutex_lock(&philo->data->death_lock);
	if (should_exit(philo))
		return (1);
	print_eat(philo, t);
	if (philo->times_eaten >= philo->data->amount_to_eat
		&& philo->data->amount_to_eat != -1)
		philo->data->full_philos++;
	if (should_exit(philo))
		return (1);
	pthread_mutex_unlock(&philo->data->death_lock);
	psleep(philo->data->time_to_eat);
	pthread_mutex_unlock(philo->avail_forks[0]);
	pthread_mutex_unlock(philo->avail_forks[1]);
	return (0);
}

int	philo_sleep(t_philo *philo)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	pthread_mutex_lock(&philo->data->death_lock);
	if (philo->data->anydead
		|| philo->data->full_philos >= philo->data->amount_of_philo)
	{
		pthread_mutex_unlock(&philo->data->death_lock);
		return (1);
	}
	pthread_mutex_lock(&philo->data->lock_print);
	printf("%ldms philosopher %d is sleeping\n",
		get_timestamp(t, philo->data->start_time), philo->num);
	pthread_mutex_unlock(&philo->data->lock_print);
	pthread_mutex_unlock(&philo->data->death_lock);
	psleep(philo->data->time_to_sleep);
	return (0);
}

int	think(t_philo *philo)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	pthread_mutex_lock(&philo->data->death_lock);
	if (philo->data->anydead
		|| philo->data->full_philos >= philo->data->amount_of_philo)
	{
		pthread_mutex_unlock(&philo->data->death_lock);
		return (1);
	}
	pthread_mutex_lock(&philo->data->lock_print);
	printf("%ldms philosopher %d is thinking\n",
		get_timestamp(t, philo->data->start_time), philo->num);
	pthread_mutex_unlock(&philo->data->lock_print);
	pthread_mutex_unlock(&philo->data->death_lock);
	return (0);
}
