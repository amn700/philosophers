/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_helpers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:00:00 by codespace         #+#    #+#             */
/*   Updated: 2025/09/11 10:41:12 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

bool	check_and_take_first_fork(t_philo *philo)
{
	if (philo->should_stop)
		return (false);
	sem_wait(philo->data->forks);
	if (philo->should_stop)
	{
		sem_post(philo->data->forks);
		return (false);
	}
	print_state("has taken a fork", philo);
	return (true);
}

bool	check_and_take_second_fork(t_philo *philo)
{
	if (philo->should_stop)
	{
		sem_post(philo->data->forks);
		return (false);
	}
	sem_wait(philo->data->forks);
	if (philo->should_stop)
	{
		sem_post(philo->data->forks);
		sem_post(philo->data->forks);
		return (false);
	}
	print_state("has taken a fork", philo);
	return (true);
}
