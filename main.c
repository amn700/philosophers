#include "philosophers.h"

void	monitor_routine(t_data *data, t_philo *philos)
{
	long long	diff;
	long long	now;
    int			i;
	int 		full_count;

	while (!data->someone_died)
	{
		i = 0;
		full_count = 0;
		while (i < data->args.philo_count)
		{
			now = current_timestamp();
			diff = now - philos[i].last_meal;
			if (diff > data->args.time_to_die)
				return die_philo(&philos[i]);
			if (philos[i].times_eaten == philos->data->args.must_eat_count)
				full_count++;
			i++;
		}
		if (data->args.must_eat_count > 0 && full_count == data->args.philo_count)
			return ; // everyone finished
		usleep(1000);
	}

}

void *philosopher_routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;

	if (philo->id % 2 == 0)
		usleep(1000);
	if (philo->data->args.philo_count == 1)
	{
		think_philo(philo);
		usleep(philo->data->args.time_to_die);
		return (NULL);
	}
	while (!philo->data->someone_died)
	{
		think_philo(philo);
		take_forks(philo);
		eat_philo(philo);
		release_forks(philo);
		if (philo->data->args.must_eat_count != -1 && philo->times_eaten >= philo->data->args.must_eat_count)
			return (NULL);
		sleep_philo(philo);
	}
	return NULL;
}

void print_error_msg(void)
{
	printf("invalid number of arguments\n"
	"USAGE : [number_of_philosophers]"
	" [time_to_die] [time_to_eat][time_to_sleep]"
	" [number_of_times_each_philosopher_must_eat]\n");
}

int main (int argc, char **argv)
{
    t_philo			*philos;
    t_args			args;
    t_data			data;
    int				i;

    if (argc < 5 || argc > 6)
        return (print_error_msg(), 1);
    if (!init_args(&args, argc, argv))
		return (1);
    philos = malloc(sizeof(t_philo) * args.philo_count);
    if (!philos)
        return (perror("malloc failed"), 1);
    init_data(args, philos, &data);
	setup_philos(&data, philos);
	i = 0;
	while (i < args.philo_count)
	{
		pthread_create(&philos[i].thread, NULL, philosopher_routine, &philos[i]);
		i++;
	}
	monitor_routine(&data, philos);
    i = 0;
	while (i < args.philo_count)
		pthread_join(philos[i++].thread, NULL);
}
