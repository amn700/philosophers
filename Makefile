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
.PHONY: all
all: mandatory

# Help target
.PHONY: help
help:
	@echo "$(CYAN)========================================$(RESET)"
	@echo "$(CYAN)         PHILOSOPHERS PROJECT           $(RESET)"
	@echo "$(CYAN)========================================$(RESET)"
	@echo ""
	@echo "$(YELLOW)Available targets:$(RESET)"
	@echo "  $(GREEN)mandatory$(RESET)    - Build mandatory version (threads + mutexes)"
	@echo "  $(GREEN)bonus$(RESET)        - Build bonus version (processes + semaphores)"
	@echo "  $(GREEN)all$(RESET)          - Build mandatory version (default)"
	@echo "  $(GREEN)both$(RESET)         - Build both mandatory and bonus versions"
	@echo "  $(GREEN)clean$(RESET)        - Clean object files from both versions"
	@echo "  $(GREEN)fclean$(RESET)       - Clean everything (objects + executables)"
	@echo "  $(GREEN)re$(RESET)           - Clean and rebuild mandatory"
	@echo "  $(GREEN)re_bonus$(RESET)     - Clean and rebuild bonus"
	@echo "  $(GREEN)re_both$(RESET)      - Clean and rebuild both versions"
	@echo "  $(GREEN)norm$(RESET)         - Check coding style (if norminette available)"
	@echo "  $(GREEN)help$(RESET)         - Show this help message"
	@echo ""

# Build mandatory version
.PHONY: mandatory
mandatory:
	@echo "$(BLUE)Building mandatory version...$(RESET)"
	@$(MAKE) -C $(MANDATORY_DIR)
	@echo "$(GREEN)✓ Mandatory version built successfully!$(RESET)"
	@echo "$(CYAN)Usage: ./$(MANDATORY_DIR)/$(MANDATORY_NAME) <nb_philo> <time_to_die> <time_to_eat> <time_to_sleep> [nb_must_eat]$(RESET)"

# Build bonus version
.PHONY: bonus
bonus:
	@echo "$(BLUE)Building bonus version...$(RESET)"
	@$(MAKE) -C $(BONUS_DIR)
	@echo "$(GREEN)✓ Bonus version built successfully!$(RESET)"
	@echo "$(CYAN)Usage: ./$(BONUS_DIR)/$(BONUS_NAME) <nb_philo> <time_to_die> <time_to_eat> <time_to_sleep> [nb_must_eat]$(RESET)"

# Build both versions
.PHONY: both
both: mandatory bonus
	@echo "$(GREEN)✓ Both versions built successfully!$(RESET)"

# Clean object files
.PHONY: clean
clean:
	@echo "$(YELLOW)Cleaning object files...$(RESET)"
	@$(MAKE) -C $(MANDATORY_DIR) clean
	@$(MAKE) -C $(BONUS_DIR) clean
	@echo "$(GREEN)✓ Object files cleaned!$(RESET)"

# Clean everything
.PHONY: fclean
fclean:
	@echo "$(YELLOW)Cleaning everything...$(RESET)"
	@$(MAKE) -C $(MANDATORY_DIR) fclean
	@$(MAKE) -C $(BONUS_DIR) fclean
	@echo "$(GREEN)✓ Everything cleaned!$(RESET)"

# Rebuild mandatory
.PHONY: re
re: fclean mandatory

# Rebuild bonus
.PHONY: re_bonus
re_bonus: fclean bonus

# Rebuild both
.PHONY: re_both
re_both: fclean both

# Style check (if norminette is available)
.PHONY: norm
norm:
	@echo "$(CYAN)Checking coding style...$(RESET)"
	@if command -v norminette >/dev/null 2>&1; then \
		norminette $(MANDATORY_DIR)/ $(BONUS_DIR)/; \
	else \
		echo "$(YELLOW)norminette not found, skipping style check$(RESET)"; \
	fi

# Show project status
.PHONY: status
# status:
# 	@echo "$(CYAN)========================================$(RESET)"
# 	@echo "$(CYAN)          PROJECT STATUS                $(RESET)"
# 	@echo "$(CYAN)========================================$(RESET)"
# 	@echo ""
# 	@if [ -f $(MANDATORY_DIR)/$(MANDATORY_NAME) ]; then \
# 		echo "$(GREEN)✓ Mandatory version: BUILT$(RESET)"; \
# 	else \
# 		echo "$(RED)✗ Mandatory version: NOT BUILT$(RESET)"; \
# 	fi
# 	@if [ -f $(BONUS_DIR)/$(BONUS_NAME) ]; then \
# 		echo "$(GREEN)✓ Bonus version: BUILT$(RESET)"; \
# 	else \
# 		echo "$(RED)✗ Bonus version: NOT BUILT$(RESET)"; \
# 	fi
# 	@echo ""
# 	@echo "$(YELLOW)File structure:$(RESET)"
# 	@ls -la $(MANDATORY_DIR)/ | head -10
# 	@echo ""
# 	@ls -la $(BONUS_DIR)/ | head -10
