/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 14:55:14 by arowe             #+#    #+#             */
/*   Updated: 2022/07/01 16:53:23 by alex             ###   ########.fr       */
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
	return (((now.tv_sec * 1000) + now.tv_usec / 1000) - ((start.tv_sec * 1000) + start.tv_usec / 1000));
}

int	psleep(int t)
{
	struct timeval	time1;
	struct timeval	time2;

	gettimeofday(&time1, NULL);
	while (1)
	{
		gettimeofday(&time2, NULL);
		if (get_timestamp(time2, time1) >= t)
			return (get_timestamp(time2, time1));
		usleep(100);
	}
}

void	*one_philo_die(t_philo *p)
{
	struct timeval t;

	usleep(p->data->time_to_die * 1000);
	gettimeofday(&t, NULL);
	printf("%ldms philosopher %d has died\n", get_timestamp(t, p->data->start_time), p->num);
	return (NULL);
}

void	*check_death(void *input)
{
	t_philo			*philo;
	pthread_t		tid;
	int				eat_no;
	struct timeval	start;
	struct timeval	curr;

	philo = (t_philo *)input;
	tid = philo->deaththread;
	philo->times_eaten++;
	eat_no = philo->times_eaten;
	gettimeofday(&start, NULL);
	gettimeofday(&curr, NULL);
	while (get_timestamp(curr, start) < philo->data->time_to_die)
	{
		if (philo->times_eaten > eat_no || philo->deaththread != tid || philo->data->full_philos == philo->data->amount_of_philo)
		{
			// if (philo->data->full_philos == philo->data->amount_of_philo)
			// 	printf("philo %d returning\n", philo->num);
			return (NULL);
		}
		psleep(10);
		gettimeofday(&curr, NULL);
	}
	if (philo->data->anydead || philo->deaththread != tid || philo->data->full_philos == philo->data->amount_of_philo)
	{
		// if (philo->data->full_philos == philo->data->amount_of_philo)
		// 	printf("philo %d returning\n", philo->num);
		return (NULL);
	}
	pthread_mutex_lock(&philo->data->lock_print);
	printf("%ldms philosopher %d has died\n", get_timestamp(curr, philo->data->start_time), philo->num);
	philo->data->anydead = true;
	return (NULL);
}

void	*do_stuff_philo(void *input)
{
	t_philo	*philo;

	philo = (t_philo *)input;
	if (philo->data->amount_of_philo == 1)
		return (one_philo_die(philo));
	while (1)
	{
		if (eat(philo) || philo_sleep(philo) || think(philo))
			break ;
	}
	return (NULL);
}