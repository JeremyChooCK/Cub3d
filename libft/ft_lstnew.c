/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 10:39:28 by jechoo            #+#    #+#             */
/*   Updated: 2024/01/11 21:46:37 by jegoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

t_map	*ft_lstnew(void *content)
{
	t_map	*new;

	new = (t_map *)malloc(sizeof(t_map));
	if (!new)
		return (NULL);
	new->content = content;
	new->next = NULL;
	return (new);
}
