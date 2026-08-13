CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude

NAME = saloShell


OBJ = obj/main.o obj/tokenizer.o obj/token.o obj/utils.o 

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -rf obj