#include "philosophers.h"

void print_state(char *msg, t_philo *philo)
{
    long long time_stamp;

    if (philo->data->someone_died)
        return;
        
    pthread_mutex_lock(&philo->data->print_lock);
    if (!philo->data->someone_died)
    {
        time_stamp = current_timestamp() - philo->data->start_time;
        printf("%lld %d %s\n", time_stamp, philo->id, msg);
    }
    pthread_mutex_unlock(&philo->data->print_lock);
}

long long current_timestamp (void)
{
    struct timeval tv;
    gettimeofday (&tv, NULL);
    return ((tv.tv_sec *1000) + (tv.tv_usec / 1000));
}

int     ft_atoi(char *str)
{
    long total = 0;
    int sign = 1;
    int i = 0;
    
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
        // Check for overflow
        if (total > 2147483647)
            return (sign == 1 ? 2147483647 : -2147483648);
        i++;
    }
    return (int)(sign * total);
}
