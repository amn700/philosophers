#include "philosophers.h"

void take_forks(t_philo *philo)
{
    pthread_mutex_lock(philo->left_fork);
    print_state("has taken a fork", philo);
    pthread_mutex_lock(philo->right_fork);
    print_state("has taken a fork", philo);
}
// void take_forks(t_philo *philo)
// {
//     if (philo->id % 2 == 0)
//     {
//         pthread_mutex_lock(philo->right_fork);
//         print_state("has taken a fork", philo);
//         pthread_mutex_lock(philo->left_fork);
//         print_state("has taken a fork", philo);
//     }
//     else
//     {
//         pthread_mutex_lock(philo->left_fork);
//         print_state("has taken a fork", philo);
//         pthread_mutex_lock(philo->right_fork);
//         print_state("has taken a fork", philo);
//     }
// }


void    release_forks(t_philo *philo)
{
    pthread_mutex_unlock(philo->right_fork);
    pthread_mutex_unlock(philo->left_fork);
}

void think_philo(t_philo *philo)
{
    philo->thinking_start = current_timestamp();
    print_state("is thinking", philo);
    return ;
}

void eat_philo(t_philo *philo)
{
    philo->last_meal = current_timestamp();
    print_state("is eating", philo);
        // printf("took %d , %lldms before eating\n",
        // philo->id,
        // philo->last_meal - philo->thinking_start);
    philo->times_eaten++;
    ft_sleep(philo->data->args.time_to_eat);
    // philo->last_meal = current_timestamp();

    // printf("philo %d ate %d time('s)\n", philo->id, philo->times_eaten);
    return ;
}

void sleep_philo(t_philo *philo)
{
    print_state("is sleeping", philo);

    ft_sleep(philo->data->args.time_to_sleep);
    return ;
}


void die_philo(t_philo *philo)
{
    print_state("died", philo);
    philo->data->someone_died = 1;
    return ;
}
