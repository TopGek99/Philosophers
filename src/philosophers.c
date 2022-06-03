/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arowe <arowe@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 14:55:14 by arowe             #+#    #+#             */
/*   Updated: 2022/06/03 15:20:39 by arowe            ###   ########.fr       */
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

void	*odd_philo(void *data)
{
	t_philo			*philo;
	struct timeval	t;
	

	philo = (t_philo *)data;
	pthread_mutex_lock(philo->fork);
	gettimeofday(&t, NULL);
	printf("%dms philosopher %d has taken a fork\n", (t.tv_sec - philo->start_time->tv_sec) * 1000, philo->num);
	printf("%dms philosopher %d is eating\n", (t.tv_sec - philo->start_time->tv_sec) * 1000, philo->num);
	usleep(philo->time_to_eat * 1000);
	gettimeofday(&t, NULL);
	printf("%dms philosopher %d is sleeping\n", (t.tv_sec - philo->start_time->tv_sec) * 1000, philo->num);
	usleep(philo->time_to_sleep * 1000);
}