/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mutex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macaruan <macaruan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 16:11:42 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/20 16:58:09 by macaruan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static void	cleanup_forks(t_data *data, int count)
{
	while (--count >= 0)
		pthread_mutex_destroy(&data->forks[count]);
	free(data->forks);
}

static int	init_forks(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (!data->forks)
		return (1);
	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			cleanup_forks(data, i);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	init_print_mutex(t_data *data)
{
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
	{
		cleanup_forks(data, data->nb_philo);
		return (1);
	}
	return (0);
}

static int	init_stop_mutex(t_data *data)
{
	if (pthread_mutex_init(&data->stop_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->print_mutex);
		cleanup_forks(data, data->nb_philo);
		return (1);
	}
	return (0);
}

int	init_mutexes(t_data *data)
{
	if (init_forks(data) != 0)
		return (1);
	if (init_print_mutex(data) != 0)
		return (1);
	if (init_stop_mutex(data) != 0)
		return (0);
	return (0);
}
