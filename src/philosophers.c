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
	return (((now.tv_sec * 1000) + now.tv_usec / 1000) - ((start.tv_sec * 1000) + start.tv_usec / 1000));
}

int	psleep(t_philo *p, int t, long death_timer)
{
	struct timeval time1;
	struct timeval time2;

	gettimeofday(&time1, NULL);
	while (1)
	{
		gettimeofday(&time2, NULL);
		if (get_timestamp(time2, time1) + death_timer >= p->data->time_to_die)
		{
			p->dead = true;
			return (get_timestamp(time2, time1));
		}
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

void	eat(t_philo *philo)
{
	struct timeval t;

	pthread_mutex_lock(philo->avail_forks[0]);
	pthread_mutex_lock(philo->avail_forks[1]);
	gettimeofday(&t, NULL);
	pthread_mutex_lock(&philo->data->lock_print);
	printf("%ldms philosopher %d has taken a fork\n", get_timestamp(t, philo->data->start_time), philo->num);
	printf("%ldms philosopher %d is eating\n", get_timestamp(t, philo->data->start_time), philo->num);
	pthread_mutex_unlock(&philo->data->lock_print);
	psleep(philo, philo->data->time_to_eat, 0);
	pthread_mutex_unlock(philo->avail_forks[0]);
	pthread_mutex_unlock(philo->avail_forks[1]);
}

void	philo_sleep(t_philo *philo)
{
	struct timeval t;

	gettimeofday(&t, NULL);
	pthread_mutex_lock(&philo->data->lock_print);
	printf("%ldms philosopher %d is sleeping\n", get_timestamp(t, philo->data->start_time), philo->num);
	pthread_mutex_unlock(&philo->data->lock_print);
	psleep(philo, philo->data->time_to_sleep, 0);
}

void	think(t_philo *philo)
{
	struct timeval t;

	gettimeofday(&t, NULL);
	pthread_mutex_lock(&philo->data->lock_print);
	printf("%ldms philosopher %d is thinking\n", get_timestamp(t, philo->data->start_time), philo->num);
	pthread_mutex_unlock(&philo->data->lock_print);
}

void	*do_stuff_philo(void *input)
{
	t_philo			*philo;

	philo = (t_philo *)input;
	if (philo->data->amount_of_philo == 1)
		return (one_philo_die(philo));
	while (1)
	{
		eat(philo);
		philo_sleep(philo);
		think(philo);
	}
	return (NULL);
}

		// gettimeofday(&temp_time, NULL);
		// if (time.tv_usec != 0)
		// {
		// 	death_timer += get_timestamp(temp_time, time);
		// 	if (death_timer >= philo->data->time_to_die)
		// 	{
		// 		pthread_mutex_lock(&philo->data->lock_print);
		// 		printf("%ldms philosopher %d has died\n", get_timestamp(temp_time, philo->data->start_time), philo->num);
		// 		pthread_mutex_unlock(&philo->data->lock_print);
		// 		join = philo;
		// 		// pthread_mutex_unlock(philo->avail_forks[0]);
		// 		// pthread_mutex_unlock(philo->avail_forks[1]);
		// 		int j = 0;
		// 		while (j < philo->data->amount_of_philo)
		// 		{
		// 			// printf("here\n");
		// 			// pthread_mutex_destroy(join->avail_forks[0]);
		// 			pthread_detach(join->next->philthread);
		// 			join = join->next;
		// 			j++;
		// 		}
		// 		break ;
		// 	}
		// }

		// if (philo->dead)
		// {
		// 	pthread_mutex_lock(&philo->data->lock_print);
		// 	printf("%ldms philosopher %d has died\n", get_timestamp(time, philo->data->start_time), philo->num);
		// 	pthread_mutex_unlock(&philo->data->lock_print);
		// 	join = philo;
		// 	while (pthread_mutex_destroy(join->avail_forks[0]) == 0)
		// 	{
		// 		printf("here\n");
		// 		join = join->next;
		// 	}
		// 	break ;
		// }
		
		
		// if (philo->dead)
		// {
		// 	pthread_mutex_lock(&philo->data->lock_print);
		// 	printf("%ldms philosopher %d has died\n", get_timestamp(time, philo->data->start_time), philo->num);
		// 	pthread_mutex_unlock(&philo->data->lock_print);
		// 	join = philo;
		// 	while (pthread_mutex_destroy(join->avail_forks[0]) == 0)
		// 	{
		// 		printf("here\n");
		// 		join = join->next;
		// 	}
		// 	break ;
		// }