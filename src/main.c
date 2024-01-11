/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegoh <jegoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 21:45:15 by jegoh             #+#    #+#             */
/*   Updated: 2024/01/11 19:47:47 by jegoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "cub3d.h"

// This main file need to redo, for now the code are boilerplates.
int	to_coord(int x, int y)
{
	return ((int)floor(y) * WIDTH + (int)floor(x));
}

void	draw_line(t_game *game, t_point start, t_point end)
{
	double	dx;
	double	dy;
	double	step;

	dx = end.x - start.x;
	dy = end.y - start.y;
	step = fabs(dy);
	if (fabs(dx) > fabs(dy))
		step = fabs(dx);
	dx /= step;
	dy /= step;
	while (fabs(end.x - start.x) > 0.01 || fabs(end.y - start.y) > 0.01)
	{
		game->img.data[to_coord(start.x, start.y)] = 0xb3b3b3;
		start.x += dx;
		start.y += dy;
	}
}

t_point	point(double x, double y)
{
	t_point	point;

	point.x = x;
	point.y = y;
	return (point);
}

void	draw_lines(t_game *game)
{
	int		i;
	int		j;

	i = 0;
	while (i < COLS)
	{
		draw_line(game, point(i * TILE_SIZE, 0), point(i * TILE_SIZE, HEIGHT));
		i++;
	}
	draw_line(game, point(COLS * TILE_SIZE - 1, 0),
		point(COLS * TILE_SIZE - 1, HEIGHT));
	j = 0;
	while (j < ROWS)
	{
		draw_line(game, point(0, j * TILE_SIZE), point(WIDTH, j * TILE_SIZE));
		j++;
	}
	draw_line(game, point(0, ROWS * TILE_SIZE - 1),
		point(WIDTH, ROWS * TILE_SIZE - 1));
}

void	draw_rectangle(t_game *game, int x, int y, char c)
{
	int	i;
	int	j;

	x *= TILE_SIZE;
	y *= TILE_SIZE;
	i = 0;
	while (i < TILE_SIZE)
	{
		j = 0;
		while (j < TILE_SIZE)
		{
			if (c == '1')
				game->img.data[(y + i) * WIDTH + x + j] = 0xffffff;
			else if (c == '0')
				game->img.data[(y + i) * WIDTH + x + j] = 0x111111;
			else if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
				game->img.data[(y + i) * WIDTH + x + j] = 0xff1111;
			else
				game->img.data[(y + i) * WIDTH + x + j] = 0xaaaaaa;
			j++;
		}
		i++;
	}
}

void	draw_rectangles(t_game *game)
{
	int	i;
	int	j;

	i = 0;
	while (i < ROWS)
	{
		j = 0;
		while (j < COLS)
		{
			draw_rectangle(game, j, i, game->map[i][j]);
			j++;
		}
		i++;
	}
}

int	ft_close(t_game *game)
{
	mlx_destroy_image(game->mlx, game->img.img);
	mlx_destroy_window(game->mlx, game->win);
	mlx_destroy_display(game->mlx);
	free(game->mlx);
	exit(0);
}

int	deal_key(int key_code, t_game *game)
{
	if (key_code == KEY_ESC)
		ft_close(game);
	return (0);
}

// TODO convert this to linked list
void	game_init(t_game *game)
{
	char	map[ROWS][COLS];

	ft_strcpy(map[0], " 11111111111111");
	ft_strcpy(map[1], "111000000000101");
	ft_strcpy(map[2], "100001000000101");
	ft_strcpy(map[3], "111100000001011");
	ft_strcpy(map[4], "100000000001011");
	ft_strcpy(map[5], "1000000N1111011");
	ft_strcpy(map[6], "100000000000011");
	ft_strcpy(map[7], "100000000000011");
	ft_strcpy(map[8], "111110000111101");
	ft_strcpy(map[9], "100000000001111");
	ft_strcpy(map[10], "111111111111   ");
	ft_memcpy(game->map, map, sizeof(char) * ROWS * COLS);
}

void	window_init(t_game *game)
{
	game->mlx = mlx_init();
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "cub3D");
}

void	img_init(t_game *game)
{
	game->img.img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	game->img.data = (int *)mlx_get_data_addr(game->img.img, &game->img.bpp,
			&game->img.size_l, &game->img.endian);
}

int	main_loop(t_game *game)
{
	draw_rectangles(game);
	draw_lines(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img.img, 0, 0);
	return (0);
}

void	check_arguments(int argc, char **argv)
{
	char	*extension;

	if (argc == 1)
	{
		printf("Argument check temporary disabled.\n");
		return ;
	}
	if (argc != 2)
	{
		ft_putstr_fd("Usage: ", 2);
		ft_putstr_fd(argv[0], 2);
		ft_putstr_fd(" <map_file.cub>\n", 2);
		exit(EXIT_FAILURE);
	}
	extension = ft_strrchr(argv[1], '.');
	if (extension == NULL || ft_strcmp(extension, ".cub") != 0)
	{
		ft_putstr_fd("Error: Map file must have a .cub extension\n", 2);
		exit(EXIT_FAILURE);
	}
}

int	main(int argc, char **argv)
{
	t_game	game;

	check_arguments(argc, argv);
	game_init(&game);
	window_init(&game);
	img_init(&game);
	mlx_hook(game.win, X_EVENT_KEY_PRESS, 1L << 0, &deal_key, &game);
	mlx_hook(game.win, X_EVENT_KEY_EXIT, 1L << 2, &ft_close, &game);
	mlx_loop_hook(game.mlx, &main_loop, &game);
	mlx_loop(game.mlx);
}
