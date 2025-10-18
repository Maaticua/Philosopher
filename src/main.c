/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maato <maato@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:28:40 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/17 12:45:22 by maato            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static void	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
}

int	main(int ac, char **av)
{
	t_data	data;

	if (parse_args(&data, ac, av) != 0)
		return (write(2, "Error: invalid args\n", 21), 1);
	if (init_mutexes(&data) != 0)
	{
		clean(&data);
		return (write(2, "Error: mutex init fail\n", 24), 1);
	}
	if (init_philos(&data) != 0)
	{
		clean(&data);
		return (write(2, "Error: philos init fail\n", 25), 1);
	}
	if (start_threads(&data) != 0)
	{
		clean(&data);
		return (write(2, "Error: thread creation fail\n", 29), 1);
	}
	if (start_monitor(&data) != 0)
	{
		data.stop = 1;
		join_threads(&data);
		clean(&data);
		return (write(2, "Error: monitor creation fail\n", 29), 1);
	}
	join_threads(&data);
	pthread_join(data.monitor_thread, NULL);
	clean(&data);
	return (0);
}
