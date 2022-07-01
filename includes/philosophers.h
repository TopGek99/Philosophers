/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 14:55:20 by arowe             #+#    #+#             */
/*   Updated: 2022/06/06 19:43:20 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdbool.h>

typedef struct s_thread_data {
	int						amount_of_philo;
	int						time_to_die;
	int						time_to_eat;
	int						time_to_sleep;
	int						amount_to_eat;
	struct timeval			start_time;
	pthread_mutex_t			*forks;
	pthread_mutex_t			lock_print;
}	t_data;

typedef struct s_philo {
	int				num;
	bool			dead;
	pthread_mutex_t	*avail_forks[2];
	pthread_t		philthread;
	t_data			*data;
	struct s_philo	*next;
}	t_philo;

int		ph_atoi(const char *str);
void	*do_stuff_philo(void *input);

#endif