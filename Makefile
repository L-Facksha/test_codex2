NAME = codixion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRC = \
	src/main.c \
	src/parsing.c \
	src/init.c \
	src/cleanup.c \
	src/fail_dongles.c \
	src/heap.c \
	src/heap_utils.c \
	src/get_dongels_sched.c \
	src/remove_heap_req_dched.c \
	src/scheduler.c \
	src/simulation.c \
	src/coder.c \
	src/monitor.c \
	src/timer.c \
	src/utils.c \
	src/utils_print.c

OBJ = $(SRC:.c=.o)

HEADER = include/codixion.h

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

run: $(NAME)
	./$(NAME)

.PHONY: all clean fclean re run