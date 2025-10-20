/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macaruan <macaruan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 19:20:24 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/20 19:20:51 by macaruan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	check_stop(t_data *data)
{
	int	result;

	pthread_mutex_lock(&data->stop_mutex);
	result = data->stop;
	pthread_mutex_unlock(&data->stop_mutex);
	return (result);
}

void	set_stop(t_data *data, int value)
{
	pthread_mutex_lock(&data->stop_mutex);
	data->stop = value;
	pthread_mutex_unlock(&data->stop_mutex);
}
