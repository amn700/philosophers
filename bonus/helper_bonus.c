/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 08:24:42 by mohchaib          #+#    #+#             */
/*   Updated: 2025/09/07 18:29:34 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

long long	current_timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_sleep(unsigned int milisec)
{
	struct timeval	start;
	struct timeval	current;
	unsigned int	elapsed;

	gettimeofday(&start, NULL);
	while (1)
	{
		gettimeofday(&current, NULL);
		elapsed = (current.tv_sec - start.tv_sec) * 1000
			+ (current.tv_usec - start.tv_usec) / 1000;
		if (elapsed >= milisec)
			break ;
		usleep(100);
	}
}

int	ft_atoi(char *str)
{
	long	total;
	int		sign;
	int		i;

	total = 0;
	sign = 1;
	i = 0;
	if (!str)
		return (0);
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -sign;
		i++;
	}
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		total = total * 10 + (str[i] - '0');
		i++;
	}
	return ((int)(sign * total));
}

void	print_state(char *state, t_philo *philo)
{
	long long	timestamp;

	pthread_mutex_lock(&philo->meal_mutex);
	if (philo->should_stop)
	{
		pthread_mutex_unlock(&philo->meal_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->meal_mutex);
		
	sem_wait(philo->data->writing);
	
	// Double check after acquiring semaphore with mutex protection
	pthread_mutex_lock(&philo->meal_mutex);
	if (!philo->should_stop)
	{
		pthread_mutex_unlock(&philo->meal_mutex);
		timestamp = current_timestamp() - philo->data->start_time;
		printf("%lld %d %s\n", timestamp, philo->id, state);
	}
	else
	{
		pthread_mutex_unlock(&philo->meal_mutex);
	}
	sem_post(philo->data->writing);
}

void	print_error_msg(void)
{
	printf("invalid number of arguments\n"
		"USAGE : [number_of_philosophers]"
		" [time_to_die] [time_to_eat][time_to_sleep]"
		" [number_of_times_each_philosopher_must_eat]\n");
}
