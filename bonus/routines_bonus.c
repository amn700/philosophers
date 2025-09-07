/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 08:24:50 by mohchaib          #+#    #+#             */
/*   Updated: 2025/09/07 17:31:39 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	wait_for_processes(t_args args, t_data data)
{
	int	i;
	int	status;

	if (waitpid(-1, &status, 0) > 0)
	{
		i = 0;
		while (i < args.philo_count)
		{
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
	long long	last_meal_time;
	int			should_continue;

	philo = (t_philo *)arg;
	should_continue = 1;
	while (should_continue)
	{
		current_time = current_timestamp();
		pthread_mutex_lock(&philo->meal_mutex);
		last_meal_time = philo->last_meal;
		pthread_mutex_unlock(&philo->meal_mutex);
		
		if (current_time - last_meal_time > philo->data->args.time_to_die)
		{
			die_philo(philo);
			return (NULL);
		}
		
		pthread_mutex_lock(&philo->stop_mutex);
		should_continue = !philo->should_stop;
		pthread_mutex_unlock(&philo->stop_mutex);
		
		usleep(500);
	}
	return (NULL);
}

void	philosopher_routine(t_philo *philo)
{
	pthread_t	monitor_thread;

	philo->should_stop = 0;
	
	if (pthread_create(&monitor_thread, NULL, death_monitor, philo) != 0)
		return (printf("pthread_create failed\n"), exit(1));
	
	// Special case for single philosopher - just think and wait to die
	if (philo->data->args.philo_count == 1)
	{
		sem_wait(philo->data->writing);
		printf("0 %d is thinking\n", philo->id);
		sem_post(philo->data->writing);
		
		// Single philosopher can't eat (needs 2 forks, only 1 available)
		// So just wait to die without taking any forks
		int should_continue = 1;
		while (should_continue)
		{
			pthread_mutex_lock(&philo->stop_mutex);
			should_continue = !philo->should_stop;
			pthread_mutex_unlock(&philo->stop_mutex);
			if (should_continue)
				usleep(1000);
		}
		
		pthread_join(monitor_thread, NULL);
		exit(0);
	}
	
	int should_continue = 1;
	while (should_continue)
	{
		think_philo(philo);
		
		pthread_mutex_lock(&philo->stop_mutex);
		should_continue = !philo->should_stop;
		pthread_mutex_unlock(&philo->stop_mutex);
		if (!should_continue)
			break ;
			
		take_forks(philo);
		
		pthread_mutex_lock(&philo->stop_mutex);
		should_continue = !philo->should_stop;
		pthread_mutex_unlock(&philo->stop_mutex);
		if (!should_continue)
		{
			release_forks(philo);
			break ;
		}
		
		eat_philo(philo);
		release_forks(philo);
		
		if (philo->data->args.must_eat_count != -1 && 
			philo->meals_eaten >= philo->data->args.must_eat_count)
		{
			pthread_mutex_lock(&philo->stop_mutex);
			philo->should_stop = 1;
			pthread_mutex_unlock(&philo->stop_mutex);
		}
		
		pthread_mutex_lock(&philo->stop_mutex);
		should_continue = !philo->should_stop;
		pthread_mutex_unlock(&philo->stop_mutex);
		if (!should_continue)
			break ;
			
		sleep_philo(philo);
	}
	
	pthread_mutex_lock(&philo->stop_mutex);
	philo->should_stop = 1;
	pthread_mutex_unlock(&philo->stop_mutex);
	
	pthread_join(monitor_thread, NULL);
}

void	die_philo(t_philo *philo)
{
	long long	timestamp;

	pthread_mutex_lock(&philo->stop_mutex);
	if (!philo->should_stop)
	{
		philo->should_stop = 1;
		pthread_mutex_unlock(&philo->stop_mutex);
		
		sem_wait(philo->data->death_print);
		sem_wait(philo->data->writing);
		timestamp = current_timestamp() - philo->data->start_time;
		printf("%lld %d died\n", timestamp, philo->id);
		sem_post(philo->data->writing);
		exit(1);
	}
	pthread_mutex_unlock(&philo->stop_mutex);
}
