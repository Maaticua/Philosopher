/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macaruan <macaruan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:28:43 by macaruan          #+#    #+#             */
/*   Updated: 2025/07/24 14:14:03 by macaruan         ###   ########.fr       */
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

	data = (t_data *)arg;
	while (!data->stop)
	{
		i = 0;
		while (i < data->nb_philo)
		{
			now = get_time();
			if ((now - data->philos[i].last_meal) > data->time_to_die)
			{
				pthread_mutex_lock(&data->print_mutex);
				printf("%ld %d died\n", now - data->start_time, data->philos[i].id);
				pthread_mutex_unlock(&data->print_mutex);
				data->stop = 1;
				return (NULL);
			}
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
