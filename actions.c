#include "philosophers.h"

void take_forks(t_philo *philo)
{
    // Quick death check before attempting to take forks
    if (philo->data->someone_died)
        return;
        
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(philo->right_fork);
        if (!philo->data->someone_died)
            print_state("has taken a fork", philo);
        if (philo->data->someone_died)
        {
            pthread_mutex_unlock(philo->right_fork);
            return;
        }
        pthread_mutex_lock(philo->left_fork);
        if (!philo->data->someone_died)
            print_state("has taken a fork", philo);
    }
    else
    {
        pthread_mutex_lock(philo->left_fork);
        if (!philo->data->someone_died)
            print_state("has taken a fork", philo);
        if (philo->data->someone_died)
        {
            pthread_mutex_unlock(philo->left_fork);
            return;
        }
        pthread_mutex_lock(philo->right_fork);
        if (!philo->data->someone_died)
            print_state("has taken a fork", philo);
    }
}


void    release_forks(t_philo *philo)
{
    pthread_mutex_unlock(philo->right_fork);
    pthread_mutex_unlock(philo->left_fork);
}

void think_philo(t_philo *philo)
{
    if (philo->data->someone_died)
        return;
    philo->thinking_start = current_timestamp();
    print_state("is thinking", philo);
    return ;
}

void eat_philo(t_philo *philo)
{
    if (philo->data->someone_died)
        return;
    philo->last_meal = current_timestamp();
    print_state("is eating", philo);
    philo->times_eaten++;
    ft_sleep(philo->data->args.time_to_eat);
    return ;
}

void sleep_philo(t_philo *philo)
{
    if (philo->data->someone_died)
        return;
    print_state("is sleeping", philo);
    ft_sleep(philo->data->args.time_to_sleep);
    return ;
}


void die_philo(t_philo *philo)
{
    long time_stamp;
    
    pthread_mutex_lock(&philo->data->print_lock);
    if (!philo->data->someone_died)
    {
        time_stamp = current_timestamp() - philo->data->start_time;
        printf("%ld %d died\n", time_stamp, philo->id);
        philo->data->someone_died = 1;
    }
    pthread_mutex_unlock(&philo->data->print_lock);
    return ;
}
