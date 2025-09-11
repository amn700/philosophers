/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2_bonus_new.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 21:02:11 by codespace         #+#    #+#             */
/*   Updated: 2025/09/11 10:41:12 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	print_state_write(long long timestamp, int id, char *state)
{
	ssize_t	ret;

	ft_putlongnbr(timestamp);
	ret = write(1, " ", 1);
	(void)ret;
	ft_putlongnbr(id);
	ret = write(1, " ", 1);
	(void)ret;
	if (state[0] == 'h' && state[1] == 'a')
		ret = write(1, "has taken a fork\n", 17);
	else if (state[0] == 'i' && state[1] == 's' && state[2] == ' '
		&& state[3] == 'e')
		ret = write(1, "is eating\n", 10);
	else if (state[0] == 'i' && state[1] == 's' && state[2] == ' '
		&& state[3] == 's')
		ret = write(1, "is sleeping\n", 12);
	else if (state[0] == 'i' && state[1] == 's' && state[2] == ' '
		&& state[3] == 't')
		ret = write(1, "is thinking\n", 12);
	else if (state[0] == 'd' && state[1] == 'i')
		ret = write(1, "died\n", 5);
	(void)ret;
}

static void	cleanup_philosopher_semaphores(t_data *data, t_philo *philos)
{
	int		i;
	char	sem_name[50];
	char	*name_prefix;
	int		j;

	name_prefix = "/meal_sem_";
	i = 0;
	while (i < data->args.philo_count)
	{
		j = 0;
		while (name_prefix[j])
		{
			sem_name[j] = name_prefix[j];
			j++;
		}
		sem_name[j] = '0' + i;
		sem_name[j + 1] = '\0';
		sem_close(philos[i].meal_sem);
		sem_unlink(sem_name);
		i++;
	}
}

void	cleanup(t_data *data, t_philo *philos)
{
	cleanup_philosopher_semaphores(data, philos);
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
