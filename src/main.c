/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macaruan <macaruan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:28:40 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/20 18:21:42 by macaruan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

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

static int	handle_error(t_data *data, const char *msg)
{
	clean(data);
	write(2, msg, ft_strlen(msg));
	return (1);
}

static int	init_all(t_data *data)
{
	if (init_mutexes(data) != 0)
		return (handle_error(data, "Error: mutex init fail\n"));
	if (init_philos(data) != 0)
		return (handle_error(data, "Error: philos init fail\n"));
	if (start_threads(data) != 0)
		return (handle_error(data, "Error: thread creation fail\n"));
	if (start_monitor(data) != 0)
	{
		set_stop(data, 1);
		join_threads(data);
		return (handle_error(data, "Error: monitor creation fail\n"));
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (parse_args(&data, ac, av) != 0)
	{
		write(2, "Error: invalid args\n", 20);
		return (1);
	}
	if (init_all(&data) != 0)
		return (1);
	join_threads(&data);
	pthread_join(data.monitor_thread, NULL);
	clean(&data);
	return (0);
}
