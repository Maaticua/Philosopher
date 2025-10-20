NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDE = -Iinclude

SRC_DIR = src
SRC =	$(SRC_DIR)/cleanup.c	\
		$(SRC_DIR)/init_mutex.c	\
		$(SRC_DIR)/init.c		\
		$(SRC_DIR)/main.c		\
		$(SRC_DIR)/monitor.c	\
		$(SRC_DIR)/parse.c		\
		$(SRC_DIR)/routine.c	\
		$(SRC_DIR)/utils.c		\

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
