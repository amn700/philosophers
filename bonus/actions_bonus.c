#include "philosophers_bonus.h"

void take_forks(t_philo *philo)
{
    sem_wait(philo->data->forks);
    print_state("has taken a fork", philo);
    sem_wait(philo->data->forks);
    print_state("has taken a fork", philo);
}

void release_forks(t_philo *philo)
{
    sem_post(philo->data->forks);
    sem_post(philo->data->forks);
}

void think_philo(t_philo *philo)
{
    print_state("is thinking", philo);
}

void eat_philo(t_philo *philo)
{
    philo->last_meal = current_timestamp();
    philo->meals_eaten += 1;
    
    print_state("is eating", philo);
    ft_sleep(philo->data->args.time_to_eat);
}

void sleep_philo(t_philo *philo)
{
    print_state("is sleeping", philo);
    ft_sleep(philo->data->args.time_to_sleep);
}

void die_philo(t_philo *philo)
{
    // Use death_print semaphore to ensure only one death message
    sem_wait(philo->data->death_print);
    
    // Print death message - only first to reach here will print
    sem_wait(philo->data->writing);
    long long timestamp = current_timestamp() - philo->data->start_time;
    printf("%lld %d died\n", timestamp, philo->id);
    sem_post(philo->data->writing);
    
    // Don't post death_print semaphore - keep it locked
    exit(1);
}
