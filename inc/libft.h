/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegoh <jegoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 21:34:32 by jegoh             #+#    #+#             */
/*   Updated: 2024/01/11 21:26:52 by jegoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 100
# endif

typedef struct s_map
{
	char			*content;
	struct s_map	*next;
}	t_map;

char	*get_next_line(int fd);

int		ft_atoi(const char *str);
void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t count, size_t size);
int		ft_isalnum(int c);
int		ft_isdigit(int c);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	*ft_memset(void *b, int c, size_t len);
void	*ft_realloc(void *ptr, size_t size);
char	*ft_strcat(char *dest, const char *src);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
char	*ft_strcpy(char *dest, const char *src);
size_t	ft_strlcpy(char *dest, const char *src, size_t size);
char	*ft_strncpy(char *dest, const char *src, size_t n);
char	*ft_strnew(size_t size);
char	*ft_strstr(const char *str, const char *to_find);
size_t	ft_strlen(const char *str);
size_t	ft_strnlen(const char *s, size_t maxlen);
char	*ft_strjoin( char const *s1, char const *s2);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strdup(const char *s1);
char	*ft_strndup(const char *s, size_t n);
char	*ft_strtok(char *str, char sepa);

void	ft_freesplit(char **s);
char	*ft_itoa(int n);
int		ft_min(int a, int b);
int		ft_max(int a, int b);
void	ft_putstr_fd(char *s, int fd);
char	**ft_split(char const *s, char c);
char	*ft_substr(const char *s, unsigned int start, size_t len);

t_map	*ft_lstnew(void *content);
void	ft_lstadd_front(t_map **lst, t_map *new);
int		ft_lstsize(t_map *lst);
t_map	*ft_lstlast(t_map *lst);
void	ft_lstadd_back(t_map **lst, t_map *new);
void	ft_lstdelone(t_map *lst, void (*del)(void *));
void	ft_lstclear(t_map **lst, void (*del)(void *));
void	ft_lstiter(t_map *lst, void (*f)(void *));
t_map	*ft_lstmap(t_map *lst, void *(*f)(void *), void (*del)(void *));

#endif
