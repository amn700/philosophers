/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchaib <mohchaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 08:24:50 by mohchaib          #+#    #+#             */
/*   Updated: 2025/09/07 11:57:13 by mohchaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	*death_monitor(void *arg)
{
	t_philo		*philo;
	long long	current_time;

	philo = (t_philo *)arg;
	while (!philo->should_stop)
	{
		current_time = current_timestamp();
		if (current_time - philo->last_meal > philo->data->args.time_to_die)
		{
			die_philo(philo);
			return (NULL);
		}
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
	while (!philo->should_stop)
	{
		think_philo(philo);
		if (philo->should_stop)
			break ;
		take_forks(philo);
		if (philo->should_stop) 
			return (release_forks(philo));
		eat_philo(philo);
		release_forks(philo);
		if (philo->data->args.must_eat_count != -1 && 
			philo->meals_eaten >= philo->data->args.must_eat_count)
			philo->should_stop = 1;
		if (philo->should_stop)
			break ;
		sleep_philo(philo);
	}
	philo->should_stop = 1;
	pthread_join(monitor_thread, NULL);
}

void	die_philo(t_philo *philo)
{
	long long	timestamp;

	sem_wait(philo->data->death_print);
	sem_wait(philo->data->writing);
	timestamp = current_timestamp() - philo->data->start_time;
	printf("%lld %d died\n", timestamp, philo->id);
	sem_post(philo->data->writing);
	exit(1);
}

void	cleanup(t_data *data, t_philo *philos)
{
	sem_close(data->forks);
	sem_close(data->writing);
	sem_close(data->death_check);
	sem_close(data->death_print);
	sem_unlink("/forks_sem");
	sem_unlink("/writing_sem");
	sem_unlink("/death_check_sem");
	sem_unlink("/death_print_sem");
	free(data->philosophers);
	free(philos);
}
