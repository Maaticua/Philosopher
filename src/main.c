/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macaruan <macaruan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:28:40 by macaruan          #+#    #+#             */
/*   Updated: 2025/07/24 11:55:26 by macaruan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"


static void	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
}
int	main(int ac, char **av)
{
	t_data	data;

	if (parse_args(&data, ac, av) != 0)
		return (write(2, "Error: invalid args\n", 22), 1);
	if (init_mutexes(&data) != 0)
		return (write(2, "Error: mutex init fail\n", 25), 1);
	if (init_philos(&data) != 0)
		return (write(2, "Error: philos init fail\n", 26), 1);
	if (start_threads(&data) != 0)
		return (write(2, "Error: thread creation fail\n", 30), 1);
	join_threads(&data);
	// clean
	return (0);
}
