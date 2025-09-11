/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 21:00:00 by codespace        #+#    #+#             */
/*   Updated: 2025/09/07 21:00:00 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int	check_stop_condition(t_philo *philo)
{
	return (philo->should_stop);
}

void	execute_philosopher_cycle(t_philo *philo)
{
	if (check_stop_condition(philo))
		return ;
	think_philo(philo);
	if (check_stop_condition(philo))
		return ;
	take_forks(philo);
	if (check_stop_condition(philo))
	{
		release_forks(philo);
		return ;
	}
	eat_philo(philo);
	release_forks(philo);
}

int	check_meal_completion(t_philo *philo)
{
	int	completed;

	completed = (philo->data->args.must_eat_count != -1
			&& philo->meals_eaten >= philo->data->args.must_eat_count);
	if (completed)
		philo->should_stop = 1;
	return (completed);
}

int	check_death_condition(t_philo *philo, long long current_time)
{
	long long	last_meal_time;
	int			should_die;

	last_meal_time = philo->last_meal;
	should_die = (current_time - last_meal_time > philo->data->args.time_to_die
			&& !philo->should_stop);
	if (should_die)
		philo->should_stop = 1;
	return (should_die);
}
