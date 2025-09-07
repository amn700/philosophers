NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror -fsanitize=thread# -pthread -O2 -march=native
SRCS = main.c initialization.c actions.c helper_func1.c routines.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c philosophers.h
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re 