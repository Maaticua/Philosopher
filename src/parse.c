/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macaruan <macaruan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:58:32 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/20 16:09:42 by macaruan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	is_val_nbr(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	if (str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	ft_atoi(const char *str)
{
	long	nb;

	nb = 0;
	while (*str >= '0' && *str <= '9')
		nb = nb * 10 + (*str++ - '0');
	return ((int)nb);
}

static int	validate_args(t_data *data, int argc)
{
	if (data->nb_philo <= 0 || data->time_to_die <= 0)
		return (1);
	if (data->time_to_eat <= 0 || data->time_to_sleep <= 0)
		return (1);
	if (argc == 6 && data->max_meals <= 0)
		return (1);
	return (0);
}

static void assign_args(t_data *data, int ac, char **av)
{
	data->nb_philo = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		data->max_meals = ft_atoi(av[5]);
	else
		data->max_meals = -1;
	data->stop = 0;
}

int parse_args(t_data *data,int ac, char **av)
{
	int i;

	if (ac != 5 && ac != 6)
		return (1);
	i = 1;
	while(i < ac)
	{
		if (!is_val_nbr(av[i]))
			return (1);
		i++;
	}
	assign_args(data, ac, av);
	return (validate_args(data, ac));
}
