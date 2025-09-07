#include "philosophers_bonus.h"

long long current_timestamp(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_sleep(unsigned int milisec)
{
	struct timeval start;
	struct timeval current;
	unsigned int elapsed;

	gettimeofday(&start, NULL);
	while (1)
	{
		gettimeofday(&current, NULL);
		elapsed = (current.tv_sec - start.tv_sec) * 1000
				+ (current.tv_usec - start.tv_usec) / 1000;
		if (elapsed >= milisec)
			break;
		usleep(50);  // High precision timing
	}
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

void print_state(char *state, t_philo *philo)
{
    sem_wait(philo->data->writing);
    long long timestamp = current_timestamp() - philo->data->start_time;
    printf("%lld %d %s\n", timestamp, philo->id, state);
    sem_post(philo->data->writing);
}

void print_error_msg(void)
{
	printf("invalid number of arguments\n"
	"USAGE : [number_of_philosophers]"
	" [time_to_die] [time_to_eat][time_to_sleep]"
	" [number_of_times_each_philosopher_must_eat]\n");
}
