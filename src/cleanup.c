/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macaruan <macaruan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 16:47:18 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/20 16:57:51 by macaruan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static void	clean_philos(t_data *data)
{
	int	i;

	if (!data->philos)
		return ;
	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_destroy(&data->philos[i].meal_mutex);
		i++;
	}
	free(data->philos);
	data->philos = NULL;
}

static void	clean_forks(t_data *data)
{
	int	i;

	if (!data->forks)
		return ;
	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	free(data->forks);
	data->forks = NULL;
}

void	clean(t_data *data)
{
	if (!data)
		return ;
	clean_philos(data);
	clean_forks(data);
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->stop_mutex);
}
