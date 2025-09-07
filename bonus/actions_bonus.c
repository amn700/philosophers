/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchaib <mohchaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 08:24:39 by mohchaib          #+#    #+#             */
/*   Updated: 2025/09/07 11:56:45 by mohchaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	take_forks(t_philo *philo)
{
	sem_wait(philo->data->forks);
	print_state("has taken a fork", philo);
	sem_wait(philo->data->forks);
	print_state("has taken a fork", philo);
}

void	release_forks(t_philo *philo)
{
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
}

void	think_philo(t_philo *philo)
{
	print_state("is thinking", philo);
}

void	eat_philo(t_philo *philo)
{
	philo->last_meal = current_timestamp();
	philo->meals_eaten += 1;
	print_state("is eating", philo);
	ft_sleep(philo->data->args.time_to_eat);
}

void	sleep_philo(t_philo *philo)
{
	print_state("is sleeping", philo);
	ft_sleep(philo->data->args.time_to_sleep);
}
