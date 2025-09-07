/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 21:00:48 by codespace         #+#    #+#             */
/*   Updated: 2025/09/07 21:03:44 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int	check_death_status(t_philo *philo)
{
	int	sem_value;

	if (sem_getvalue(philo->data->death_print, &sem_value) == 0
		&& sem_value == 0)
		return (1);
	return (0);
}

void	ft_release(pthread_mutex_t *mutex, sem_t *sem)
{
	pthread_mutex_unlock(mutex);
	sem_post(sem);
}

void	handle_print_output(char *state, t_philo *philo)
{
	long long	timestamp;

	if (check_death_status(philo))
	{
		sem_post(philo->data->writing);
		return ;
	}
	timestamp = current_timestamp() - philo->data->start_time;
	printf("%lld %d %s\n", timestamp, philo->id, state);
}

void	cleanup(t_data *data, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < data->args.philo_count)
		pthread_mutex_destroy(&philos[i++].meal_mutex);
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
