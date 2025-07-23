#include "../include/philo.h"

int	main(int ac, char **av)
{
	t_data	data;

	if (parse_args(&data, ac, av) != 0)
		return (write(2, "Error: invalid args\n", 22), 1);
	if (init_mutexes(&data) != 0)
		return (write(2, "Error: mutex init fail\n",25), 1);
	if (init_philos(&data) != 0)
		return (write(2, "Error: philos init fail\n",26), 1);
	//thread
	//clean
	return (0);
}
