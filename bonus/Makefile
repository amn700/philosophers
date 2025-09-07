NAME = philo_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -O2 -march=native

SRCDIR = .
OBJDIR = .

SOURCES = main_bonus.c actions_bonus.c routines_bonus.c helper_bonus.c

OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
