/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegoh <jegoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 21:34:32 by jegoh             #+#    #+#             */
/*   Updated: 2024/01/10 22:50:45 by jegoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef CUB3D_H
# define CUB3D_H

# include "libft.h"
# include "mlx.h"
# include <math.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# define X_EVENT_KEY_PRESS 2
# define X_EVENT_KEY_RELEASE 3
# define X_EVENT_KEY_EXIT 17

# define KEY_ESC 0xff1b
# define KEY_W 0x77
# define KEY_A 0x61
# define KEY_S 0x73
# define KEY_D 0x64
# define KEY_UP 0xff52
# define KEY_DOWN 0xff54
# define KEY_LEFT 0xff51
# define KEY_RIGHT 0xff53

// TODO cleanup unnessary macro over time
# define WIN_WIDTH 800
# define WIN_HEIGHT 600
# define IMG_WIDTH 400
# define IMG_HEIGHT 300
# define TILE_SIZE 32
# define ROWS 11
# define COLS 15
# define WIDTH 480
# define HEIGHT 352

typedef struct s_img
{
	void	*img;
	int		*data;
	int		size_l;
	int		bpp;
	int		endian;
}	t_img;

// TODO consider malloc the map
typedef struct s_game
{
	void	*mlx;
	void	*win;
	t_img	img;
	char	map[ROWS][COLS];
}	t_game;

typedef struct s_point
{
	double	x;
	double	y;
}	t_point;

#endif
