/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 08:24:44 by mohchaib          #+#    #+#             */
/*   Updated: 2025/09/07 20:47:30 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

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
		return (printf("invalid number of philos\n"), false);
	if (args->philo_count > 200)
		return (printf("too many philosophers (max 200)\n"), false);
	if (args->time_to_die < 60)
		return (printf("time_to_die must be at least 60ms\n"), false);
	if (args->time_to_eat < 60)
		return (printf("time_to_eat must be at least 60ms\n"), false);
	if (args->time_to_sleep < 60)
		return (printf("time_to_sleep must be at least 60ms\n"), false);
	if (argc == 6 && args->must_eat_count < 0)
		return (printf("invalid number for must_eat_count\n"), false);
	return (true);
}

bool	init_data(t_args args, t_data *data)
{
	data->args = args;
	data->start_time = 0;
	data->philosophers = malloc(sizeof(pid_t) * args.philo_count);
	if (!data->philosophers)
		return (printf("malloc failed for philosophers array\n"), false);
	sem_unlink("/forks_sem");
	sem_unlink("/writing_sem");
	sem_unlink("/death_check_sem");
	sem_unlink("/death_print_sem");
	data->forks = sem_open("/forks_sem", O_CREAT, 0644, args.philo_count);
	if (data->forks == SEM_FAILED)
		return (printf("sem_open failed for forks\n"), false);
	data->writing = sem_open("/writing_sem", O_CREAT, 0644, 1);
	if (data->writing == SEM_FAILED)
		return (printf("sem_open failed for writing\n"), false);
	data->death_check = sem_open("/death_check_sem", O_CREAT, 0644, 1);
	if (data->death_check == SEM_FAILED)
		return (printf("sem_open failed for death_check\n"), false);
	data->death_print = sem_open("/death_print_sem", O_CREAT, 0644, 1);
	if (data->death_print == SEM_FAILED)
		return (printf("sem_open failed for death_print\n"), false);
	return (true);
}

void	setup_philos(t_data *data, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < data->args.philo_count)
	{
		philos[i].id = i + 1;
		philos[i].last_meal = 0;
		philos[i].meals_eaten = 0;
		philos[i].should_stop = 0;
		philos[i].data = data;
		if (pthread_mutex_init(&philos[i].meal_mutex, NULL) != 0)
		{
			printf("Failed to initialize meal mutex\n");
			exit(1);
		}
		i++;
	}
}

void	multy_process_management(t_data *data, t_philo *philos, t_args *args)
{
	int	i;

	i = 0;
	while (i < args->philo_count)
	{
		data->philosophers[i] = fork();
		if (data->philosophers[i] == 0)
		{
			pthread_mutex_lock(&philos[i].meal_mutex);
			philos[i].last_meal = data->start_time;
			pthread_mutex_unlock(&philos[i].meal_mutex);
			philosopher_routine(&philos[i]);
			exit(0);
		}
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_philo	*philos;
	t_args	args;
	t_data	data;

	if (argc != 5 && argc != 6)
		return (print_error_msg(), 1);
	if (!init_args(&args, argc, argv))
		return (1);
	if (!init_data(args, &data))
		return (1);
	philos = malloc(sizeof(t_philo) * args.philo_count);
	if (!philos)
		return (free(data.philosophers), printf("malloc failed\n"), 1);
	setup_philos(&data, philos);
	data.start_time = current_timestamp();
	multy_process_management(&data, philos, &args);
	wait_for_processes(args, data);
	cleanup(&data, philos);
	return (0);
}
