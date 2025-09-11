/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 08:24:39 by mohchaib          #+#    #+#             */
/*   Updated: 2025/09/11 10:41:12 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	take_forks(t_philo *philo)
{
	if (!check_and_take_first_fork(philo))
		return ;
	if (!check_and_take_second_fork(philo))
		return ;
}

void	release_forks(t_philo *philo)
{
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
}

void	think_philo(t_philo *philo)
{
	if (!philo->should_stop)
		print_state("is thinking", philo);
}

void	eat_philo(t_philo *philo)
{
	if (philo->should_stop)
		return ;
	philo->last_meal = current_timestamp();
	philo->meals_eaten += 1;
	print_state("is eating", philo);
	ft_sleep(philo->data->args.time_to_eat);
}

void	sleep_philo(t_philo *philo)
{
	if (!philo->should_stop)
	{
		print_state("is sleeping", philo);
		ft_sleep(philo->data->args.time_to_sleep);
	}
}
