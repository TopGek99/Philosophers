/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 14:55:14 by arowe             #+#    #+#             */
/*   Updated: 2022/06/06 19:50:49 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ph_isspace(char c)
{
	return (c == ' ' || c == '\n'
		|| c == '\t' || c == '\v'
		|| c == '\f' || c == '\r');
}

int	ph_isdigit(char c)
{
	return (c >= '0' && c <= '9');
}

int	ph_atoi(const char *str)
{
	int	index;
	int	res;
	int	temp;

	index = 0;
	while (ph_isspace(str[index]))
		index++;
	res = 1;
	if (str[index] == '-')
	{
		res *= -1;
		index++;
	}
	temp = 0;
	while (ph_isdigit(str[index]))
	{
		if ((temp > (temp * 10) || temp > (temp * 10) + (str[index] - '0'))
			&& (((long)temp * 10) + (str[index] - '0')) * res != -2147483648)
			return (-1);
		temp *= 10;
		temp += (str[index++] - '0');
	}
	if (str[index] || (temp == 0 && res == -1))
		return (-1);
	return (temp * res);
}

long	get_timestamp(struct timeval now, struct timeval start)
{
	return ((((now.tv_sec * 1000000) + now.tv_usec) - ((start.tv_sec * 1000000) + start.tv_usec)) / 1000);
}

void	*do_stuff_philo(void *input)
{
	t_philo			*philo;
	struct timeval	time;
	long			death_timer;

	philo = (t_philo *)input;
	while (!philo->dead)
	{
		pthread_mutex_lock(philo->avail_forks[0]);
		pthread_mutex_lock(philo->avail_forks[1]);
		gettimeofday(&time, NULL);
		pthread_mutex_lock(&philo->data->lock_print);
		printf("%ldms philosopher %d has taken a fork\n", get_timestamp(time, philo->data->start_time), philo->num);
		printf("%ldms philosopher %d is eating\n", get_timestamp(time, philo->data->start_time), philo->num);
		pthread_mutex_unlock(&philo->data->lock_print);
		usleep(philo->data->time_to_eat * 1000);
		death_timer = 0;
		pthread_mutex_unlock(philo->avail_forks[0]);
		pthread_mutex_unlock(philo->avail_forks[1]);
		gettimeofday(&time, NULL);
		pthread_mutex_lock(&philo->data->lock_print);
		printf("%ldms philosopher %d is sleeping\n", get_timestamp(time, philo->data->start_time), philo->num);
		pthread_mutex_unlock(&philo->data->lock_print);
		death_timer += philo->data->time_to_sleep;
		if (death_timer >= philo->data->time_to_die)
		{
			death_timer -= philo->data->time_to_die;
			usleep(death_timer * 1000);
			printf("%ldms philosopher %d has died\n", get_timestamp(time, philo->data->start_time), philo->num);
		}
		usleep(philo->data->time_to_sleep * 1000);
		gettimeofday(&time, NULL);
		pthread_mutex_lock(&philo->data->lock_print);
		printf("%ldms philosopher %d is thinking\n", get_timestamp(time, philo->data->start_time), philo->num);
		pthread_mutex_unlock(&philo->data->lock_print);
		if (death_timer >= philo->data->time_to_die)
		{
			death_timer -= philo->data->time_to_die;
			usleep(death_timer * 1000);
			printf("%ldms philosopher %d has died\n", get_timestamp(time, philo->data->start_time), philo->num);
		}
	}
	return (NULL);
}