/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_func1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchaib <mohchaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 08:24:55 by mohchaib          #+#    #+#             */
/*   Updated: 2025/09/07 12:11:47 by mohchaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_state(char *msg, t_philo *philo)
{
	long long	time_stamp;

	pthread_mutex_lock(&philo->data->print_lock);
	if (!philo->data->someone_died)
	{
		time_stamp = current_timestamp() - philo->data->start_time;
		printf("%lld %d %s\n", time_stamp, philo->id, msg);
	}
	pthread_mutex_unlock(&philo->data->print_lock);
}

int	check_death(t_philo *philo)
{
	int	died;

	pthread_mutex_lock(&philo->data->death_lock);
	died = philo->data->someone_died;
	pthread_mutex_unlock(&philo->data->death_lock);
	return (died);
}

void	set_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death_lock);
	philo->data->someone_died = 1;
	pthread_mutex_unlock(&philo->data->death_lock);
}

long long	current_timestamp(void)
{
	struct timeval	tv;

	gettimeofday (&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	ft_atoi(char *str)
{
	long	total;
	int		sign;

	total = 0;
	sign = 1;
	if (!str)
		return (0);
	while (*str == 32 || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -sign;
		str++;
	}
	while (*str && *str >= '0' && *str <= '9')
	{
		total = total * 10 + (*str - '0');
		str++;
	}
	return ((int)(sign * total));
}
