CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude

NAME = saloShell


OBJ = obj/linked_list.o obj/token.o obj/command.o obj/redirection.o obj/tokenizer.o obj/parser.o obj/utils.o obj/main.o

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -rf obj