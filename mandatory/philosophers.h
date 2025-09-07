/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 08:25:05 by mohchaib          #+#    #+#             */
/*   Updated: 2025/09/07 11:07:41 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_args
{
	int	philo_count;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	must_eat_count;
}		t_args;

typedef struct s_data
{
	t_args			args;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	death_lock;
	pthread_mutex_t	ready_mutex;
	int				someone_died;

}				t_data;

typedef struct s_philo
{
	int				id;
	int				times_eaten;
	long long		last_meal;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
}					t_philo;

void		*philosopher_routine(void *arg);
void		ft_sleep(unsigned int milisec);
long long	current_timestamp(void);
int			ft_atoi(char *str);
void		print_state(char *msg, t_philo *philo);
void		think_philo(t_philo *philo);
void		eat_philo(t_philo *philo);
void		sleep_philo(t_philo *philo);
int			check_death(t_philo *philo);
int			check_death_unsafe(t_data *data);
void		set_death(t_philo *philo);
void		die_philo(t_philo *philo);
bool		init_args(t_args *args, int argc, char **argv);
bool		init_data(t_args args, t_data *data);
void		setup_philos(t_data *data, t_philo *philos);
void		take_forks(t_philo *philo);
void		release_forks(t_philo *philo);
bool		launch_threads(t_args args, t_data data, t_philo *philos);
void		monitor_routine(t_data *data, t_philo *philos);
void		start_simulation(t_data *data, t_philo *philos, t_args *args);
void		cleanup(t_data *data, t_philo *philos, int philo_count, t_args *args);
#endif