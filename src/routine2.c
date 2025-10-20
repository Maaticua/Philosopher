/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macaruan <macaruan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 19:11:23 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/20 19:15:29 by macaruan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	philo_sleep_think(t_philo *philo)
{
	if (check_stop(philo->data))
		return (1);
	print_state(philo, "is sleeping");
	ft_usleep(philo->data->time_to_sleep);
	if (check_stop(philo->data))
		return (1);
	print_state(philo, "is thinking");
	return (0);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->nb_philo == 1)
		return (handle_single_philo(philo));
	init_philo_routine(philo);
	while (1)
	{
		if (philo_eat(philo))
			break ;
		if (check_meal_limit(philo))
			break ;
		if (philo_sleep_think(philo))
			break ;
	}
	return (NULL);
}
