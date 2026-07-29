NAME		= codexion

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread
INCLUDES	= -Iinclude

SRC_DIR		= src
OBJ_DIR		= obj

SRCS		= main.c \
			  utils.c \
			  parser/parser.c \
			  init/init_coder.c init/init_dongle.c init/init.c \
			  scheduler/scheduler_utils.c scheduler/scheduler.c \
			  sync/dongle.c sync/dongle_utils.c \
			  simulation/coder.c simulation/coder_utils.c \
			  monitor/monitor.c monitor/burnout.c\
			  logger/logger.c \

OBJS		= $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
