/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 14:55:14 by arowe             #+#    #+#             */
/*   Updated: 2022/07/05 12:22:34 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ph_isspace(char c)
{
	return (c == ' ' || c == '\n'
		|| c == '\t' || c == '\v'
		|| c == '\f' || c == '\r');
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
	while (str[index] >= '0' && str[index] <= '9')
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

void	*one_philo_die(t_philo *p)
{
	struct timeval	t;

	usleep(p->data->time_to_die * 1000);
	gettimeofday(&t, NULL);
	printf("%ldms philosopher %d has died\n",
		get_timestamp(t, p->data->start_time), p->num);
	return (NULL);
}

bool	should_exit(t_philo *philo, pthread_t tid)
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
