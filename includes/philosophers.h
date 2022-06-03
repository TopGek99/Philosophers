/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arowe <arowe@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 14:55:20 by arowe             #+#    #+#             */
/*   Updated: 2022/06/03 15:07:20 by arowe            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

typedef struct s_philo {
	int				num;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				amount_to_eat;
	pthread_mutex_t	*fork;
	struct timeval	*start_time;
}	t_philo;

int		ph_atoi(const char *str);
void	*odd_philo(void *nothing);

#endif