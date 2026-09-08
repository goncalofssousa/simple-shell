CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -Iinclude $(shell pkg-config --cflags glib-2.0)
LIBS = $(shell pkg-config --libs glib-2.0) -lreadline

NAME = simpleShell

SRC = $(shell find src -name "*.c")

OBJ = $(SRC:src/%.c=obj/%.o)


all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LIBS)

obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re