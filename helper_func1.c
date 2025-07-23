#include "philosophers.h"

void print_state(char *msg, t_philo *philo)
{
    long time_stamp;

    pthread_mutex_lock(&philo->data->print_lock);
    time_stamp =  current_timestamp() - philo->data->start_time;
    if (!philo->data->someone_died)
        printf("%ld %d %s\n", time_stamp , philo->id, msg);
    pthread_mutex_unlock(&philo->data->print_lock);
}

long current_timestamp (void)
{
    struct timeval tv;
    gettimeofday (&tv, NULL);
    return ((tv.tv_sec *1000) + (tv.tv_usec / 1000));
}

int     ft_atoi(char *str)
{
    int total = 0;
    int sign = 1;
    int i = 0;
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
        i++;
    }
    return sign * total;
}
