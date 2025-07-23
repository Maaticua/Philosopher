/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macaruan <macaruan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:28:29 by macaruan          #+#    #+#             */
/*   Updated: 2025/07/23 14:34:18 by macaruan         ###   ########.fr       */
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
	while (1)
	{
		if (philo->data->stop)
			break;
		take_forks(philo);
		print_state(philo, "is eating");
		philo->last_meal = get_time();
		ft_usleep(philo->data->time_to_eat);
		release_fork(philo);
		philo->nb_meals++;
		if (philo->data->max_meals > 0
			&& philo->nb_meals >= philo->data->max_meals)
			break;
		print_state(philo, "is sleeping");
		ft_usleep(philo->data->time_to_sleep);
		print_state(philo, "is thinking");
	}
	return (NULL);
}
