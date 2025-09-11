/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 08:24:57 by mohchaib          #+#    #+#             */
/*   Updated: 2025/09/11 10:41:12 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	die_philo(t_philo *philo)
{
	long long	time_stamp;

	pthread_mutex_lock(&philo->data->death_lock);
	if (!philo->data->someone_died)
	{
		philo->data->someone_died = 1;
		pthread_mutex_unlock(&philo->data->death_lock);
		pthread_mutex_lock(&philo->data->print_lock);
		time_stamp = current_timestamp() - philo->data->start_time;
		print_death(time_stamp, philo->id);
		pthread_mutex_unlock(&philo->data->print_lock);
	}
	else
		pthread_mutex_unlock(&philo->data->death_lock);
	return ;
}

bool	init_args(t_args *args, int argc, char **argv)
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
		return (ft_write_error("invalid number of philos\n"), false);
	if (args->philo_count > 200)
		return (ft_write_error("too many philosophers (max 200)\n"), false);
	if (args->time_to_die < 60)
		return (ft_write_error("time_to_die must be at least 60ms\n"), false);
	if (args->time_to_eat < 60)
		return (ft_write_error("time_to_eat must be at least 60ms\n"), false);
	if (args->time_to_sleep < 60)
		return (ft_write_error("time_to_sleep must be at least 60ms\n"), false);
	if (argc == 6 && args->must_eat_count < 0)
		return (ft_write_error("invalid number for must_eat_count\n"), false);
	return (true);
}

bool	init_data(t_args args, t_data *data)
{
	int	i;

	data->args = args;
	data->start_time = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * args.philo_count);
	if (!data->forks)
		return (ft_write_error("malloc failed\n"), false);
	i = 0;
	while (i < args.philo_count)
		pthread_mutex_init(&data->forks[i++], NULL);
	pthread_mutex_init(&data->print_lock, NULL);
	pthread_mutex_init(&data->meal_lock, NULL);
	pthread_mutex_init(&data->death_lock, NULL);
	pthread_mutex_init(&data->ready_mutex, NULL);
	pthread_mutex_lock(&data->ready_mutex);
	data->someone_died = 0;
	data->simulation_complete = 0;
	return (true);
}

void	setup_philos(t_data *data, t_philo *philos)
{
	int	i;

	i = 0;
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

bool	launch_threads(t_args args, t_data data, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < args.philo_count)
	{
		if (pthread_create(&philos[i].thread, NULL,
				philosopher_routine, &philos[i]) != 0)
			return (cleanup(&data, philos, i, &args), false);
		i++;
	}
	return (true);
}
