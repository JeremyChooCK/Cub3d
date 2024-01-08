NAME = cub3D
CC = cc
CFLAGS = -Wall -Wextra -Werror -Wshadow -g
INCLUDES = -Iinc -Iminilibx-linux
BUILDDIR = build

LIBFT = ./libft/libft.a
LIBMLX = ./minilibx-linux/libmlx.a
LIBRARIES = -L./libft -lft -L./minilibx-linux -lmlx -lXext -lX11 -lm -lz

SRC = main.c \

SRC := $(addprefix src/,$(SRC))
OBJS = $(SRC:src/%.c=$(BUILDDIR)/%.o)

all: $(LIBFT) $(LIBMLX) $(BUILDDIR) $(NAME)

$(LIBFT):
	make -s -C libft

$(LIBMLX):
	make -s -C minilibx-linux

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/%.o: src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LIBFT) $(LIBMLX) $(LIBRARIES) $(INCLUDES) -o $(NAME)

clean:
	rm -rf $(BUILDDIR)
	make -C libft fclean
	make -C minilibx-linux clean

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
