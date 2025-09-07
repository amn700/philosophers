# Philosophers Mandatory

This folder contains the mandatory implementation of the philosophers dining problem using threads and mutexes.

## Features:
- Thread-based implementation
- Mutex synchronization for forks and shared resources
- Optimized performance with minimal overhead
- Race condition protection with dedicated death_lock
- High-precision timing system

## Files:
- `main.c` - Main program and thread management
- `actions.c` - Philosopher actions (eat, sleep, think, fork management)
- `helper_func1.c` - Utility functions (death checking, printing, timing)
- `initialization.c` - Input validation and data structure initialization
- `philosophers.h` - Header file with all declarations
- `Makefile` - Build configuration

## Build:
```bash
make
```

## Usage:
```bash
./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat]
```

## Performance Optimizations:
- Monitor frequency: 100μs for optimal responsiveness
- Sleep precision: 50μs intervals for accurate timing
- Minimal startup delays: 50μs staggering for even philosophers
- Streamlined critical sections with reduced death checks
- Optimized print function with efficient lock usage

## Examples:
```bash
./philo 5 800 200 200        # 5 philosophers, no death limit
./philo 4 410 200 200        # Tight timing scenario
./philo 5 600 150 150 7      # Each philosopher must eat 7 times
```
