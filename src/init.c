/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maato <maato@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:28:37 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/15 11:00:46 by maato            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int is_val_nbr(const char *str)
{
	int i;

	if (!str || !str[0])
		return(0);
	i = 0;
	if (str[i] == '+')
		i++;
	if (!str[i])
		return(0);
	while(str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int ft_atoi(const char *str)
{
	long nb;

	nb = 0;
	while (*str >= '0' && *str <= '9')
		nb = nb * 10 + (*str++ - '0');
	return ((int)nb);
}

int parse_args(t_data *data, int argc, char **argv)
{
	int i;

	if (argc != 5 && argc != 6)
		return (1);
	i = 1;
	while(i < argc)
	{
		if (!is_val_nbr(argv[i]))
			return (1);
		i++;
	}
	data->nb_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->max_meals = ft_atoi(argv[5]);
	else
		data->max_meals = -1;
	data->stop = 0;
	if (data->nb_philo <= 0 || data->time_to_die <= 0 || data->time_to_eat <= 0 || data->time_to_sleep <= 0 || (argc == 6 && data->max_meals <= 0))
		return (1);
	return (0);
}

int init_mutexes(t_data *data)
{
	int i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (!data->forks)
		return (1);
	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (1);
		i++;
	}
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (1);
	return (0);
}

int init_philos(t_data *data)
{
	int i;

	data->philos = malloc(sizeof(t_philo) * data->nb_philo);
	if (!data->philos)
		return (1);
	i = 0;
	while (i < data->nb_philo)
	{
		data->philos[i].id = i + 1;
		data->philos[i].nb_meals = 0;
		data->philos[i].last_meal = 0;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->nb_philo];
		data->philos[i].data = data;

		if (pthread_mutex_init(&data->philos[i].meal_mutex, NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}
void clean(t_data *data)
{
	int i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philos[i].meal_mutex);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	free(data->forks);
	free(data->philos);
}
