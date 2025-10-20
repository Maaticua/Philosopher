/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macaruan <macaruan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:28:29 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/20 15:01:28 by macaruan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	start_threads(t_data *data)
{
	int		i;
	long	start;
	t_philo	*philo;

	start = get_time();
	if (start == -1)
		return (1);
	data->start_time = start;
	i = 0;
	while (i < data->nb_philo)
	{
		philo = &data->philos[i];
		philo->last_meal = start;
		if (pthread_create(&philo->thread, NULL, &philo_routine, philo) != 0)
			return (1);
		i++;
	}
	return (0);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->nb_philo == 1)
	{
		print_state(philo, "has taken a fork");
		while (!check_stop(philo->data))
			ft_usleep(100);
		return (NULL);
	}
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->meal_mutex);
	if (philo->id % 2 == 0)
	{
		print_state(philo, "is thinking");
		ft_usleep(philo->data->time_to_eat);
	}
	while (1)
	{
		if (check_stop(philo->data))
			break ;
		take_forks(philo);
		if (check_stop(philo->data))
		{
			release_forks(philo);
			break ;
		}
		print_state(philo, "is eating");
		ft_usleep(philo->data->time_to_eat);
		pthread_mutex_lock(&philo->meal_mutex);
		philo->last_meal = get_time();
		philo->nb_meals++;
		pthread_mutex_unlock(&philo->meal_mutex);
		release_forks(philo);
		pthread_mutex_lock(&philo->meal_mutex);
		if (philo->data->max_meals > 0 && philo->nb_meals >= philo->data->max_meals)
		{
			pthread_mutex_unlock(&philo->meal_mutex);
			break;
		}
		pthread_mutex_unlock(&philo->meal_mutex);
		if (check_stop(philo->data))
			break ;
		print_state(philo, "is sleeping");
		ft_usleep(philo->data->time_to_sleep);
		if (check_stop(philo->data))
			break ;
		print_state(philo, "is thinking");
	}
	return (NULL);
}
