# Philosophers Bonus

This folder contains the bonus implementation of the philosophers problem using semaphores instead of mutexes.

## Key Differences from Mandatory:
- Uses semaphores for synchronization
- Fork management with semaphores
- Process-based instead of thread-based (if required)

## Files:
- `philo_bonus.c` - Main program
- `philosophers_bonus.h` - Header file
- `Makefile` - Build configuration

## Build:
```bash
make
```

## Usage:
```bash
./philo_bonus [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat]
```
