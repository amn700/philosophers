/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 08:24:50 by mohchaib          #+#    #+#             */
/*   Updated: 2025/09/07 21:04:06 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	wait_for_processes(t_args args, t_data data)
{
	int		i;
	int		status;
	pid_t	finished_pid;

	finished_pid = waitpid(-1, &status, 0);
	if (finished_pid > 0)
	{
		i = 0;
		while (i < args.philo_count)
		{
			if (data.philosophers[i] != finished_pid)
				kill(data.philosophers[i], SIGTERM);
			i++;
		}
		while (waitpid(-1, &status, 0) > 0)
			;
	}
}

void	*death_monitor(void *arg)
{
	t_philo		*philo;
	long long	current_time;

	philo = (t_philo *)arg;
	while (1)
	{
		if (check_stop_condition(philo))
			break ;
		current_time = current_timestamp();
		if (check_death_condition(philo, current_time))
		{
			die_philo(philo);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

void	single_philo_routine(t_philo *philo)
{
	print_state("is thinking", philo);
	ft_sleep(philo->data->args.time_to_die);
	pthread_mutex_lock(&philo->meal_mutex);
	if (!philo->should_stop)
	{
		philo->should_stop = 1;
		pthread_mutex_unlock(&philo->meal_mutex);
		sem_wait(philo->data->writing);
		printf("%lld %d died\n", current_timestamp()
			- philo->data->start_time, philo->id);
		fflush(stdout);
		sem_post(philo->data->writing);
	}
	else
		pthread_mutex_unlock(&philo->meal_mutex);
	exit(1);
}

void	philosopher_routine(t_philo *philo)
{
	pthread_t	monitor_thread;

	philo->should_stop = 0;
	if (philo->data->args.philo_count == 1)
		single_philo_routine(philo);
	if (pthread_create(&monitor_thread, NULL, death_monitor, philo) != 0)
		return (printf("pthread_create failed\n"), exit(1));
	while (1)
	{
		execute_philosopher_cycle(philo);
		if (check_stop_condition(philo))
			break ;
		if (check_meal_completion(philo))
			break ;
		if (check_stop_condition(philo))
			break ;
		sleep_philo(philo);
	}
	pthread_mutex_lock(&philo->meal_mutex);
	philo->should_stop = 1;
	pthread_mutex_unlock(&philo->meal_mutex);
	pthread_join(monitor_thread, NULL);
	exit(0);
}

void	die_philo(t_philo *philo)
{
	long long	timestamp;

	sem_wait(philo->data->death_print);
	timestamp = current_timestamp() - philo->data->start_time;
	sem_wait(philo->data->writing);
	printf("%lld %d died\n", timestamp, philo->id);
	fflush(stdout);
	sem_post(philo->data->writing);
	exit(1);
}
