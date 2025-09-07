/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 08:25:00 by mohchaib          #+#    #+#             */
/*   Updated: 2025/09/07 11:07:42 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ft_sleep(unsigned int milisec)
{
	struct timeval	start;
	struct timeval	current;
	unsigned int	elapsed;

	gettimeofday(&start, NULL);
	while (1)
	{
		gettimeofday(&current, NULL);
		elapsed = (current.tv_sec - start.tv_sec) * 1000
			+ (current.tv_usec - start.tv_usec) / 1000;
		if (elapsed >= milisec)
			break ;
		usleep(50);
	}
}

void	print_error_msg(void)
{
	printf("invalid number of arguments\n"
		"USAGE : [number_of_philosophers]"
		" [time_to_die] [time_to_eat][time_to_sleep]"
		" [number_of_times_each_philosopher_must_eat]\n");
}

void	start_simulation(t_data *data, t_philo *philos, t_args *args)
{
	struct timeval	tv;
	int				i;

	gettimeofday(&tv, NULL);
	data->start_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	i = 0;
	while (i < args->philo_count)
	{
		pthread_mutex_lock(&data->meal_lock);
		philos[i].last_meal = data->start_time;
		pthread_mutex_unlock(&data->meal_lock);
		i++;
	}
	pthread_mutex_unlock(&data->ready_mutex);
}

void	cleanup(t_data *data, t_philo *philos, int created_count, t_args *args)
{
	int	i;

	data->someone_died = 1;
	i = 0;
	while (i < created_count)
		pthread_join(philos[i++].thread, NULL);
	pthread_mutex_unlock(&data->ready_mutex);
	i = 0;
	while (i < args->philo_count)
		pthread_mutex_destroy(&data->forks[i++]);
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->meal_lock);
	pthread_mutex_destroy(&data->death_lock);
	pthread_mutex_destroy(&data->ready_mutex);
	free(data->forks);
	free(philos);
}

int	main(int argc, char **argv)
{
	t_philo			*philos;
	t_args			args;
	t_data			data;

	if (argc < 5 || argc > 6)
		return (print_error_msg(), 1);
	if (!init_args(&args, argc, argv))
		return (1);
	philos = malloc(sizeof(t_philo) * args.philo_count);
	if (!philos)
		return (perror("malloc failed"), 1);
	if (!init_data(args, &data))
		return (free(philos), 1);
	setup_philos(&data, philos);
	launch_threads(args, data, philos);
	start_simulation(&data, philos, &args);
	monitor_routine(&data, philos);
	cleanup(&data, philos, args.philo_count, &args);
	return (0);
}
