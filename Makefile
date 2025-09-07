# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: philosophers                               +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/07                          #+#    #+#    #+#             #
#    Updated: 2025/09/07                         ###   ########.fr             #
#                                                                              #
# **************************************************************************** #

# Colors for output
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
MAGENTA = \033[0;35m
CYAN = \033[0;36m
WHITE = \033[0;37m
RESET = \033[0m

# Project directories
MANDATORY_DIR = mandatory
BONUS_DIR = bonus

# Executable names
MANDATORY_NAME = philo
BONUS_NAME = philo_bonus

# Default target
all: mandatory

# Help target
help:
	@echo "$(CYAN)========================================$(RESET)"
	@echo "$(CYAN)         PHILOSOPHERS PROJECT           $(RESET)"
	@echo "$(CYAN)========================================$(RESET)"
	@echo ""
	@echo "$(YELLOW)Available targets:$(RESET)"
	@echo "  $(GREEN)mandatory$(RESET)    - Build mandatory version (threads + mutexes)"
	@echo "  $(GREEN)bonus$(RESET)        - Build bonus version (processes + semaphores)"
	@echo "  $(GREEN)all$(RESET)          - Build mandatory version (default)"
	@echo "  $(GREEN)clean$(RESET)        - Clean object files from both versions"
	@echo "  $(GREEN)fclean$(RESET)       - Clean everything (objects + executables)"
	@echo "  $(GREEN)re$(RESET)           - Clean and rebuild mandatory"
	@echo "  $(GREEN)re_bonus$(RESET)     - Clean and rebuild bonus"
	@echo "  $(GREEN)help$(RESET)         - Show this help message"
	@echo ""

mandatory:
	@echo "$(BLUE)Building mandatory version...$(RESET)"
	@$(MAKE) -C $(MANDATORY_DIR)
	@echo "$(GREEN)✓ Mandatory version built successfully!$(RESET)"
	@echo "$(CYAN)Usage: ./$(MANDATORY_DIR)/$(MANDATORY_NAME) <nb_philo> <time_to_die> <time_to_eat> <time_to_sleep> [nb_must_eat]$(RESET)"

bonus:
	@echo "$(BLUE)Building bonus version...$(RESET)"
	@$(MAKE) -C $(BONUS_DIR)
	@echo "$(GREEN)✓ Bonus version built successfully!$(RESET)"
	@echo "$(CYAN)Usage: ./$(BONUS_DIR)/$(BONUS_NAME) <nb_philo> <time_to_die> <time_to_eat> <time_to_sleep> [nb_must_eat]$(RESET)"


clean:
	@echo "$(YELLOW)Cleaning object files...$(RESET)"
	@$(MAKE) -C $(MANDATORY_DIR) clean
	@$(MAKE) -C $(BONUS_DIR) clean
	@echo "$(GREEN)✓ Object files cleaned!$(RESET)"


fclean:
	@echo "$(YELLOW)Cleaning everything...$(RESET)"
	@$(MAKE) -C $(MANDATORY_DIR) fclean
	@$(MAKE) -C $(BONUS_DIR) fclean
	@echo "$(GREEN)✓ Everything cleaned!$(RESET)"

re: fclean mandatory

re_bonus: fclean bonus

.PHONY: help all mandatory bonus clean fclean re re_bonus
