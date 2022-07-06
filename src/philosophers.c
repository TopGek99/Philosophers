/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 14:55:14 by arowe             #+#    #+#             */
/*   Updated: 2022/07/06 10:44:15 by alex             ###   ########.fr       */
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

void	join_all(t_philo *philos, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->amount_of_philo)
	{
		pthread_join(philos[i].philthread, NULL);
		pthread_join(philos[i].deaththread, NULL);
		i++;
	}
}
