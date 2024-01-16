/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegoh <jegoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 21:45:15 by jegoh             #+#    #+#             */
/*   Updated: 2024/01/15 23:17:56 by jegoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "cub3d.h"

void	del(void *content)
{
	free(content);
}

void	free_game(t_game *game)
{
	int	i;

	if (game == NULL)
		return ;
	if (game->mlx)
		free(game->mlx);
	if (game->read_map)
		ft_lstclear(&(game->read_map), del);
	i = 0;
	while (i < game->row)
	{
		if (game->map[i])
			free(game->map[i]);
		i++;
	}
	if (game->map)
		free(game->map);
	i = 0;
	while (i < 4)
	{
		if (game->textures[i].path)
			free(game->textures[i].path);
		i++;
	}
	free(game);
}

// Ensure each color component is within the range of 0-255
unsigned int	rgb_to_bitmask(int rgb[3])
{
	int	i;

	i = 0;
	while (i < 3)
	{
		if (rgb[i] < 0 || rgb[i] > 255)
			return (0);
		i++;
	}
	return ((rgb[0] << 16) | (rgb[1] << 8) | rgb[2]);
}
// Combine the RGB components into a single integer
// Shift red 16 bits left, green 8 bits left, and blue stays as it is

void	ft_close(t_game *game, int exit_code)
{
	int	i;

	if (game && game->mlx)
	{
		i = 0;
		while (i < 4)
		{
			if (game->textures[i].img)
			{
				mlx_destroy_image(game->mlx, game->textures[i].img);
				game->textures[i].img = NULL;
			}
			i++;
		}
		if (game->img.img)
			mlx_destroy_image(game->mlx, game->img.img);
		if (game->win)
			mlx_destroy_window(game->mlx, game->win);
		mlx_destroy_display(game->mlx);
	}
	free_game(game);
	exit(exit_code);
}

int	ft_close_game(t_game *game)
{
	ft_close(game, EXIT_SUCCESS);
	return (0);
}

void	move_forward(t_game *game)
{
	if (!game->map[(int)(game->player.x + game->player.dir_x
			* MOVE_SPEED)][(int)(game->player.y)])
		game->player.x += game->player.dir_x * MOVE_SPEED;
	if (!game->map[(int)(game->player.x)][(int)(game->player.y
		+ game->player.dir_y * MOVE_SPEED)])
		game->player.y += game->player.dir_y * MOVE_SPEED;
}

void	move_backwards(t_game *game)
{
	if (!game->map[(int)(game->player.x - game->player.dir_x
			* MOVE_SPEED)][(int)(game->player.y)])
		game->player.x -= game->player.dir_x * MOVE_SPEED;
	if (!game->map[(int)(game->player.x)][(int)(game->player.y
		- game->player.dir_y * MOVE_SPEED)])
		game->player.y -= game->player.dir_y * MOVE_SPEED;
}

void	move_left(t_game *game, t_dealkey_vars *deal)
{
	deal->leftx = game->player.x - game->player.plane_x * MOVE_SPEED;
	deal->lefty = game->player.y - game->player.plane_y * MOVE_SPEED;
	if (!game->map[(int)deal->leftx][(int)game->player.y])
		game->player.x = deal->leftx;
	if (!game->map[(int)game->player.x][(int)(deal->lefty)])
		game->player.y = deal->lefty;
}

void	move_right(t_game *game, t_dealkey_vars *deal)
{
	deal->rightx = game->player.x + game->player.plane_x * MOVE_SPEED;
	deal->righty = game->player.y + game->player.plane_y * MOVE_SPEED;
	if (!game->map[(int)(deal->rightx)][(int)game->player.y])
		game->player.x = deal->rightx;
	if (!game->map[(int)game->player.x][(int)(deal->righty)])
		game->player.y = deal->righty;
}

void	look_left(t_game *game, t_dealkey_vars *deal)
{
	deal->olddirx = game->player.dir_x;
	game->player.dir_x = game->player.dir_x * cos(ROT_SPEED)
		- game->player.dir_y * sin(ROT_SPEED);
	game->player.dir_y = deal->olddirx * sin(ROT_SPEED)
		+ game->player.dir_y * cos(ROT_SPEED);
	deal->oldplanex = game->player.plane_x;
	game->player.plane_x = game->player.plane_x
		* cos(ROT_SPEED) - game->player.plane_y * sin(ROT_SPEED);
	game->player.plane_y = deal->oldplanex * sin(ROT_SPEED)
		+ game->player.plane_y * cos(ROT_SPEED);
}

void	look_right(t_game *game, t_dealkey_vars *deal)
{
	deal->olddirx = game->player.dir_x;
	game->player.dir_x = game->player.dir_x * cos(-ROT_SPEED)
		- game->player.dir_y * sin(-ROT_SPEED);
	game->player.dir_y = deal->olddirx * sin(-ROT_SPEED)
		+ game->player.dir_y * cos(-ROT_SPEED);
	deal->oldplanex = game->player.plane_x;
	game->player.plane_x = game->player.plane_x
		* cos(-ROT_SPEED) - game->player.plane_y * sin(-ROT_SPEED);
	game->player.plane_y = deal->oldplanex * sin(-ROT_SPEED)
		+ game->player.plane_y * cos(-ROT_SPEED);
}

int	deal_key(int key_code, t_game *game)
{
	t_dealkey_vars	deal;

	if (key_code == KEY_ESC || key_code == KEY_Q)
		ft_close(game, 0);
	else if (key_code == KEY_W)
		move_forward(game);
	else if (key_code == KEY_S)
		move_backwards(game);
	else if (key_code == KEY_A)
		move_left(game, &deal);
	else if (key_code == KEY_D)
		move_right(game, &deal);
	if (key_code == KEY_LEFT)
		look_left(game, &deal);
	else if (key_code == KEY_RIGHT)
		look_right(game, &deal);
	return (0);
}

void	clear_image(t_game *game)
{
	int	x;
	int	y;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			game->img.data[y * WIDTH + x] = 0x000000;
			x++;
		}
		y++;
	}
}

void	draw_floor_and_ceiling(t_game *game)
{
	int	x;
	int	y;

	y = 0;
	while (y < HEIGHT / 2)
	{
		x = 0;
		while (x < WIDTH)
			game->img.data[y * WIDTH + x++]
				= rgb_to_bitmask(game->ceiling_color);
		y++;
	}
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
			game->img.data[y * WIDTH + x++]
				= rgb_to_bitmask(game->floor_color);
		y++;
	}
}

void	dda(t_game *game, t_raycasting *ray)
{
	while (ray->hit == 0)
	{
		if (ray->sidedistx < ray->sidedisty)
		{
			ray->sidedistx += ray->deltadistx;
			ray->mapx += ray->stepx;
			ray->side = 0;
		}
		else
		{
			ray->sidedisty += ray->deltadisty;
			ray->mapy += ray->stepy;
			ray->side = 1;
		}
		if (game->map[ray->mapx][ray->mapy] > 0)
			ray->hit = 1;
	}
}

void	assign_ray_vars(t_game *game, t_raycasting *ray, int x)
{
	ray->camerax = 2 * x / (double)WIDTH - 1;
	ray->raydirx = game->player.dir_x + game->player.plane_x * ray->camerax;
	ray->raydiry = game->player.dir_y + game->player.plane_y * ray->camerax;
	ray->mapx = (int)game->player.x;
	ray->mapy = (int)game->player.y;
	ray->deltadistx = fabs(1 / ray->raydirx);
	ray->deltadisty = fabs(1 / ray->raydiry);
	ray->hit = 0;
}

void	step_and_sidedist_calc(t_game *game, t_raycasting *ray)
{
	if (ray->raydirx < 0)
	{
		ray->stepx = -1;
		ray->sidedistx = (game->player.x - ray->mapx) * ray->deltadistx;
	}
	else
	{
		ray->stepx = 1;
		ray->sidedistx = (ray->mapx + 1.0 - game->player.x) * ray->deltadistx;
	}
	if (ray->raydiry < 0)
	{
		ray->stepy = -1;
		ray->sidedisty = (game->player.y - ray->mapy) * ray->deltadisty;
	}
	else
	{
		ray->stepy = 1;
		ray->sidedisty = (ray->mapy + 1.0 - game->player.y) * ray->deltadisty;
	}
}

void	calc_wall_distance(t_game *game, t_raycasting *ray)
{
	if (ray->side == 0)
		ray->perpwalldist = (ray->mapx - game->player.x
				+ (1 - ray->stepx) / 2) / ray->raydirx;
	else
		ray->perpwalldist = (ray->mapy - game->player.y
				+ (1 - ray->stepy) / 2) / ray->raydiry;
}

void	assign_texture(t_raycasting *ray)
{
	if (ray->side == 0)
	{
		if (ray->raydirx > 0)
			ray->texnum = 3;
		else
			ray->texnum = 2;
	}
	else
	{
		if (ray->raydiry > 0)
			ray->texnum = 0;
		else
			ray->texnum = 1;
	}
}

void	calc_tex_coords(t_game *game, t_raycasting *ray, int *y)
{
	if (ray->side == 0)
		ray->wallx = game->player.y + ray->perpwalldist * ray->raydiry;
	else
		ray->wallx = game->player.x + ray->perpwalldist * ray->raydirx;
	ray->wallx -= floor(ray->wallx);
	ray->texwidth = game->textures[ray->texnum].width;
	ray->texheight = game->textures[ray->texnum].height;
	ray->texx = (int)(ray->wallx * (double)(ray->texwidth));
	if (ray->side == 0 && ray->raydirx > 0)
		ray->texx = ray->texwidth - ray->texx - 1;
	if (ray->side == 1 && ray->raydiry < 0)
		ray->texx = ray->texwidth - ray->texx - 1;
	ray->step = 1.0 * ray->texheight / ray->lineheight;
	ray->texpos = (ray->drawstart - HEIGHT / 2 + ray->lineheight / 2)
		* ray->step;
	*y = ray->drawstart;
	ray->step = 1.0 * ray->texheight / ray->lineheight;
	ray->texpos = (ray->drawstart - HEIGHT / 2 + ray->lineheight / 2)
		* ray->step;
	*y = ray->drawstart;
}

void	draw_onto_image(t_game *game, t_raycasting *ray, int *y, int *x)
{
	int	a;

	a = *y;
	while (a < ray->drawend)
	{
		ray->texy = (int)ray->texpos & (ray->texheight - 1);
		ray->texpos += ray->step;
		ray->color
			= game->textures[ray->texnum].data[
			ray->texheight * ray->texy + ray->texx];
		if (ray->side == 1)
			ray->color = (ray->color >> 1) & 8355711;
		game->img.data[a * WIDTH + *x] = ray->color;
		a++;
	}
}

void	calc_vertical_line_coords(t_raycasting *ray)
{
	ray->lineheight = (int)(HEIGHT / ray->perpwalldist);
	ray->drawstart = -ray->lineheight / 2 + HEIGHT / 2;
	if (ray->drawstart < 0)
		ray->drawstart = 0;
	ray->drawend = ray->lineheight / 2 + HEIGHT / 2;
	if (ray->drawend >= HEIGHT)
		ray->drawend = HEIGHT - 1;
}

void	raycasting(t_game *game)
{
	int				x;
	int				y;
	t_raycasting	ray;

	clear_image(game);
	draw_floor_and_ceiling(game);
	x = 0;
	while (x < WIDTH)
	{
		assign_ray_vars(game, &ray, x);
		step_and_sidedist_calc(game, &ray);
		dda(game, &ray);
		calc_wall_distance(game, &ray);
		calc_vertical_line_coords(&ray);
		assign_texture(&ray);
		calc_tex_coords(game, &ray, &y);
		draw_onto_image(game, &ray, &y, &x);
		x++;
	}
}

void	set_player_north(t_game *game, int i, int j)
{
	game->player.dir_x = -1;
	game->player.dir_y = 0;
	game->player.plane_x = 0;
	game->player.plane_y = 0.66;
	game->map[i][j] = 0;
}

void	set_player_south(t_game *game, int i, int j)
{
	game->player.dir_x = 1;
	game->player.dir_y = 0;
	game->player.plane_x = 0;
	game->player.plane_y = -0.66;
	game->map[i][j] = 0;
}

void	set_player_east(t_game *game, int i, int j)
{
	game->player.dir_x = 0;
	game->player.dir_y = 1;
	game->player.plane_x = 0.66;
	game->player.plane_y = 0;
	game->map[i][j] = 0;
}

void	set_player_west(t_game *game, int i, int j)
{
	game->player.dir_x = 0;
	game->player.dir_y = -1;
	game->player.plane_x = -0.66;
	game->player.plane_y = 0;
	game->map[i][j] = 0;
}

void	set_player_direction(t_game *game, int i, int j)
{
	game->player.x = i + 0.5;
	game->player.y = j + 0.5;
	if (game->map[i][j] == 'N')
		set_player_north(game, i, j);
	else if (game->map[i][j] == 'S')
		set_player_south(game, i, j);
	else if (game->map[i][j] == 'E')
		set_player_east(game, i, j);
	else if (game->map[i][j] == 'W')
		set_player_west(game, i, j);
}

void	player_init(t_game *game)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < game->row)
	{
		j = 0;
		while (j < game->col)
		{
			if (game->map[i][j] == 'N' || game->map[i][j] == 'S'
				|| game->map[i][j] == 'E' || game->map[i][j] == 'W')
			{
				set_player_direction(game, i, j);
				return ;
			}
			j++;
		}
		i++;
	}
}

void	print_error_and_close(t_game *game, char *path,
	t_load_texture_vars *textvar)
{
	perror("Error");
	ft_putstr_fd("\n", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd("\n", 2);
	close(textvar->fd);
	ft_close(game, errno);
}

// Function to load a texture from an XPM file
void	load_texture(t_game *game, int tex_index, char *path, void *mlx)
{
	t_texture			*tex;
	t_load_texture_vars	textvar;

	tex = &game->textures[tex_index];
	textvar.fd = open(path, O_RDONLY);
	if (textvar.fd == -1)
		print_error_and_close(game, path, &textvar);
	close(textvar.fd);
	game->textures[tex_index].img
		= mlx_xpm_file_to_image(mlx, path, &(textvar.width), &(textvar.height));
	game->textures[tex_index].data
		= (int *)mlx_get_data_addr(
			game->textures[tex_index].img,
			&(textvar.bpp), &(textvar.size_l), &(textvar.endian));
	if (!tex->data)
	{
		ft_putstr_fd("Error\nUnable to load texture\n", 2);
		ft_close(game, EXIT_FAILURE);
	}
	tex->width = textvar.width;
	tex->height = textvar.height;
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
	if (game->player.x < 0 || game->player.x > game->row
		|| game->player.y < 0 || game->player.y > game->col)
	{
		ft_putstr_fd("Error\nPlayer is out of boundary.\n", 2);
		ft_close(game, EXIT_FAILURE);
	}
	raycasting(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img.img, 0, 0);
	return (0);
}

void	print_error_and_close2(t_game *game, char **argv)
{
	ft_putstr_fd("Error\nUsage: ", 2);
	ft_putstr_fd(argv[0], 2);
	ft_putstr_fd(" <map_file.cub>\n", 2);
	ft_close(game, EXIT_FAILURE);
}

void	check_arguments(int argc, char **argv, t_game *game)
{
	int		fd;
	char	*extension;

	if (argc != 2)
		print_error_and_close2(game, argv);
	extension = ft_strrchr(argv[1], '.');
	if (extension == NULL || ft_strcmp(extension, ".cub") != 0)
	{
		ft_putstr_fd("Error\nMap file must have a .cub extension\n", 2);
		ft_putstr_fd(extension, 2);
		ft_putstr_fd("\n", 2);
		ft_close(game, EXIT_FAILURE);
	}
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		perror("Error");
		ft_putstr_fd("\n", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd("\n", 2);
		close(fd);
		ft_close(game, errno);
	}
	close(fd);
}

void	check_incorrect_number(int i, char **str, char **colors, t_game *game)
{
	if (i != 3)
	{
		perror("Error\nIncorrect number of color values\n");
		if (str)
			ft_freesplit(str);
		if (colors)
			ft_freesplit(colors);
		ft_close(game, EXIT_FAILURE);
	}
}

void	check_ceiling_and_floor(char texture, char **colors,
	t_game *game, char **str)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		if (texture == 'F')
			game->floor_color[i] = ft_atoi(colors[i]);
		else if (texture == 'C')
			game->ceiling_color[i] = ft_atoi(colors[i]);
		if (ft_atoi(colors[i]) < 0 || ft_atoi(colors[i]) > 255)
		{
			perror("Error\nColor values must be between 0 and 255\n");
			ft_freesplit(str);
			ft_freesplit(colors);
			ft_close(game, EXIT_FAILURE);
		}
		i++;
	}
}

void	parse_color(t_game *game, char **str, char texture)
{
	char	**colors;
	int		i;

	if (str == NULL)
	{
		perror("Error\nInput rgb string is NULL\n");
		ft_close(game, EXIT_FAILURE);
	}
	colors = ft_split(str[1], ',');
	if (colors == NULL)
	{
		perror("Error\nMemory allocation failed\n");
		ft_freesplit(str);
		ft_close(game, EXIT_FAILURE);
	}
	i = 0;
	while (colors[i] != NULL)
		i++;
	check_incorrect_number(i, str, colors, game);
	i = 0;
	check_ceiling_and_floor(texture, colors, game, str);
	ft_freesplit(colors);
}

void	print_game_map(t_game *game)
{
	int	i;
	int	j;

	i = 0;
	printf("Treat '.' as empty space\n");
	while (i < game->row)
	{
		j = 0;
		while (j < game->col)
		{
			if (game->map[i][j] == 1 || game->map[i][j] == 0)
				printf("%d", game->map[i][j]);
			else if (game->map[i][j] == ' ')
				printf(".");
			else
				printf("%c", game->map[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
}

void	free_visited_array(t_game *game, int **visited)
{
	int	i;

	i = 0;
	while (i < game->row)
	{
		if (visited[i])
			free(visited[i]);
		i++;
	}
	free(visited);
}

void	check_player_err(t_game *game, int i, int j, int **visited)
{
	if (i == 0 || i == game->row - 1
		|| j == 0 || j == game->col - 1)
	{
		perror("Error\nPlayer location is OOB\n");
		free_visited_array(game, visited);
		ft_close(game, EXIT_FAILURE);
	}
}

void	find_player_position(
	t_game *game, int *playerx, int *playery, int **visited)
{
	int	i;
	int	j;

	i = 0;
	while (i < game->row)
	{
		j = 0;
		while (j < game->col)
		{
			if (game->map[i][j] == 'N' || game->map[i][j] == 'S'
				|| game->map[i][j] == 'E' || game->map[i][j] == 'W')
			{
				check_player_err(game, i, j, visited);
				*playerx = i;
				*playery = j;
				return ;
			}
			j++;
		}
		i++;
	}
}

void	flood_fill(t_game *game, int x, int y, int **visited)
{
	if (game->map[x][y] == 1 || visited[x][y])
		return ;
	if (x <= 0 || x >= game->row - 1 || y <= 0 || y >= game->col - 1
		|| game->map[x][y] == ' ')
	{
		ft_putstr_fd("Error\nMap is not enclosed\n", 2);
		free_visited_array(game, visited);
		ft_close(game, EXIT_FAILURE);
	}
	visited[x][y] = 1;
	flood_fill(game, x + 1, y, visited);
	flood_fill(game, x - 1, y, visited);
	flood_fill(game, x, y + 1, visited);
	flood_fill(game, x, y - 1, visited);
}

void	assign_visited_array(t_game *game, int **visited)
{
	int	i;
	int	j;

	i = 0;
	while (i < game->row)
	{
		visited[i] = malloc(game->col * sizeof(int));
		if (!visited[i])
		{
			perror("Error\nMemory allocation failed for visited row\n");
			j = 0;
			while (j < i)
				free(visited[j++]);
			free(visited);
			ft_close(game, EXIT_FAILURE);
		}
		j = 0;
		while (j < game->col)
			visited[i][j++] = 0;
		i++;
	}
}

int	**create_visited_array(t_game *game)
{
	int	**visited;

	visited = malloc(game->row * sizeof(int *));
	if (!visited)
	{
		perror("Error\nMemory allocation failed for visited array\n");
		ft_close(game, EXIT_FAILURE);
	}
	assign_visited_array(game, visited);
	return (visited);
}

void	is_map_fully_enclosed(t_game *game)
{
	int	**visited;
	int	playerx;
	int	playery;

	visited = create_visited_array(game);
	playerx = 1;
	playery = 1;
	find_player_position(game, &playerx, &playery, visited);
	flood_fill(game, playerx, playery, visited);
	free_visited_array(game, visited);
}

void	parse_textures(t_game *game, t_cub *cub)
{
	if (ft_strcmp(cub->split_line[0], "NO") == 0)
		game->textures[0].path = ft_strdup(cub->split_line[1]);
	else if (ft_strcmp(cub->split_line[0], "SO") == 0)
		game->textures[1].path = ft_strdup(cub->split_line[1]);
	else if (ft_strcmp(cub->split_line[0], "EA") == 0)
		game->textures[2].path = ft_strdup(cub->split_line[1]);
	else if (ft_strcmp(cub->split_line[0], "WE") == 0)
		game->textures[3].path = ft_strdup(cub->split_line[1]);
	else if (ft_strcmp(cub->split_line[0], "F") == 0)
		parse_color(game, cub->split_line, 'F');
	else if (ft_strcmp(cub->split_line[0], "C") == 0)
		parse_color(game, cub->split_line, 'C');
	else
	{
		ft_putstr_fd("Error\nInvalid param line in .cub file\n", 2);
		ft_putstr_fd(cub->line, 2);
		ft_putstr_fd("\n", 2);
		ft_freesplit(cub->split_line);
		game->textures[0].path = NULL;
		game->textures[1].path = NULL;
		game->textures[2].path = NULL;
		game->textures[3].path = NULL;
		ft_close(game, EXIT_FAILURE);
	}
}

void	err_invalid_line(t_game *game, t_cub *cub)
{
	ft_putstr_fd("Error\nInvalid line in .cub file\n", 2);
	ft_putstr_fd(cub->line, 2);
	ft_putstr_fd("\n", 2);
	ft_freesplit(cub->split_line);
	ft_close(game, EXIT_FAILURE);
}

void	check_map(t_game *game, t_cub *cub)
{
	if (ft_strlen(cub->line) == 0)
	{
		perror("Error\nEmpty line in .cub map file\n");
		ft_freesplit(cub->split_line);
		ft_close(game, EXIT_FAILURE);
	}
	ft_strcpy(cub->temp_map[game->row], cub->line);
	if ((int)ft_strlen(cub->line) > game->col)
		game->col = ft_strlen(cub->line);
	game->row++;
}

void	check_character(t_game *game, t_cub *cub, int i, int j)
{
	if (cub->temp_map[i][j] == '0')
		game->map[i][j] = 0;
	else if (cub->temp_map[i][j] == '1')
		game->map[i][j] = 1;
	else if (cub->temp_map[i][j] == 'N')
		game->map[i][j] = 'N';
	else if (cub->temp_map[i][j] == 'S')
		game->map[i][j] = 'S';
	else if (cub->temp_map[i][j] == 'E')
		game->map[i][j] = 'E';
	else if (cub->temp_map[i][j] == 'W')
		game->map[i][j] = 'W';
	else if (cub->temp_map[i][j] == ' '
		|| ((int)ft_strlen(cub->temp_map[i]) < game->col
		&& j >= (int)ft_strlen(cub->temp_map[i])))
		game->map[i][j] = ' ';
	else
	{
		ft_putstr_fd("Error\nInvalid character in map\n", 2);
		ft_putstr_fd(cub->temp_map[i], 2);
		ft_putstr_fd("\n", 2);
		ft_close(game, EXIT_FAILURE);
	}
}

void	copy_line_to_map(t_game *game, t_cub *cub)
{
	ft_strcpy(cub->temp_map[game->row], cub->line);
	game->col = ft_strlen(cub->line);
	game->row++;
	cub->mapcheck = 1;
}

void	check_line(t_game *game, t_cub *cub)
{
	while (cub->current != NULL)
	{
		cub->line = cub->current->content;
		cub->split_line = ft_split(cub->line, ' ');
		if (!(cub->split_line))
		{
			ft_putstr_fd("Error\nFailed to split line\n", 2);
			ft_close(game, EXIT_FAILURE);
		}
		if (cub->mapcheck == 1)
			check_map(game, cub);
		else if ((ft_strncmp(cub->line, " ", 1) == 0
				|| ft_strncmp(cub->line, "1", 1) == 0
				|| ft_strncmp(cub->line, "1", 1) == 0) && cub->mapcheck == 0)
			copy_line_to_map(game, cub);
		else if (cub->split_line[0] && cub->split_line[1] && cub->mapcheck == 0)
			parse_textures(game, cub);
		else if (ft_strlen(cub->line) > 0 && cub->mapcheck == 0)
			err_invalid_line(game, cub);
		ft_freesplit(cub->split_line);
		cub->current = cub->current->next;
	}
}

void	check_direction(t_game *game, t_cub *cub)
{
	int	i;
	int	j;

	i = 0;
	while (i < game->row)
	{
		j = 0;
		while (j < game->col)
		{
			if (cub->temp_map[i][j] == 'N' || cub->temp_map[i][j] == 'S'
				|| cub->temp_map[i][j] == 'E' || cub->temp_map[i][j] == 'W')
				cub->player_count++;
			if (cub->player_count > 1)
			{
				ft_putstr_fd("Error\nMultiple players in map\n", 2);
				ft_putstr_fd(cub->temp_map[i], 2);
				ft_putstr_fd("\n", 2);
				ft_close(game, EXIT_FAILURE);
			}
			check_character(game, cub, i, j);
			j++;
		}
		i++;
	}
}

void	make_map(t_game *game)
{
	int	i;

	i = 0;
	while (i < game->row)
	{
		game->map[i] = malloc(game->col * sizeof(int));
		if (!game->map[i])
		{
			ft_putstr_fd("Error\nMemory allocation failed\n", 2);
			ft_close(game, EXIT_FAILURE);
		}
		i++;
	}
}

void	parse_cub_file(t_game *game)
{
	t_cub	cub;

	game->row = 0;
	game->col = 0;
	cub.mapcheck = 0;
	cub.player_count = 0;
	cub.current = game->read_map;
	ft_memset(cub.temp_map, 0, sizeof(cub.temp_map));
	check_line(game, &cub);
	game->map = malloc(game->row * sizeof(int *));
	if (game->map == NULL)
	{
		ft_putstr_fd("Error\nMemory allocation failed\n", 2);
		ft_close(game, EXIT_FAILURE);
	}
	make_map(game);
	check_direction(game, &cub);
	is_map_fully_enclosed(game);
}

void	init_game_var(t_game **game, int i)
{
	(*game)->mlx = NULL;
	(*game)->win = NULL;
	(*game)->read_map = NULL;
	(*game)->map = NULL;
	(*game)->img.img = NULL;
	(*game)->img.data = NULL;
	(*game)->img.size_l = 0;
	(*game)->img.bpp = 0;
	(*game)->img.endian = 0;
	(*game)->player.x = 0.0;
	(*game)->player.y = 0.0;
	(*game)->player.dir_x = 1.0;
	(*game)->player.dir_y = 0.0;
	(*game)->player.plane_x = 0.0;
	(*game)->player.plane_y = 0.66;
	while (i < 4)
	{
		(*game)->textures[i].data = NULL;
		(*game)->textures[i].img = NULL;
		(*game)->textures[i].width = 0;
		(*game)->textures[i].height = 0;
		i++;
	}
}

void	init_game_struct(t_game **game)
{
	if (*game == NULL)
	{
		perror("Error\nMemory allocation failed\n");
		ft_close(*game, EXIT_FAILURE);
	}
	init_game_var(game, 0);
}

void	init_game(t_game **game, char **argv)
{
	int		fd;
	char	*buff;

	*game = (t_game *)malloc(sizeof(t_game));
	init_game_struct(game);
	window_init(*game);
	fd = open(argv[1], O_RDONLY);
	buff = get_next_line(fd);
	while (buff)
	{
		if (ft_strlen(buff) > 0 && buff[ft_strlen(buff) - 1] == '\n')
			buff[ft_strlen(buff) - 1] = '\0';
		ft_lstadd_back(&((*game)->read_map), ft_lstnew(buff));
		buff = get_next_line(fd);
	}
	free(buff);
	close(fd);
	parse_cub_file(*game);
	player_init(*game);
	load_texture(*game, 0, (*game)->textures[0].path, (*game)->mlx);
	load_texture(*game, 1, (*game)->textures[1].path, (*game)->mlx);
	load_texture(*game, 2, (*game)->textures[2].path, (*game)->mlx);
	load_texture(*game, 3, (*game)->textures[3].path, (*game)->mlx);
	img_init(*game);
}

int	main(int argc, char **argv)
{
	t_game	*game;

	game = NULL;
	check_arguments(argc, argv, game);
	init_game(&game, argv);
	mlx_hook(game->win, X_EVENT_KEY_PRESS, 1L << 0, &deal_key, game);
	mlx_hook(game->win, X_EVENT_KEY_EXIT, 1L << 2, &ft_close_game, game);
	mlx_loop_hook(game->mlx, &main_loop, game);
	mlx_loop(game->mlx);
}
