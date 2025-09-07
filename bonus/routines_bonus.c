/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 08:24:50 by mohchaib          #+#    #+#             */
/*   Updated: 2025/09/07 18:29:34 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	wait_for_processes(t_args args, t_data data)
{
	int		i;
	int		status;
	int		completed_philosophers;
	pid_t	finished_pid;

	// If must_eat_count is specified, we need to handle meal completion
	if (args.must_eat_count != -1)
	{
		completed_philosophers = 0;
		
		// Create a separate thread/logic to monitor meal completion
		// For now, we'll use a simpler approach: wait for first process to exit
		// and check if all completed or someone died
		
		finished_pid = waitpid(-1, &status, 0);
		if (finished_pid > 0)
		{
			// If someone died (exit status 1), kill all others
			if (WEXITSTATUS(status) == 1)
			{
				i = 0;
				while (i < args.philo_count)
				{
					if (data.philosophers[i] != finished_pid)
						kill(data.philosophers[i], SIGTERM);
					i++;
				}
			}
			else
			{
				// Someone finished eating, check if all are done
				// Try to collect meal completion signals
				while (completed_philosophers < args.philo_count)
				{
					if (sem_trywait(data.meal_complete) == 0)
						completed_philosophers++;
					else
						break; // No more signals available
				}
				
				// If all completed, kill remaining processes gently
				if (completed_philosophers >= args.philo_count)
				{
					i = 0;
					while (i < args.philo_count)
					{
						if (data.philosophers[i] != finished_pid)
							kill(data.philosophers[i], SIGTERM);
						i++;
					}
				}
			}
			
			// Wait for all remaining processes
			while (waitpid(-1, &status, 0) > 0)
				;
		}
	}
	else
	{
		// No meal limit - just wait for first death
		if (waitpid(-1, &status, 0) > 0)
		{
			// Kill all remaining processes
			i = 0;
			while (i < args.philo_count)
			{
				kill(data.philosophers[i], SIGTERM);
				i++;
			}
			
			// Wait for all processes to finish
			while (waitpid(-1, &status, 0) > 0)
				;
		}
	}
}

void	*death_monitor(void *arg)
{
	t_philo		*philo;
	long long	current_time;
	long long	last_meal_time;
	int			should_stop;

	philo = (t_philo *)arg;
	while (1)
	{
		// Check if we should stop
		pthread_mutex_lock(&philo->meal_mutex);
		should_stop = philo->should_stop;
		pthread_mutex_unlock(&philo->meal_mutex);
		
		if (should_stop)
			break;
		
		current_time = current_timestamp();
		
		// Safely read last_meal time using pthread mutex
		pthread_mutex_lock(&philo->meal_mutex);
		last_meal_time = philo->last_meal;
		pthread_mutex_unlock(&philo->meal_mutex);
		
		if (current_time - last_meal_time > philo->data->args.time_to_die)
		{
			die_philo(philo);
			return (NULL);
		}
		usleep(1000);  // Check every 1ms to avoid CPU overload
	}
	return (NULL);
}

void	philosopher_routine(t_philo *philo)
{
	pthread_t	monitor_thread;

	philo->should_stop = 0;
	
	// Special case for single philosopher - no death monitor needed
	if (philo->data->args.philo_count == 1)
	{
		print_state("is thinking", philo);
		ft_sleep(philo->data->args.time_to_die);
		
		// Set should_stop to prevent any race conditions
		pthread_mutex_lock(&philo->meal_mutex);
		if (!philo->should_stop)
		{
			philo->should_stop = 1;
			pthread_mutex_unlock(&philo->meal_mutex);
			
			sem_wait(philo->data->death_print);
			sem_wait(philo->data->writing);
			printf("%lld %d died\n", current_timestamp() - philo->data->start_time, philo->id);
			sem_post(philo->data->writing);
		}
		else
		{
			pthread_mutex_unlock(&philo->meal_mutex);
		}
		exit(1);
	}
	
	// For multiple philosophers, create death monitor
	if (pthread_create(&monitor_thread, NULL, death_monitor, philo) != 0)
		return (printf("pthread_create failed\n"), exit(1));
	
	while (1)
	{
		pthread_mutex_lock(&philo->meal_mutex);
		if (philo->should_stop)
		{
			pthread_mutex_unlock(&philo->meal_mutex);
			break;
		}
		pthread_mutex_unlock(&philo->meal_mutex);
		
		think_philo(philo);
		
		pthread_mutex_lock(&philo->meal_mutex);
		if (philo->should_stop)
		{
			pthread_mutex_unlock(&philo->meal_mutex);
			break;
		}
		pthread_mutex_unlock(&philo->meal_mutex);
		
		take_forks(philo);
		
		pthread_mutex_lock(&philo->meal_mutex);
		if (philo->should_stop)
		{
			pthread_mutex_unlock(&philo->meal_mutex);
			release_forks(philo);
			break;
		}
		pthread_mutex_unlock(&philo->meal_mutex);
		
		eat_philo(philo);
		release_forks(philo);
		
		// Check if this philosopher has eaten enough
		pthread_mutex_lock(&philo->meal_mutex);
		if (philo->data->args.must_eat_count != -1 && 
			philo->meals_eaten >= philo->data->args.must_eat_count)
		{
			// Signal that this philosopher is done eating
			sem_post(philo->data->meal_complete);
			philo->should_stop = 1;
			pthread_mutex_unlock(&philo->meal_mutex);
			break;
		}
		
		if (philo->should_stop)
		{
			pthread_mutex_unlock(&philo->meal_mutex);
			break;
		}
		pthread_mutex_unlock(&philo->meal_mutex);
		
		sleep_philo(philo);
	}
	
	pthread_mutex_lock(&philo->meal_mutex);
	philo->should_stop = 1;
	pthread_mutex_unlock(&philo->meal_mutex);
	pthread_join(monitor_thread, NULL);
	
	// Exit with status 0 if finished eating, 1 if died
	exit(0);
}

void	die_philo(t_philo *philo)
{
	long long	timestamp;

	// Prevent multiple threads from calling die_philo simultaneously
	pthread_mutex_lock(&philo->meal_mutex);
	if (philo->should_stop)
	{
		pthread_mutex_unlock(&philo->meal_mutex);
		return;  // Already dying, exit quietly
	}
	philo->should_stop = 1;  // Prevent further actions
	pthread_mutex_unlock(&philo->meal_mutex);
	
	sem_wait(philo->data->death_print);
	sem_wait(philo->data->writing);
	timestamp = current_timestamp() - philo->data->start_time;
	printf("%lld %d died\n", timestamp, philo->id);
	sem_post(philo->data->writing);
	exit(1);
}

void	cleanup(t_data *data, t_philo *philos)
{
	int	i;
	
	i = 0;
	while (i < data->args.philo_count)
	{
		pthread_mutex_destroy(&philos[i].meal_mutex);
		i++;
	}
	sem_close(data->forks);
	sem_close(data->writing);
	sem_close(data->death_check);
	sem_close(data->death_print);
	sem_close(data->meal_complete);
	sem_unlink("/forks_sem");
	sem_unlink("/writing_sem");
	sem_unlink("/death_check_sem");
	sem_unlink("/death_print_sem");
	sem_unlink("/meal_complete_sem");
	free(data->philosophers);
	free(philos);
}
