NAME = philo

SRC_DIR = ./src/
OBJ_DIR = ./obj/

CFLAGS = -Wall -Werror -Wextra
CC = gcc

SRC_NAME = args_check.c \
    init_simulation.c \
	philo_routine.c \
	ft_atoi.c \
    main.c \
	philo_watcher.c \
	handle_errors.c \
	parse.c

OBJ_NAME = $(SRC_NAME:.c=.o)
HEDAERS = ./header/philosopher.h

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