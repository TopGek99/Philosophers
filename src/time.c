/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 09:33:40 by alex              #+#    #+#             */
/*   Updated: 2022/07/06 09:33:46 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	get_timestamp(struct timeval now, struct timeval start)
{
	return (((now.tv_sec * 1000) + now.tv_usec / 1000)
		- ((start.tv_sec * 1000) + start.tv_usec / 1000));
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
