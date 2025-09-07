/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 08:24:39 by mohchaib          #+#    #+#             */
/*   Updated: 2025/09/07 21:13:05 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	if (philo->should_stop)
		return (pthread_mutex_unlock(&philo->meal_mutex));
	pthread_mutex_unlock(&philo->meal_mutex);
	sem_wait(philo->data->forks);
	pthread_mutex_lock(&philo->meal_mutex);
	if (philo->should_stop)
		return (ft_release(&philo->meal_mutex, philo->data->forks));
	pthread_mutex_unlock(&philo->meal_mutex);
	print_state("has taken a fork", philo);
	pthread_mutex_lock(&philo->meal_mutex);
	if (philo->should_stop)
		return (ft_release(&philo->meal_mutex, philo->data->forks));
	pthread_mutex_unlock(&philo->meal_mutex);
	sem_wait(philo->data->forks);
	pthread_mutex_lock(&philo->meal_mutex);
	if (philo->should_stop)
	{
		pthread_mutex_unlock(&philo->meal_mutex);
		sem_post(philo->data->forks);
		sem_post(philo->data->forks);
		return ;
	}
	pthread_mutex_unlock(&philo->meal_mutex);
	print_state("has taken a fork", philo);
}

void	release_forks(t_philo *philo)
{
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
}

void	think_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	if (!philo->should_stop)
	{
		pthread_mutex_unlock(&philo->meal_mutex);
		print_state("is thinking", philo);
	}
	else
	{
		pthread_mutex_unlock(&philo->meal_mutex);
	}
}

void	eat_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	if (philo->should_stop)
	{
		pthread_mutex_unlock(&philo->meal_mutex);
		return ;
	}
	philo->last_meal = current_timestamp();
	philo->meals_eaten += 1;
	pthread_mutex_unlock(&philo->meal_mutex);
	print_state("is eating", philo);
	ft_sleep(philo->data->args.time_to_eat);
}

void	sleep_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	if (!philo->should_stop)
	{
		pthread_mutex_unlock(&philo->meal_mutex);
		print_state("is sleeping", philo);
		ft_sleep(philo->data->args.time_to_sleep);
	}
	else
		pthread_mutex_unlock(&philo->meal_mutex);
}
