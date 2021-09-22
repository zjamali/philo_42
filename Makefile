NAME = philo
BONUS = philo_bonus

SRC_MANDATORY_DIR = ./src/mandatory/
OBJ_MANDATORY_DIR = ./obj/mandatory/

SRC_COMMUN_DIR = ./src/commun_functions
OBJ_COMMUN_DIR = ./obj/commun_functions

SRC_BONUS_DIR = ./src/bonus
OBJ_BONUS_DIR = ./obj/bonus

CFLAGS = -Wall -Werror -Wextra
CC = gcc

SRC_MANDATORY = main.c\
	init_simulation.c\
	philo_routine.c\
	philo_watcher.c\
	parse.c

SRC_COMMUN = ft_atoi.c\
	handle_errors.c\
	args_check.c

SRC_BONUS = main_bonus.c\
	init_simulation_bonus.c\
	philo_routine_bonus.c\
	philo_watcher_bonus.c\
	parse_bonus.c


HEDAERS = headers/philosopher.h
HEADERS_BONUS = headers/philosopher_bonus.h
OBJ_MANDATORY := $(SRC_MANDATORY:.c=.o)
OBJ_MANDATORY := $(addprefix $(OBJ_MANDATORY_DIR)/,$(OBJ_MANDATORY))

OBJ_COMMUN := $(SRC_COMMUN:.c=.o)
OBJ_COMMUN := $(addprefix $(OBJ_COMMUN_DIR)/,$(OBJ_COMMUN))

OBJ_BONUS := $(SRC_BONUS:.c=.o)
OBJ_BONUS := $(addprefix $(OBJ_BONUS_DIR)/,$(OBJ_BONUS))

all: $(NAME)

$(NAME): $(OBJ_MANDATORY) $(OBJ_COMMUN)
	$(CC) $^ -lpthread -o $@

$(OBJ_MANDATORY_DIR)/%.o: $(SRC_MANDATORY_DIR)/%.c $(HEDAERS)
	@mkdir obj 2> /dev/null || true
	@mkdir  obj/mandatory 2> /dev/null || true
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_COMMUN_DIR)/%.o: $(SRC_COMMUN_DIR)/%.c $(HEDAERS)
	@mkdir obj 2> /dev/null || true
	@mkdir obj/commun_functions 2> /dev/null || true
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_BONUS_DIR)/%.o: $(SRC_BONUS_DIR)/%.c $(HEADERS_BONUS)
	@mkdir obj 2> /dev/null || true
	@mkdir obj/bonus 2> /dev/null || true
	$(CC) $(CFLAGS) -c $< -o $@

bonus : $(BONUS)

$(BONUS): $(OBJ_BONUS) $(OBJ_COMMUN)
	$(CC) $^ -lpthread -o $@

clean:
	rm -rf $(OBJ_DIR)/*.o
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all