/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maato <maato@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:28:43 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/15 10:46:48 by maato            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	start_monitor(t_data *data)
{
	pthread_t	monitor;

	if (pthread_create(&monitor, NULL, &monitor_routine, data) != 0)
		return (1);
	pthread_detach(monitor);
	return (0);
}
void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;
	long	now;
	int	all_ate;

	data = (t_data *)arg;
	while (!data->stop)
	{
		i = 0;
		all_ate = 1;
		while (i < data->nb_philo)
		{
			pthread_mutex_lock(&data->philos[i].meal_mutex);
			now = get_time();
			if ((now - data->philos[i].last_meal) > data->time_to_die)
			{
				pthread_mutex_unlock(&data->philos[i].meal_mutex);
				pthread_mutex_lock(&data->print_mutex);
				printf("%ld %d died\n", now - data->start_time, data->philos[i].id);
				pthread_mutex_unlock(&data->print_mutex);
				data->stop = 1;
				return (NULL);
			}
			if (data->max_meals > 0 && data->philos[i].nb_meals < data->max_meals)
				all_ate = 0;
			pthread_mutex_unlock(&data->philos[i].meal_mutex);
			i++;
		}
		if (data->max_meals > 0 && all_ate)
		{
			data->stop = 1;
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
