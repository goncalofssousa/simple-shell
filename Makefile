CC = gcc

CFLAGS = -Wall -Wextra -Werror -g -Iinclude

NAME = simpleShell

OBJ = obj/linked_list.o \
      obj/token.o \
      obj/command.o \
      obj/redirection.o \
      obj/tokenizer.o \
      obj/parser.o \
      obj/execution.o \
      obj/utils.o \
      obj/main.o


all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@mkdir -p tmp

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj tmp

fclean: clean
	rm -f $(NAME)

re: fclean all