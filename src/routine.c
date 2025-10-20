/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macaruan <macaruan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:28:29 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/20 19:12:55 by macaruan         ###   ########.fr       */
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

void	*handle_single_philo(t_philo *philo)
{
	print_state(philo, "has taken a fork");
	while (!check_stop(philo->data))
		ft_usleep(100);
	return (NULL);
}

void	init_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->meal_mutex);
	if (philo->id % 2 == 0)
	{
		print_state(philo, "is thinking");
		ft_usleep(philo->data->time_to_eat);
	}
}

int	philo_eat(t_philo *philo)
{
	if (check_stop(philo->data))
		return (1);
	take_forks(philo);
	if (check_stop(philo->data))
	{
		release_forks(philo);
		return (1);
	}
	print_state(philo, "is eating");
	ft_usleep(philo->data->time_to_eat);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = get_time();
	philo->nb_meals++;
	pthread_mutex_unlock(&philo->meal_mutex);
	release_forks(philo);
	return (0);
}

int	check_meal_limit(t_philo *philo)
{
	int	should_stop;

	pthread_mutex_lock(&philo->meal_mutex);
	should_stop = 0;
	if (philo->data->max_meals > 0 && philo->nb_meals >= philo->data->max_meals)
		should_stop = 1;
	pthread_mutex_unlock(&philo->meal_mutex);
	return (should_stop);
}
