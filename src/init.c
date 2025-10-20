/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macaruan <macaruan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:28:37 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/20 17:05:52 by macaruan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static void	init_philo_data(t_philo *philo, t_data *data, int i)
{
	philo->id = i + 1;
	philo->nb_meals = 0;
	philo->last_meal = 0;
	philo->left_fork = &data->forks[i];
	philo->right_fork = &data->forks[(i + 1) % data->nb_philo];
	philo->data = data;
}

static void	cleanup_philo_mutexes(t_data *data, int count)
{
	while (--count >= 0)
		pthread_mutex_destroy(&data->philos[count].meal_mutex);
	free(data->philos);
}

int	init_philos(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->nb_philo);
	if (!data->philos)
		return (1);
	i = 0;
	while (i < data->nb_philo)
	{
		init_philo_data(&data->philos[i], data, i);
		if (pthread_mutex_init(&data->philos[i].meal_mutex, NULL) != 0)
		{
			cleanup_philo_mutexes(data, i);
			return (1);
		}
		i++;
	}
	return (0);
}
