/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegoh <jegoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 21:45:15 by jegoh             #+#    #+#             */
/*   Updated: 2024/01/15 22:15:11 by jegoh            ###   ########.fr       */
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

// TODO: line is too long, need to reduce length.
int	deal_key(int key_code, t_game *game)
{
	double	leftx;
	double	lefty;
	double	rightx;
	double	righty;
	double	olddirx;
	double	oldplanex;

	if (key_code == KEY_ESC || key_code == KEY_Q)
		ft_close(game, 0);
	else if (key_code == KEY_W) // Move forward
	{
		if (!game->map[(int)(game->player.x + game->player.dir_x * MOVE_SPEED)][(int)(game->player.y)])
			game->player.x += game->player.dir_x * MOVE_SPEED;
		if (!game->map[(int)(game->player.x)][(int)(game->player.y + game->player.dir_y * MOVE_SPEED)])
			game->player.y += game->player.dir_y * MOVE_SPEED;
	}
	else if (key_code == KEY_S) // Move backward
	{
		if (!game->map[(int)(game->player.x - game->player.dir_x * MOVE_SPEED)][(int)(game->player.y)])
			game->player.x -= game->player.dir_x * MOVE_SPEED;
		if (!game->map[(int)(game->player.x)][(int)(game->player.y - game->player.dir_y * MOVE_SPEED)])
			game->player.y -= game->player.dir_y * MOVE_SPEED;
	}
	else if (key_code == KEY_A) // Strafe left
	{
		leftx = game->player.x - game->player.plane_x * MOVE_SPEED;
		lefty = game->player.y - game->player.plane_y * MOVE_SPEED;
		if (!game->map[(int)leftx][(int)game->player.y])
			game->player.x = leftx;
		if (!game->map[(int)game->player.x][(int)lefty])
			game->player.y = lefty;
	}
	else if (key_code == KEY_D) // Strafe right
	{
		rightx = game->player.x + game->player.plane_x * MOVE_SPEED;
		righty = game->player.y + game->player.plane_y * MOVE_SPEED;
		if (!game->map[(int)rightx][(int)game->player.y])
			game->player.x = rightx;
		if (!game->map[(int)game->player.x][(int)righty])
			game->player.y = righty;
	}
	if (key_code == KEY_LEFT) // Rotate to the left
	{
		// Rotating both the direction vector and the camera plane
		olddirx = game->player.dir_x;
		game->player.dir_x = game->player.dir_x * cos(ROT_SPEED) - game->player.dir_y * sin(ROT_SPEED);
		game->player.dir_y = olddirx * sin(ROT_SPEED) + game->player.dir_y * cos(ROT_SPEED);
		oldplanex = game->player.plane_x;
		game->player.plane_x = game->player.plane_x * cos(ROT_SPEED) - game->player.plane_y * sin(ROT_SPEED);
		game->player.plane_y = oldplanex * sin(ROT_SPEED) + game->player.plane_y * cos(ROT_SPEED);
	}
	else if (key_code == KEY_RIGHT) // Rotate to the right
	{
		// Rotating both the direction vector and the camera plane
		olddirx = game->player.dir_x;
		game->player.dir_x = game->player.dir_x * cos(-ROT_SPEED) - game->player.dir_y * sin(-ROT_SPEED);
		game->player.dir_y = olddirx * sin(-ROT_SPEED) + game->player.dir_y * cos(-ROT_SPEED);
		oldplanex = game->player.plane_x;
		game->player.plane_x = game->player.plane_x * cos(-ROT_SPEED) - game->player.plane_y * sin(-ROT_SPEED);
		game->player.plane_y = oldplanex * sin(-ROT_SPEED) + game->player.plane_y * cos(-ROT_SPEED);
	}
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
			game->img.data[y * WIDTH + x++] =
				rgb_to_bitmask(game->ceiling_color);
		y++;
	}
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
			game->img.data[y * WIDTH + x++] =
				rgb_to_bitmask(game->floor_color);
		y++;
	}
}

void	raycasting(t_game *game)
{
	int		x;
	int		y;
	int		hit; // Was there a wall hit?
	int		side; // Was a NS or a EW wall hit?
	int		mapx;
	int		mapy;
	int		stepx;
	int		stepy;
	int		lineheight;
	int		drawstart;
	int		drawend;
	int		texnum;
	int		texwidth;
	int		texheight;
	int		texx;
	int		texy;
	int		color;
	double	camerax;
	double	raydirx;
	double	raydiry;
	double	sidedistx;
	double	sidedisty;
	double	deltadistx;
	double	deltadisty;
	double	perpwalldist;
	double	wallx;
	double	step;
	double	texpos;

	clear_image(game);
	draw_floor_and_ceiling(game);
	x = 0;
	while (x < WIDTH)
	{
		camerax = 2 * x / (double)WIDTH - 1;
		raydirx = game->player.dir_x + game->player.plane_x * camerax;
		raydiry = game->player.dir_y + game->player.plane_y * camerax;
		mapx = (int)game->player.x;
		mapy = (int)game->player.y;
		deltadistx = fabs(1 / raydirx);
		deltadisty = fabs(1 / raydiry);
		hit = 0;
		// Step direction and initial sideDist calculation
		if (raydirx < 0)
		{
			stepx = -1;
			sidedistx = (game->player.x - mapx) * deltadistx;
		}
		else
		{
			stepx = 1;
			sidedistx = (mapx + 1.0 - game->player.x) * deltadistx;
		}
		if (raydiry < 0)
		{
			stepy = -1;
			sidedisty = (game->player.y - mapy) * deltadisty;
		}
		else
		{
			stepy = 1;
			sidedisty = (mapy + 1.0 - game->player.y) * deltadisty;
		}
		// DDA Algorithm
		while (hit == 0)
		{
			if (sidedistx < sidedisty)
			{
				sidedistx += deltadistx;
				mapx += stepx;
				side = 0;
			}
			else
			{
				sidedisty += deltadisty;
				mapy += stepy;
				side = 1;
			}
			// Check if ray has hit a wall
			if (game->map[mapx][mapy] > 0)
				hit = 1;
		}
		// Calculate distance to the wall
		if (side == 0)
			perpwalldist = (mapx - game->player.x + (1 - stepx) / 2) / raydirx;
		else
			perpwalldist = (mapy - game->player.y + (1 - stepy) / 2) / raydiry;
		// Calculate height of line to draw on screen
		lineheight = (int)(HEIGHT / perpwalldist);
		// Calculate lowest and highest pixel to fill in current stripe
		drawstart = -lineheight / 2 + HEIGHT / 2;
		if (drawstart < 0)
			drawstart = 0;
		drawend = lineheight / 2 + HEIGHT / 2;
		if (drawend >= HEIGHT)
			drawend = HEIGHT - 1;
		// choose texture based on direction
		if (side == 0)
		{
			if (raydirx > 0)
				texnum = 3; //west
			else
				texnum = 2; //east
		}
		else
		{
			if (raydiry > 0)
				texnum = 0;//north
			else
				texnum = 1; //south
		}
		// Calculate texture coordinates
		if (side == 0)
			wallx = game->player.y + perpwalldist * raydiry;
		else
			wallx = game->player.x + perpwalldist * raydirx;
		wallx -= floor(wallx);
		texwidth = game->textures[texnum].width;
		texheight = game->textures[texnum].height;
        texx = (int)(wallx * (double)(texwidth));
		if (side == 0 && raydirx > 0)
			texx = texwidth - texx - 1;
		if (side == 1 && raydiry < 0)
			texx = texwidth - texx - 1;
		// How much to increase the texture coordinate per screen pixel
		step = 1.0 * texheight / lineheight;
		// Starting texture coordinate
		texpos = (drawstart - HEIGHT / 2 + lineheight / 2) * step;
		y = drawstart;
		while (y < drawend)
		{
			// Cast the texture coordinate to integer, and mask with (texheight - 1) in case of overflow
			texy = (int)texpos & (texheight - 1);
			texpos += step;
			color = game->textures[texnum].data[texheight * texy + texx];
            // make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
			if (side == 1)
				color = (color >> 1) & 8355711;
            game->img.data[y * WIDTH + x] = color;
			y++;
		}
		x++;
	}
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
				game->player.x = i + 0.5;
				game->player.y = j + 0.5;
				printf("Player position: %f, %f\n", game->player.x, game->player.y);
				if (game->map[i][j] == 'N')
				{
					game->player.dir_x = -1;
					game->player.dir_y = 0;
					game->player.plane_x = 0;
					game->player.plane_y = 0.66;
					game->map[i][j] = 0;
				}
				else if (game->map[i][j] == 'S')
				{
					game->player.dir_x = 1;
					game->player.dir_y = 0;
					game->player.plane_x = 0;
					game->player.plane_y = -0.66;
					game->map[i][j] = 0;
				}
				else if (game->map[i][j] == 'E')
				{
					game->player.dir_x = 0;
					game->player.dir_y = 1;
					game->player.plane_x = 0.66;
					game->player.plane_y = 0;
					game->map[i][j] = 0;
				}
				else if (game->map[i][j] == 'W')
				{
					game->player.dir_x = 0;
					game->player.dir_y = -1;
					game->player.plane_x = -0.66;
					game->player.plane_y = 0;
					game->map[i][j] = 0;
				}
				return ;
			}
			j++;
		}
		i++;
	}
}

// Function to load a texture from an XPM file
void	load_texture(t_game *game, int tex_index, char *path, void *mlx)
{
	t_texture	*tex;
	int			width;
	int			height;
	int			size_l;
	int			bpp;
	int			endian;

	tex = &game->textures[tex_index];
	game->textures[tex_index].img =
		mlx_xpm_file_to_image(mlx, path, &width, &height);
	game->textures[tex_index].data =
		(int *)mlx_get_data_addr(
			game->textures[tex_index].img, &bpp, &size_l, &endian);
	if (!tex->data)
	{
		ft_putstr_fd("Error\nUnable to load texture\n", 2);
		ft_close(game, EXIT_FAILURE);
	}
	tex->width = width;
	tex->height = height;
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

void	check_arguments(int argc, char **argv, t_game *game)
{
	int		fd;
	char	*extension;

	if (argc != 2)
	{
		ft_putstr_fd("Error\nUsage: ", 2);
		ft_putstr_fd(argv[0], 2);
		ft_putstr_fd(" <map_file.cub>\n", 2);
		ft_close(game, EXIT_FAILURE);
	}
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
		perror("Error\n");
		ft_close(game, errno);
	}
	close(fd);
}

void	parse_color(t_game *game, char *str, char texture)
{
	char	**colors;
	int		i;

	if (str == NULL)
	{
		perror("Error\nInput rgb string is NULL\n");
		ft_close(game, EXIT_FAILURE);
	}
	colors = ft_split(str, ',');
	if (colors == NULL)
	{
		perror("Error\nMemory allocation failed\n");
		ft_close(game, EXIT_FAILURE);
	}
	i = 0;
	while (colors[i] != NULL)
		i++;
	if (i != 3)
	{
		perror("Error\nIncorrect number of color values\n");
		ft_freesplit(colors);
		ft_close(game, EXIT_FAILURE);
	}
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
			ft_freesplit(colors);
			ft_close(game, EXIT_FAILURE);
		}
		i++;
	}
	ft_freesplit(colors);
}

// TODO remove this function once testing is done
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

void	find_player_position(t_game *game, int *playerx, int *playery)
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
				if (i == 0 || i == game->row - 1
					|| j == 0 || j == game->col - 1)
				{
					perror("Error\nPlayer location is OOB\n");
                	ft_close(game, EXIT_FAILURE);
				}
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
		ft_close(game, EXIT_FAILURE);
	}
	visited[x][y] = 1;
	flood_fill(game, x + 1, y, visited);
	flood_fill(game, x - 1, y, visited);
	flood_fill(game, x, y + 1, visited);
	flood_fill(game, x, y - 1, visited);
}

int	**create_visited_array(t_game *game)
{
	int	i;
	int	j;
	int	**visited;

	visited = malloc(game->row * sizeof(int *));
	if (!visited)
	{
		perror("Error\nMemory allocation failed for visited array\n");
		ft_close(game, EXIT_FAILURE);
	}
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
	return (visited);
}

void	free_visited_array(t_game *game, int **visited)
{
	int	i;

	i = 0;
	while (i < game->row)
		free(visited[i++]);
	free(visited);
}

void	is_map_fully_enclosed(t_game *game)
{
	int	**visited;
	int playerx;
	int	playery;

	visited = create_visited_array(game);
	playerx = 1;
	playery = 1;
	find_player_position(game, &playerx, &playery);
	flood_fill(game, playerx, playery, visited);
	free_visited_array(game, visited);
}

void	parse_cub_file(t_game *game)
{
	char	temp_map[1000][1000];
	char	*line;
	char	**split_line;
	int		i;
	int		j;
	int		player_count;
	int		mapcheck;
	t_map	*current;

	game->row = 0;
	game->col = 0;
	mapcheck = 0;
	player_count = 0;
	current = game->read_map;
	ft_memset(temp_map, 0, sizeof(temp_map));
	while (current != NULL)
	{
		line = current->content;
		split_line = ft_split(line, ' ');
		if (mapcheck == 1)
		{
			if (ft_strlen(line) == 0)
			{
				perror("Error\nEmpty line in .cub map file\n");
				ft_close(game, EXIT_FAILURE);
			}
			ft_strcpy(temp_map[game->row], line);
			if ((int)ft_strlen(line) > game->col)
				game->col = ft_strlen(line);
			game->row++;
		}
		else if ((ft_strncmp(line, " ", 1) == 0
			|| ft_strncmp(line, "1", 1) == 0
			|| ft_strncmp(line, "1", 1) == 0) && mapcheck == 0)
		{
			ft_strcpy(temp_map[game->row], line);
			game->col = ft_strlen(line);
			game->row++;
			mapcheck = 1;
		}
		else if (split_line[0] && split_line[1] && mapcheck == 0)
		{
			if (ft_strcmp(split_line[0], "NO") == 0)
				game->textures[0].path = ft_strdup(split_line[1]);
			else if (ft_strcmp(split_line[0], "SO") == 0)
				game->textures[1].path = ft_strdup(split_line[1]);
			else if (ft_strcmp(split_line[0], "EA") == 0)
				game->textures[2].path = ft_strdup(split_line[1]);
			else if (ft_strcmp(split_line[0], "WE") == 0)
				game->textures[3].path = ft_strdup(split_line[1]);
			else if (ft_strcmp(split_line[0], "F") == 0)
				parse_color(game, split_line[1], 'F');
			else if (ft_strcmp(split_line[0], "C") == 0)
				parse_color(game, split_line[1], 'C');
			else
			{
				ft_putstr_fd("Error\nInvalid param line in .cub file\n", 2);
				ft_putstr_fd(line, 2);
				ft_putstr_fd("\n", 2);
				ft_close(game, EXIT_FAILURE);
			}
		}
		else if (ft_strlen(line) > 0 && mapcheck == 0)
		{
			ft_putstr_fd("Error\nInvalid line in .cub file\n", 2);
			ft_putstr_fd(line, 2);
			ft_putstr_fd("\n", 2);
			ft_close(game, EXIT_FAILURE);
		}
		ft_freesplit(split_line);
		current = current->next;
	}
    game->map = malloc(game->row * sizeof(int *));
	if (game->map == NULL)
	{
		ft_putstr_fd("Error\nMemory allocation failed\n", 2);
		ft_close(game, EXIT_FAILURE);
	}
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
	i = 0;
	while (i < game->row)
	{
		j = 0;
		while (j < game->col)
		{
			if (temp_map[i][j] == 'N' || temp_map[i][j] == 'S'
				|| temp_map[i][j] == 'E' || temp_map[i][j] == 'W')
				player_count++;
			if (player_count > 1)
			{
				perror("Error\nMultiple players in map\n");
				ft_close(game, EXIT_FAILURE);
			}
			if (temp_map[i][j] == '0')
				game->map[i][j] = 0;
			else if (temp_map[i][j] == '1')
				game->map[i][j] = 1;
			else if (temp_map[i][j] == 'N')
				game->map[i][j] = 'N';
			else if (temp_map[i][j] == 'S')
				game->map[i][j] = 'S';
			else if (temp_map[i][j] == 'E')
				game->map[i][j] = 'E';
			else if (temp_map[i][j] == 'W')
				game->map[i][j] = 'W';
			else if (temp_map[i][j] == ' '
				|| ((int)ft_strlen(temp_map[i]) < game->col
				&& j >= (int)ft_strlen(temp_map[i])))
				game->map[i][j] = ' ';
			else
			{
				ft_putstr_fd("Error\nInvalid character in map\n", 2);
				ft_putstr_fd(temp_map[i], 2);
				ft_putstr_fd("\n", 2);
				ft_close(game, EXIT_FAILURE);
			}
			j++;
		}
		i++;
	}
	is_map_fully_enclosed(game);
	// TODO remove this once debugging is over.
	print_game_map(game);
	printf("Ceiling color: %d, %d, %d\n", game->ceiling_color[0],
		game->ceiling_color[1], game->ceiling_color[2]);
}

// game with single pointer will give error
void	init_game(t_game **game, char **argv)
{
	int		i;
	int		fd;
	char	*buff;

	*game = (t_game *)malloc(sizeof(t_game));
	if (*game == NULL)
	{
		perror("Error\nMemory allocation failed\n");
		ft_close(*game, EXIT_FAILURE);
	}
	(*game)->mlx = NULL;
	(*game)->win = NULL;
	(*game)->read_map = NULL;
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
	i = 0;
	while (i < 4)
	{
		(*game)->textures[i].data = NULL;
		(*game)->textures[i].img = NULL;
		(*game)->textures[i].width = 0;
		(*game)->textures[i].height = 0;
		i++;
	}
	window_init(*game);
	fd = open(argv[1], O_RDONLY);
	buff = get_next_line(fd);
	while(buff)
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
	printf("Player initial position, x: %d, y: %d\n", (int)(*game)->player.x,
		(int)(*game)->player.y);
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
