#include "philosophers.h"

void	ft_sleep(unsigned int milisec)
{
	struct timeval start;
	struct timeval current;
	unsigned int elapsed;

	gettimeofday(&start, NULL);
	while (1)
	{
		gettimeofday(&current, NULL);
		elapsed = (current.tv_sec - start.tv_sec) * 1000
				+ (current.tv_usec - start.tv_usec) / 1000;
		if (elapsed >= milisec)
			break;
		usleep(50);  // Reduced for better responsiveness
	}
}

void	monitor_routine(t_data *data, t_philo *philos)
{
	long long	diff;
	long long	now;
    int			i;
	int 		full_count;

	while (!data->someone_died)
	{
		usleep(100);  // Very frequent checking for tight timing
		i = 0;
		full_count = 0;
		while (i < data->args.philo_count && !data->someone_died)
		{
			now = current_timestamp();
			diff = now - philos[i].last_meal;
			if (diff > data->args.time_to_die)
				return die_philo(&philos[i]);
			if (data->args.must_eat_count > 0 && philos[i].times_eaten >= data->args.must_eat_count)
				full_count++;
			i++;
		}
		if (data->args.must_eat_count > 0 && full_count == data->args.philo_count)
			return ;
	}

}

void *philosopher_routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;

	// Wait for all philosophers to be created before starting
	// Block until ready_mutex is unlocked by main thread
	pthread_mutex_lock(&philo->data->ready_mutex);
	pthread_mutex_unlock(&philo->data->ready_mutex);  // Immediately unlock
		
	if (philo->id % 2 == 0)
		usleep(100);  // Reduced from 300 for faster startup
	if (philo->data->args.philo_count == 1)
	{
		think_philo(philo);
		ft_sleep(philo->data->args.time_to_die);
		return (NULL);
	}
	while (!philo->data->someone_died)
	{
		think_philo(philo);
		if (philo->data->someone_died) break;
		
		take_forks(philo);
		if (philo->data->someone_died) break;
		
		eat_philo(philo);
		release_forks(philo);  // Always release after eating
		
		if (philo->data->args.must_eat_count != -1 && philo->times_eaten >= philo->data->args.must_eat_count)
			return (NULL);
		if (philo->data->someone_died) break;
		
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
    init_data(args, &data);
	setup_philos(&data, philos);
	i = 0;
	// Create all philosopher threads first
	while (i < args.philo_count)
	{
		pthread_create(&philos[i].thread, NULL, philosopher_routine, &philos[i]);
		i++;
	}
	
	// Now set the actual starting time
	struct timeval tv;
	gettimeofday(&tv, NULL);
	data.start_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	
	// Update all philosophers' last_meal with the actual start time
	i = 0;
	while (i < args.philo_count)
	{
		philos[i].last_meal = data.start_time;
		i++;
	}
	
	// Now signal all philosophers to start by unlocking the barrier
	pthread_mutex_unlock(&data.ready_mutex);
	
	monitor_routine(&data, philos);
    i = 0;
	while (i < args.philo_count)
		pthread_join(philos[i++].thread, NULL);
	
	// Cleanup
	i = 0;
	while (i < args.philo_count)
		pthread_mutex_destroy(&data.forks[i++]);
	pthread_mutex_destroy(&data.print_lock);
	pthread_mutex_destroy(&data.ready_mutex);
	free(data.forks);
	free(philos);
	return (0);
}
