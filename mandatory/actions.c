/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 08:24:52 by mohchaib          #+#    #+#             */
/*   Updated: 2025/09/07 16:27:48 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	take_forks(t_philo *philo)
{
	if (check_death(philo) || check_simulation_complete(philo))
		return ;
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_state("has taken a fork", philo);
		pthread_mutex_lock(philo->left_fork);
		print_state("has taken a fork", philo);
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_state("has taken a fork", philo);
		pthread_mutex_lock(philo->right_fork);
		print_state("has taken a fork", philo);
	}
}

void	release_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
}

void	think_philo(t_philo *philo)
{
	if (check_death(philo) || check_simulation_complete(philo))
		return ;
	print_state("is thinking", philo);
	return ;
}

void	eat_philo(t_philo *philo)
{
	if (check_death(philo) || check_simulation_complete(philo))
		return ;
	pthread_mutex_lock(&philo->data->meal_lock);
	philo->last_meal = current_timestamp();
	philo->times_eaten++;
	pthread_mutex_unlock(&philo->data->meal_lock);
	print_state("is eating", philo);
	ft_sleep(philo->data->args.time_to_eat);
	return ;
}

void	sleep_philo(t_philo *philo)
{
	if (check_death(philo) || check_simulation_complete(philo))
		return ;
	print_state("is sleeping", philo);
	ft_sleep(philo->data->args.time_to_sleep);
	return ;
}
