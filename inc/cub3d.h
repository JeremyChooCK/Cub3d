/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegoh <jegoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 21:34:32 by jegoh             #+#    #+#             */
/*   Updated: 2024/01/12 23:05:47 by jegoh            ###   ########.fr       */
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
# define MOVE_SPEED 0.1
# define ROT_SPEED 0.05

# define WIN_WIDTH 800
# define WIN_HEIGHT 600
# define IMG_WIDTH 400
# define IMG_HEIGHT 300
# define TILE_SIZE 32
# define ROWS 11
# define COLS 15
# define WIDTH 480
# define HEIGHT 352

// Red:   Alpha 00, Red FF, Green 00, Blue 00
# define RED 0x00FF0000
// Green: Alpha 00, Red 00, Green FF, Blue 00
# define GREEN 0x0000FF00
// Blue:  Alpha 00, Red 00, Green 00, Blue FF
# define BLUE 0x000000FF
// White: Alpha 00, Red FF, Green FF, Blue FF
# define WHITE 0x00FFFFFF
// Black: Alpha 00, Red 00, Green 00, Blue 00
# define BLACK 0x00000000
// Yellow: Alpha 00, Red FF, Green FF, Blue 00
# define YELLOW 0x00FFFF00

typedef struct s_img
{
	void	*img;
	int		*data;
	int		size_l;
	int		bpp;
	int		endian;
}	t_img;

typedef struct s_player
{
	double	x;
	double	y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
}	t_player;

// Pointer to texture data
typedef struct s_texture
{
	int	*data;
	int	width;
	int	height;
}	t_texture;

typedef struct s_game
{
	void		*mlx;
	void		*win;
	t_img		img;
	t_map		*read_map;
	int			map[ROWS][COLS];
	t_player	player;
	t_texture	textures[4];
}	t_game;

typedef struct s_point
{
	double	x;
	double	y;
}	t_point;

int		ft_close(t_game *game);
int		deal_key(int key_code, t_game *game);
void	clear_image(t_game *game);
void	raycasting(t_game *game);
void	player_init(t_game *game);
void	load_texture(t_game *game, int tex_index, char *path, void *mlx);
void	game_init(t_game *game);
void	window_init(t_game *game);
void	img_init(t_game *game);
int		main_loop(t_game *game);
void	check_arguments(int argc, char **argv);

#endif
