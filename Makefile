NAME=philosophers
CC=gcc
CFLAGS=-Wall -Wextra -Werror -I$(INC_DIR)

SRC_DIR=src
INC_DIR=includes

_INC = philosophers.h
INC = $(patsubst %, $(INC_DIR)/%, $(_INC))

OBJ = philosophers.o main.o

all: $(NAME)

%.o: $(SRC_DIR)/%.c $(INC)
	$(CC) -c -o $@ $< $(CFLAGS)

$(NAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o

fclean:
	rm -f *.o $(NAME)

re: fclean all