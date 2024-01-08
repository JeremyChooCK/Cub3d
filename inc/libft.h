/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegoh <jegoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 21:34:32 by jegoh             #+#    #+#             */
/*   Updated: 2024/01/07 13:01:50 by jegoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>

int		ft_atoi(const char *str);
void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t count, size_t size);
void	ft_freesplit(char **s);
int		ft_isalnum(int c);
int		ft_isdigit(int c);
char	*ft_itoa(int n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	*ft_memset(void *b, int c, size_t len);
int		ft_min(int a, int b);
void	ft_putstr_fd(char *s, int fd);
void	*ft_realloc(void *ptr, size_t size);
char	**ft_split(char const *s, char c);
char	**ft_split_quote(char *s, char delimiter);
char	*ft_strcat(char *dest, const char *src);
char	*ft_strchr(const char *s, int c);
char	*ft_strcpy(char *dest, const char *src);
size_t	ft_strlcpy(char *dest, const char *src, size_t size);
char	*ft_strncpy(char *dest, const char *src, size_t n);
char	*ft_strnew(size_t size);
char	*ft_strstr(const char *str, const char *to_find);
size_t	ft_strlen(const char *str);
size_t	ft_strnlen(const char *s, size_t maxlen);
char	*ft_strjoin( char const *s1, char const *s2);
char	*ft_strjoin_space(char **arr);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strdup(const char *s1);
char	*ft_strndup(const char *s, size_t n);
char	*ft_strtok(char *str, char sepa);
char	*ft_substr(const char *s, unsigned int start, size_t len);

#endif
