NAME = philo_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread #-fsanitize=thread -g -O2 -march=native

SRCS = main_bonus.c actions_bonus.c helper_bonus.c routines_bonus.c
OBJS = $(SRCS:.c=.o)


all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c philosophers_bonus.h
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
