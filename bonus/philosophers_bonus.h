/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 08:24:47 by mohchaib          #+#    #+#             */
/*   Updated: 2025/09/11 10:48:42 by codespace        ###   ########.fr       */
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
# include <limits.h>

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
	int				id;
	long long		last_meal;
	int				meals_eaten;
	int				should_stop;
	t_data			*data;
}				t_philo;

long long	current_timestamp(void);
void		ft_sleep(unsigned int milisec);
int			ft_atoi(char *str);
void		print_error_msg(void);
void		ft_putlongnbr(long long nbr);
void		print_state_write(long long timestamp, int id, char *state);
int			ft_strlen(char *str);
void		ft_write_error(char *msg);
void		*death_monitor(void *arg);
void		philosopher_routine(t_philo *philo);
void		single_philo_routine(t_philo *philo);
int			check_stop_condition(t_philo *philo);
void		execute_philosopher_cycle(t_philo *philo);
int			check_meal_completion(t_philo *philo);
int			check_death_condition(t_philo *philo, long long current_time);
void		think_philo(t_philo *philo);
void		take_forks(t_philo *philo);
void		eat_philo(t_philo *philo);
void		release_forks(t_philo *philo);
void		sleep_philo(t_philo *philo);
void		die_philo(t_philo *philo);
void		print_state(char *state, t_philo *philo);
void		cleanup(t_data *data, t_philo *philos);
void		wait_for_processes(t_args args, t_data data);
bool		check_and_take_first_fork(t_philo *philo);
bool		check_and_take_second_fork(t_philo *philo);
#endif
