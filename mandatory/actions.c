#include "philosophers.h"

void take_forks(t_philo *philo)
{
    // Single death check at start to avoid redundant checking
    if (check_death(philo))
        return;
        
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(philo->right_fork);
        print_state("has taken a fork", philo);
        
        pthread_mutex_lock(philo->left_fork);
        print_state("has taken a fork", philo);
    }
    else
    {
        pthread_mutex_lock(philo->left_fork);
        print_state("has taken a fork", philo);
        
        pthread_mutex_lock(philo->right_fork);
        print_state("has taken a fork", philo);
    }
}


void    release_forks(t_philo *philo)
{
    // Always release forks in same order as taken to prevent deadlock
    if (philo->id % 2 == 0)
    {
        pthread_mutex_unlock(philo->left_fork);
        pthread_mutex_unlock(philo->right_fork);
    }
    else
    {
        pthread_mutex_unlock(philo->right_fork);
        pthread_mutex_unlock(philo->left_fork);
    }
}

void think_philo(t_philo *philo)
{
    if (check_death(philo))
        return;
    print_state("is thinking", philo);
    return ;
}

void eat_philo(t_philo *philo)
{
    if (check_death(philo))
        return;
    
    pthread_mutex_lock(&philo->data->meal_lock);
    philo->last_meal = current_timestamp();
    philo->times_eaten++;
    pthread_mutex_unlock(&philo->data->meal_lock);
    
    print_state("is eating", philo);
    ft_sleep(philo->data->args.time_to_eat);
    return ;
}

void sleep_philo(t_philo *philo)
{
    if (check_death(philo))
        return;
    print_state("is sleeping", philo);
    ft_sleep(philo->data->args.time_to_sleep);
    return ;
}


void die_philo(t_philo *philo)
{
    long long time_stamp;
    
    pthread_mutex_lock(&philo->data->death_lock);
    if (!philo->data->someone_died)
    {
        philo->data->someone_died = 1;
        pthread_mutex_unlock(&philo->data->death_lock);
        
        pthread_mutex_lock(&philo->data->print_lock);
        time_stamp = current_timestamp() - philo->data->start_time;
        printf("%lld %d died\n", time_stamp, philo->id);
        pthread_mutex_unlock(&philo->data->print_lock);
    }
    else
    {
        pthread_mutex_unlock(&philo->data->death_lock);
    }
    return ;
}
