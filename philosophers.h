#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

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
	long			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	int				someone_died;
}				t_data;

typedef struct s_philo
{
	int				id;
	int				times_eaten;
	long			last_meal;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
	long long thinking_start;
}	                t_philo;


long current_timestamp (void);
int     ft_atoi(char *str);

void print_state(char *msg, t_philo *philo);
void think_philo(t_philo *philo);
void eat_philo(t_philo *philo);
void sleep_philo(t_philo *philo);
void die_philo(t_philo *philo);

bool    init_args(t_args *args, int argc, char **argv);
void    init_philos(t_philo *philos, t_args args);
void    init_data(t_args args, t_philo *philos, t_data *data);
void setup_philos(t_data *data, t_philo *philos);

void    take_forks(t_philo *philo);
void    release_forks(t_philo *philo);
#endif