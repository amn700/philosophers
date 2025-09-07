#include "philosophers_bonus.h"
#include <pthread.h>

void *death_monitor(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    
    while (!philo->should_stop)
    {
        long long current_time = current_timestamp();
        
        if (current_time - philo->last_meal > philo->data->args.time_to_die)
        {
            die_philo(philo);
            return (NULL);
        }
        usleep(1000); // Check every 1ms
    }
    return (NULL);
}

void philosopher_routine(t_philo *philo)
{
    pthread_t monitor_thread;
    
    philo->should_stop = 0;
    
    // Create death monitoring thread
    if (pthread_create(&monitor_thread, NULL, death_monitor, philo) != 0)
    {
        printf("pthread_create failed\n");
        exit(1);
    }
    
    while (!philo->should_stop)
    {
        think_philo(philo);
        if (philo->should_stop) break;
        
        take_forks(philo);
        if (philo->should_stop) 
        {
            release_forks(philo);
            break;
        }
        
        eat_philo(philo);
        release_forks(philo);
        
        if (philo->data->args.must_eat_count != -1 && 
            philo->meals_eaten >= philo->data->args.must_eat_count)
        {
            philo->should_stop = 1;
            break;
        }
            
        if (philo->should_stop) break;
        sleep_philo(philo);
    }
    
    philo->should_stop = 1;
    pthread_join(monitor_thread, NULL);
}
