/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 08:50:39 by mohchaib          #+#    #+#             */
/*   Updated: 2025/09/07 16:27:48 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	monitor_core(t_data *data, t_philo *philos, int *full_count)
{
	int			i;
	long long	now;
	long long	diff;
	int			times_eaten;

	i = 0;
	while (i < data->args.philo_count && !check_death(philos))
	{
		now = current_timestamp();
		pthread_mutex_lock(&data->meal_lock);
		diff = now - philos[i].last_meal;
		times_eaten = philos[i].times_eaten;
		pthread_mutex_unlock(&data->meal_lock);
		if (diff > data->args.time_to_die)
			return (die_philo(&philos[i]));
		if (data->args.must_eat_count > 0
			&& times_eaten >= data->args.must_eat_count)
			(*full_count)++;
		i++;
	}
}

void	monitor_routine(t_data *data, t_philo *philos)
{
	int			full_count;

	// Wait for start time
	while (current_timestamp() < data->start_time)
		continue ;
		
	while (!check_death(philos))
	{
		full_count = 0;
		monitor_core(data, philos, &full_count);
		if (data->args.must_eat_count > 0
			&& full_count == data->args.philo_count)
		{
			pthread_mutex_lock(&data->death_lock);
			data->simulation_complete = 1;
			pthread_mutex_unlock(&data->death_lock);
			return ;
		}
		usleep(1000);
	}
}

void	one_philo_routine(t_philo *philo)
{
	if (philo->data->args.philo_count == 1)
	{
		think_philo(philo);
		ft_sleep(philo->data->args.time_to_die);
		pthread_mutex_lock(&philo->data->death_lock);
		if (!philo->data->someone_died)
		{
			philo->data->someone_died = 1;
			pthread_mutex_unlock(&philo->data->death_lock);
			pthread_mutex_lock(&philo->data->print_lock);
			printf("%d %d died\n", philo->data->args.time_to_die, philo->id);
			pthread_mutex_unlock(&philo->data->print_lock);
		}
		else
			pthread_mutex_unlock(&philo->data->death_lock);
		return ;
	}
}

int	philosopher_core(t_philo	*philo)
{
	think_philo(philo);
	if (check_death(philo) || check_simulation_complete(philo))
		return (1);
	take_forks(philo);
	if (check_death(philo) || check_simulation_complete(philo))
		return (1);
	eat_philo(philo);
	release_forks(philo);
	if (philo->data->args.must_eat_count != -1)
	{
		pthread_mutex_lock(&philo->data->meal_lock);
		if (philo->times_eaten >= philo->data->args.must_eat_count)
		{
			pthread_mutex_unlock(&philo->data->meal_lock);
			return (1);
		}
		pthread_mutex_unlock(&philo->data->meal_lock);
	}
	if (check_death(philo) || check_simulation_complete(philo))
		return (1);
	sleep_philo(philo);
	return (0);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->data->ready_mutex);
	pthread_mutex_unlock(&philo->data->ready_mutex);
	
	// Wait for start time
	while (current_timestamp() < philo->data->start_time)
		usleep(100);
	
	one_philo_routine(philo);
	if (philo->id % 2 == 1)
		usleep(1000);
	while (!check_death(philo) && !check_simulation_complete(philo))
	{
		if (philosopher_core(philo))
			break ;
	}
	return (NULL);
}
