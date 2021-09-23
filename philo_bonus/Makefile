NAME = philo_bonus

SRC_DIR = ./src/
OBJ_DIR = ./obj/

CFLAGS = -Wall -Werror -Wextra
CC = gcc

SRC_NAME = args_check_bonus.c \
    init_simulation_bonus.c \
	philo_routine_bonus.c \
	ft_atoi_bonus.c \
    main_bonus.c \
	philo_watcher_bonus.c \
	handle_errors_bonus.c \
	parse_bonus.c

OBJ_NAME = $(SRC_NAME:.c=.o)
HEDAERS = ./header/philosopher_bonus.h

OBJ = $(addprefix $(OBJ_DIR)/,$(OBJ_NAME))
all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -lpthread -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEDAERS)
	@mkdir obj 2> /dev/null || true
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)
re: fclean all