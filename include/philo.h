/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maato <maato@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:28:32 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/17 11:48:42 by maato            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

#include "utils.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
// # include "../libft/inc/libft.h"

typedef struct s_data t_data;

typedef struct s_philo
{
	int id;
	int nb_meals;
	long last_meal;
	pthread_mutex_t meal_mutex;
	pthread_t thread;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
	t_data *data;
} t_philo;

typedef struct s_data
{
	int nb_philo;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int max_meals;
	int stop;
	long start_time;
	pthread_mutex_t *forks;
	pthread_mutex_t print_mutex;
	t_philo *philos;
	pthread_t	monitor_thread;
} t_data;

// init.c
int init_mutexes(t_data *data);
int init_philos(t_data *data);
int parse_args(t_data *data, int argc, char **argv);
void clean(t_data *data);

// routine.c
int start_threads(t_data *data);
void *philo_routine(void *arg);

// monitor.c
void *monitor_routine(void *arg);
int start_monitor(t_data *data);

#endif
