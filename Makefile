NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
SRCS = 	src/main.c \
		src/parsing.c \
		src/init.c \
		src/simulation.c \
		src/timer.c \
		src/utils.c \
		src/cleanup.c \
		src/fail_dongles.c \
		src/heap.c \
		src/heap_utils.c \
		src/scheduler.c \
		src/coder.c \
		src/monitor.c \
		src/remove_request_from_sch.c \
		src/scheduler_give_take.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c include/codixion.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
