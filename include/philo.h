#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include "utils.h"
//# include "../libft/inc/libft.h"

typedef struct s_data t_data;

typedef struct  s_philo
{
	int				id;
	int				nb_meals;
	long			last_meal;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
}t_philo;

typedef struct s_data
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				max_meals;
	int				stop;
	long			start_time;
	pthread_mutex_t *forks;
	pthread_mutex_t print_mutex;
	t_philo			*philos;
}t_data;

//init.c
int init_mutexes(t_data *data);
int init_philos(t_data *data);
int parse_args(t_data *data, int argc, char **argv);

#endif
