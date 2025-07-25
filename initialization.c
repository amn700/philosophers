#include "philosophers.h"

bool    init_args(t_args *args, int argc, char **argv)
{
	args->philo_count = ft_atoi(argv[1]);
	args->time_to_die = ft_atoi(argv[2]);
	args->time_to_eat = ft_atoi(argv[3]);
	args->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		args->must_eat_count = ft_atoi(argv[5]);
	else
		args->must_eat_count = -1;

	if (args->philo_count <= 0)
		return (printf("invalid number of philos\n"), false);
	if (args->time_to_die < 0)
		return (printf("invalid number for time_to_die\n"), false);
	if (args->time_to_eat < 0)
		return (printf("invalid number for time_to_eat\n"), false);
	if (args->time_to_sleep < 0)
		return (printf("invalid number for time_to_sleep\n"), false);
	if (argc == 6 && args->must_eat_count < 0)
		return (printf("invalid number for must_eat_count\n"), false);
	return (true);
}

bool    init_data(t_args args, t_data *data)
{
	int             i;

	data->args = args;
	data->start_time = 0;
	data->ready_status = false;
	data->forks = malloc(sizeof(pthread_mutex_t) * args.philo_count);
	if (!data->forks)
		return (perror("malloc failed"), false);
	i = 0;
	while (i < args.philo_count)
		pthread_mutex_init(&data->forks[i++], NULL);
	pthread_mutex_init(&data->print_lock, NULL);
	pthread_mutex_init(&data->meal_lock, NULL);
	pthread_mutex_init(&data->ready_mutex, NULL);
	pthread_mutex_lock(&data->ready_mutex);  // Start locked!
	data->someone_died = 0;
	return (true);
}

void setup_philos(t_data *data, t_philo *philos)
{
	int i = 0;
	while (i < data->args.philo_count)
	{
		philos[i].left_fork = &data->forks[i];
		philos[i].right_fork = &data->forks[(i + 1) % data->args.philo_count];
		philos[i].id = i + 1;
		philos[i].data = data;
		philos[i].times_eaten = 0;
		philos[i].last_meal = data->start_time;
		i++;
	}
}
