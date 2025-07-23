NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC = src/init.c src/main.c src/monitor.c src/routine.c src/utils.c
OBJ = $(SRC:.c=.o)
INCLUDE = -Iinclude

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

all: $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
