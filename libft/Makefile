NAME = libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror -Wshadow -g
INCLUDE = ../inc
BUILDDIR = build

SRC = ft_isalnum.c \
	ft_isdigit.c \
	ft_itoa.c \
	ft_atoi.c \
	ft_bzero.c \
	ft_calloc.c \
	ft_freesplit.c \
	ft_putstr_fd.c \
	ft_realloc.c \
	ft_strnew.c \
	ft_strchr.c \
	ft_strrchr.c \
	ft_strdup.c \
	ft_strncat.c \
	ft_strndup.c \
	ft_memcpy.c \
	ft_memmove.c \
	ft_memset.c \
	ft_split.c \
	ft_strcmp.c \
	ft_strjoin.c \
	ft_strncmp.c \
	ft_strnlen.c \
	ft_max.c \
	ft_min.c \
	ft_strcat.c \
	ft_strcpy.c \
	ft_strlcpy.c \
	ft_strlen.c \
	ft_strncpy.c \
	ft_strstr.c \
	ft_strtok.c \
	ft_substr.c \
	ft_lstnew.c \
	ft_lstadd_front.c \
	ft_lstsize.c \
	ft_lstlast.c \
	ft_lstadd_back.c \
	ft_lstdelone.c \
	ft_lstclear.c \
	ft_lstiter.c \
	ft_lstmap.c \
	get_next_line.c \

OBJS := $(SRC:%.c=$(BUILDDIR)/%.o)

all: $(BUILDDIR) $(NAME)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

$(NAME): $(OBJS)
	ar rc $(NAME) $(OBJS)
	ranlib $(NAME)

clean:
	rm -rf $(BUILDDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
