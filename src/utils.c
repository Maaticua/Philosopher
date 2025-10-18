/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maato <maato@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:28:47 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/18 15:17:48 by maato            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != 0)
		return (-1);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(int duration)
{
	long	start;

	start = get_time();
	while ((get_time() - start) < duration)
		usleep(500);
}

void	print_state(t_philo *philo, const char *msg)
{
	long	timestamp;

	pthread_mutex_lock(&philo->data->print_mutex);
	if (!philo->data->stop)
	{
		timestamp = get_time() - philo->data->start_time;
		printf("%ld %d %s\n", timestamp, philo->id, msg);
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
}

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_state(philo, "took a fork");
		pthread_mutex_lock(philo->left_fork);
		print_state(philo, "took a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_state(philo, "took a fork");
		pthread_mutex_lock(philo->right_fork);
		print_state(philo, "took a fork");
	}
}

void	release_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}
int check_stop(t_data *data)
{
	int result;
	pthread_mutex_lock(&data->stop_mutex);
	result = data->stop;
	pthread_mutex_unlock(&data->stop_mutex);
	return(result);
}
void set_stop(t_data *data, int value)
{
	pthread_mutex_lock(&data->stop_mutex);
	data->stop = value;
	pthread_mutex_unlock(&data->stop_mutex);
}
