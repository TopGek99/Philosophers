/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arowe <arowe@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 14:55:17 by arowe             #+#    #+#             */
/*   Updated: 2022/06/02 16:11:04 by arowe            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// args[0] = number_of_philosophers
// args[1] = time_to_die
// args[2] = time_to_eat
// args[3] = time_to_sleep
// args[4] = number_of_times_each_philosopher_must_eat (-1 if not given)
int	main(int argc, char *argv[])
{
	pthread_t	philo_thread;
	int			args[5];
	int			i;

	if (argc < 5 || argc > 6)
	{
		printf("Error\n");
		return (0);
	}
	i = 0;
	while (argv[i + 1])
	{
		args[i] = ph_atoi(argv[i + 1]);
		i++;
	}
	if (argc == 5)
		args[4] = -1;
	i = 0;
	while (i < args[0])
	{
		pthread_create(&philo, NULL, create_philo, );
	}
	return (0);
}
