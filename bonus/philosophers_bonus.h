/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 08:24:47 by mohchaib          #+#    #+#             */
/*   Updated: 2025/09/07 17:17:39 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <semaphore.h>
# include <stdbool.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <pthread.h>

typedef struct s_args
{
	int	philo_count;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	must_eat_count;
}	t_args;

typedef struct s_data
{
	t_args		args;
	long long	start_time;
	sem_t		*forks;
	sem_t		*writing;
	sem_t		*death_check;
	sem_t		*death_print;
	pid_t		*philosophers;
}				t_data;

typedef struct s_philo
{
	int			id;
	long long	last_meal;
	int			meals_eaten;
	int			should_stop;
	pid_t		pid;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	stop_mutex;
	t_data		*data;
}				t_philo;

long long	current_timestamp(void);
void		ft_sleep(unsigned int milisec);
int			ft_atoi(char *str);
void		print_error_msg(void);
void		*death_monitor(void *arg);
void		philosopher_routine(t_philo *philo);
void		think_philo(t_philo *philo);
void		take_forks(t_philo *philo);
void		eat_philo(t_philo *philo);
void		release_forks(t_philo *philo);
void		sleep_philo(t_philo *philo);
void		die_philo(t_philo *philo);
void		print_state(char *state, t_philo *philo);
void		cleanup(t_data *data, t_philo *philos);
void		wait_for_processes(t_args args, t_data data);
#endif
