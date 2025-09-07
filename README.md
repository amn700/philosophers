# Philosophers Project

This project implements the classic "Dining Philosophers" problem in C, demonstrating concurrent programming concepts and synchronization techniques.

## Project Structure

```
philosophers/
├── mandatory/          # Thread-based implementation with mutexes
│   ├── main.c
│   ├── actions.c
│   ├── helper_func1.c
│   ├── initialization.c
│   ├── philosophers.h
│   ├── Makefile
│   └── README.md
├── bonus/              # Process-based implementation with semaphores
│   ├── philo_bonus.c
│   ├── philosophers_bonus.h
│   ├── Makefile
│   └── README.md
└── README.md          # This file
```

## The Problem

The Dining Philosophers problem is a classic synchronization problem that illustrates the challenges of avoiding deadlock and resource starvation in concurrent systems. N philosophers sit around a circular table with N forks. Each philosopher alternates between thinking and eating, but needs two forks to eat.

## Implementations

### Mandatory (Threads + Mutexes)
- **Location**: `./mandatory/`
- **Approach**: Uses pthread library with mutex locks
- **Features**: Optimized performance, race condition protection, high-precision timing
- **Build**: `cd mandatory && make`
- **Run**: `./philo [philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [must_eat_count]`

### Bonus (Processes + Semaphores)
- **Location**: `./bonus/`
- **Approach**: Uses processes with semaphore synchronization
- **Features**: Process isolation, semaphore-based fork management
- **Build**: `cd bonus && make`
- **Run**: `./philo_bonus [philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [must_eat_count]`

## Quick Start

1. **Mandatory version**:
   ```bash
   cd mandatory
   make
   ./philo 5 800 200 200
   ```

2. **Bonus version**:
   ```bash
   cd bonus
   make
   ./philo_bonus 5 800 200 200
   ```

## Parameters

- `number_of_philosophers`: Number of philosophers (and forks)
- `time_to_die`: Time in milliseconds after which a philosopher dies if they haven't eaten
- `time_to_eat`: Time in milliseconds it takes for a philosopher to eat
- `time_to_sleep`: Time in milliseconds a philosopher spends sleeping
- `number_of_times_each_philosopher_must_eat`: (Optional) Program stops when all philosophers have eaten this many times

## Performance Notes

The mandatory implementation includes several optimizations:
- High-frequency monitoring (100μs intervals)
- Precision timing (50μs sleep intervals)
- Minimized lock contention
- Efficient death detection

Test cases like `./philo 5 600 150 150` demonstrate the implementation's ability to handle tight timing constraints.
