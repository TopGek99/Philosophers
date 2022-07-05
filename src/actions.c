/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 10:00:00 by alex              #+#    #+#             */
/*   Updated: 2022/07/05 10:35:07 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	eat(t_philo *philo)
{
	struct	timeval t;

	pthread_mutex_lock(philo->avail_forks[0]);
	pthread_mutex_lock(philo->avail_forks[1]);
	pthread_create(&philo->deaththread, NULL, check_death, philo);
	gettimeofday(&t, NULL);
    pthread_mutex_lock(&philo->data->death_lock);
	if (philo->data->anydead || philo->data->full_philos >= philo->data->amount_of_philo)
	{
        pthread_mutex_unlock(&philo->data->death_lock);
        pthread_mutex_unlock(philo->avail_forks[0]);
	    pthread_mutex_unlock(philo->avail_forks[1]);
		return (1);
	}
	pthread_mutex_lock(&philo->data->lock_print);
	printf("%ldms philosopher %d has taken a fork\n", get_timestamp(t, philo->data->start_time), philo->num);
	printf("%ldms philosopher %d is eating\n", get_timestamp(t, philo->data->start_time), philo->num);
    pthread_mutex_unlock(&philo->data->lock_print);
	if (philo->times_eaten >= philo->data->amount_to_eat && philo->data->amount_to_eat != -1)
		philo->data->full_philos++;
    if (philo->data->full_philos >= philo->data->amount_of_philo)
	{
        pthread_mutex_unlock(&philo->data->death_lock);
        pthread_mutex_unlock(philo->avail_forks[0]);
	    pthread_mutex_unlock(philo->avail_forks[1]);
		return (1);
	}
    pthread_mutex_unlock(&philo->data->death_lock);
	psleep(philo->data->time_to_eat);
	pthread_mutex_unlock(philo->avail_forks[0]);
	pthread_mutex_unlock(philo->avail_forks[1]);
	
	return (0);
}

int	philo_sleep(t_philo *philo)
{
	struct timeval t;

	gettimeofday(&t, NULL);
    pthread_mutex_lock(&philo->data->death_lock);
	if (philo->data->anydead || philo->data->full_philos >= philo->data->amount_of_philo)
	{
		pthread_mutex_unlock(&philo->data->death_lock);
		return (1);
	}
	pthread_mutex_lock(&philo->data->lock_print);
	printf("%ldms philosopher %d is sleeping\n", get_timestamp(t, philo->data->start_time), philo->num);
	pthread_mutex_unlock(&philo->data->lock_print);
    pthread_mutex_unlock(&philo->data->death_lock);
	psleep(philo->data->time_to_sleep);
	return (0);
}

int	think(t_philo *philo)
{
	struct timeval t;

	gettimeofday(&t, NULL);
    pthread_mutex_lock(&philo->data->death_lock);
	if (philo->data->anydead || philo->data->full_philos >= philo->data->amount_of_philo)
	{
        pthread_mutex_unlock(&philo->data->death_lock);
		return (1);
	}
	pthread_mutex_lock(&philo->data->lock_print);
	printf("%ldms philosopher %d is thinking\n", get_timestamp(t, philo->data->start_time), philo->num);
	pthread_mutex_unlock(&philo->data->lock_print);
    pthread_mutex_unlock(&philo->data->death_lock);
	return (0);
}