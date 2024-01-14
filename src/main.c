/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegoh <jegoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 21:45:15 by jegoh             #+#    #+#             */
/*   Updated: 2024/01/14 12:31:48 by jegoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "cub3d.h"

void	free_game(t_game *game)
{
	int	i;

	if (game == NULL)
		return ;
	if (game->mlx)
		free(game->mlx);
	i = 0;
	while (i < 4)
	{
		if (game->textures[i].data != NULL)
			free(game->textures[i].data);
		i++;
	}
	free(game);
}

int	ft_close(t_game *game, int exit_code)
{
	if (game->mlx)
	{
		if (game->img.img)
			mlx_destroy_image(game->mlx, game->img.img);
		mlx_destroy_window(game->mlx, game->win);
		mlx_destroy_display(game->mlx);
	}
	free_game(game);
	exit(exit_code);
}

// TODO line is too long, need to reduce length.
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

void	raycasting(t_game *game)
{
	clear_image(game);
	for (int x = 0; x < WIDTH; x++)
	{
		double cameraX = 2 * x / (double)WIDTH - 1;
		double rayDirX = game->player.dir_x + game->player.plane_x * cameraX;
		double rayDirY = game->player.dir_y + game->player.plane_y * cameraX;
		int mapX = (int)game->player.x;
		int mapY = (int)game->player.y;
		double sideDistX, sideDistY;
		double deltaDistX = fabs(1 / rayDirX);
		double deltaDistY = fabs(1 / rayDirY);
		double perpWallDist;
		int stepX, stepY;
		int hit = 0; // Was there a wall hit?
		int side; // Was a NS or a EW wall hit?

        // Step direction and initial sideDist calculation
		if (rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (game->player.x - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.0 - game->player.x) * deltaDistX;
		}
		if (rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (game->player.y - mapY) * deltaDistY;
		}
		else
		{
            stepY = 1;
            sideDistY = (mapY + 1.0 - game->player.y) * deltaDistY;
        }
        // DDA Algorithm
        while (hit == 0)
		{
            if (sideDistX < sideDistY)
			{
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
			else
			{
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            // Check if ray has hit a wall
			if (game->map[mapX][mapY] > 0)
				hit = 1;
        }
        // Calculate distance to the wall
		if (side == 0)
			perpWallDist = (mapX - game->player.x + (1 - stepX) / 2) / rayDirX;
		else
			perpWallDist = (mapY - game->player.y + (1 - stepY) / 2) / rayDirY;
        // Calculate height of line to draw on screen
        int lineHeight = (int)(HEIGHT / perpWallDist);
        // Calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + HEIGHT / 2;
		if (drawStart < 0)
			drawStart = 0;
        int drawEnd = lineHeight / 2 + HEIGHT / 2;
		if (drawEnd >= HEIGHT)
			drawEnd = HEIGHT - 1;
        // Choose wall texture
        int texNum = game->map[mapX][mapY] - 1; // assuming 0 is an empty space and textures are 1, 2, 3, 4...
		if (texNum > 3)
			texNum = 1; // Prevents segfaults (for now...), temporary solution
        // Calculate texture coordinates
        double wallX;
		if (side == 0)
			wallX = game->player.y + perpWallDist * rayDirY;
		else
			wallX = game->player.x + perpWallDist * rayDirX;
        wallX -= floor(wallX);
		int texWidth = game->textures[texNum].width;
		int texHeight = game->textures[texNum].height;
        int texX = (int)(wallX * (double)(texWidth));
		if (side == 0 && rayDirX > 0)
			texX = texWidth - texX - 1;
		if (side == 1 && rayDirY < 0)
			texX = texWidth - texX - 1;
        // How much to increase the texture coordinate per screen pixel
        double step = 1.0 * texHeight / lineHeight;
        // Starting texture coordinate
        double texPos = (drawStart - HEIGHT / 2 + lineHeight / 2) * step;
        for (int y = drawStart; y < drawEnd; y++)
        {
            // Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
            int texY = (int)texPos & (texHeight - 1);
            texPos += step;
			// (void)texY;
            int color = game->textures[1].data[texHeight * texY + texX];
			// int color = game->textures[0].data[0];
            // make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
			if (side == 1)
				color = (color >> 1) & 8355711;
            game->img.data[y * WIDTH + x] = color;
        }
    }
}

void	player_init(t_game *game)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < ROWS)
	{
		j = 0;
		while (j < COLS)
		{
			if (game->map[i][j] == 'N')
			{
                game->map[i][j] = 0;
				game->player.x = j;
				game->player.y = i;
				game->player.dir_x = -1;
				game->player.dir_y = 0;
				game->player.plane_x = 0;
				game->player.plane_y = 0.66;
				return ;
			}
			j++;
		}
		i++;
	}
}

// Function to load a texture from an XPM file
// TODO remove printf statement once debug finished
void	load_texture(t_game *game, int tex_index, char *path, void *mlx)
{
	t_texture	*tex;
	int			width;
	int			height;
	void	*img;
	// int		*data;
	int		size_l;
	int		bpp;
	int		endian;

	tex = &game->textures[tex_index];
	img = mlx_xpm_file_to_image(mlx, path, &width, &height);
	game->textures[tex_index].data = (int*)mlx_get_data_addr(img, &bpp, &size_l, &endian);
	printf("%i\n", tex->data[0]);
	if (!tex->data)
	{
		ft_putstr_fd("Error loading texture\n", 2);
		ft_close(game, EXIT_FAILURE);
	}
	tex->width = width;
	tex->height = height;
}

// TODO remove hardcoded values once transitioned to reading cub file
void	game_init(t_game *game)
{
	int map[ROWS][COLS] = {
	{' ', 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 'N', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	};

	ft_memcpy(game->map, map, sizeof(int) * ROWS * COLS);
	player_init(game);
	load_texture(game, 0, "./img/so.xpm", game->mlx); //north
	load_texture(game, 1, "./img/we.xpm", game->mlx); //south
	load_texture(game, 2, "./img/bluestone.xpm", game->mlx); //east
	load_texture(game, 3, "./img/bluestone.xpm", game->mlx); //west
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
    raycasting(game);
    mlx_put_image_to_window(game->mlx, game->win, game->img.img, 0, 0);
    return (0);
}

void	check_arguments(int argc, char **argv, t_game *game)
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
		ft_close(game, EXIT_FAILURE);
	}
	extension = ft_strrchr(argv[1], '.');
	if (extension == NULL || ft_strcmp(extension, ".cub") != 0)
	{
		ft_putstr_fd("Error: Map file must have a .cub extension\n", 2);
		ft_close(game, EXIT_FAILURE);
	}
}

int	*parse_color(t_game *game, char *str)
{
	char	**colors;
	int		*rgb;
	int		i;

	if (str == NULL)
	{
		ft_putstr_fd("Error: Input rgb string is NULL\n", 2);
		ft_close(game, EXIT_FAILURE);
	}
	colors = ft_split(str, ',');
	if (colors == NULL)
	{
		ft_putstr_fd("Error: Memory allocation failed\n", 2);
		ft_close(game, EXIT_FAILURE);
	}
	i = 0;
	while (colors[i] != NULL)
		i++;
	if (i != 3)
	{
		ft_putstr_fd("Error: Incorrect number of color values\n", 2);
		ft_freesplit(colors);
		ft_close(game, EXIT_FAILURE);
	}
	rgb = malloc(3 * sizeof(int));
	if (rgb == NULL)
	{
		ft_putstr_fd("Error: Memory allocation failed\n", 2);
		ft_freesplit(colors);
		ft_close(game, EXIT_FAILURE);
	}
	i = 0;
	while (i < 3)
	{
		rgb[i] = atoi(colors[i]);
		i++;
		if (rgb[i] < 0 || rgb[i] > 255)
		{
			ft_putstr_fd("Error: Color values must be between 0 and 255\n", 2);
			free(rgb);
			ft_freesplit(colors);
			ft_close(game, EXIT_FAILURE);
		}
	}
	ft_freesplit(colors);
	return (rgb);
}

void	parse_cub_file(t_game *game)
{
	char	temp_map[ROWS][COLS];
	char	*line;
	char	**split_line;
	int		row;
	int		i;
	int		j;
	int		mapcheck;
	t_map	*current;

	row = 0;
	mapcheck = 0;
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
				ft_putstr_fd("Error: Invalid line in .cub file\n", 2);
                ft_putstr_fd(line, 2);
                ft_putstr_fd("\n", 2);
                ft_close(game, EXIT_FAILURE);
			}
			ft_memcpy(temp_map[row], line, COLS);
			row++;
		}
		else if (split_line[0] != NULL && split_line[1] != NULL && mapcheck == 0)
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
				game->floor_color = parse_color(game, split_line[1]);
			else if (ft_strcmp(split_line[0], "C") == 0)
				game->ceiling_color = parse_color(game, split_line[1]);
			else
			{
				ft_putstr_fd("Error: Invalid line in .cub file\n", 2);
				ft_putstr_fd(line, 2);
				ft_putstr_fd("\n", 2);
				ft_close(game, EXIT_FAILURE);
			}
		}
		else if ((ft_strncmp(line, " ", 1) == 0 || ft_strncmp(line, "1", 1) == 0
			|| ft_strncmp(line, "1", 1) == 0) && mapcheck == 0)
		{
            ft_memcpy(temp_map[row], line, COLS);
            row++;
			mapcheck = 1;
		}
		else if (ft_strlen(line) > 0 && mapcheck == 0)
		{
			ft_putstr_fd("Error: Invalid line? in .cub file\n", 2);
			ft_putstr_fd(line, 2);
			ft_putstr_fd("\n", 2);
			ft_close(game, EXIT_FAILURE);
		}
		ft_freesplit(split_line);
		current = current->next;
	}
	i = 0;
	while (i < ROWS)
	{
		j = 0;
		while (j < COLS)
		{
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
			else if (temp_map[i][j] == ' ' || (ft_strlen(temp_map[i]) < COLS
				&& j >= (int)ft_strlen(temp_map[i])))
				game->map[i][j] = ' ';
			else
			{
				printf("Error: Invalid character in map\n");
				ft_close(game, EXIT_FAILURE);
			}
			j++;
		}
		i++;
	}
}

// TODO once parser is done properly, remove if loop for argv
void	init_game(t_game **game, char **argv)
{
	int		i;
	int		fd;
	char	*buff;

	*game = (t_game *)malloc(sizeof(t_game));
	if (*game == NULL)
	{
		ft_putstr_fd("Error: Memory allocation failed\n", 2);
		ft_close(*game, EXIT_FAILURE);
	}
	(*game)->mlx = NULL;
	(*game)->win = NULL;
	(*game)->img.img = NULL;
	(*game)->img.data = NULL;
	(*game)->img.size_l = 0;
	(*game)->img.bpp = 0;
	(*game)->img.endian = 0;
	ft_memset((*game)->map, 0, sizeof(int) * ROWS * COLS);
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
		(*game)->textures[i].width = 0;
		(*game)->textures[i].height = 0;
		i++;
	}
	window_init(*game);
	if (argv[1])
	{
		fd = open(argv[1], O_RDONLY);
		buff = get_next_line(fd);
		while(buff)
		{
			if (ft_strlen(buff) > 0 && buff[ft_strlen(buff) - 1] == '\n')
				buff[ft_strlen(buff) - 1] = '\0';
			ft_lstadd_back(&((*game)->read_map), ft_lstnew(buff));
			buff = get_next_line(fd);
		}
		close(fd);
		parse_cub_file(*game);
		player_init(*game);
		load_texture(*game, 0, (*game)->textures[0].path, (*game)->mlx); //north
		load_texture(*game, 1, (*game)->textures[1].path, (*game)->mlx); //south
		load_texture(*game, 2, (*game)->textures[2].path, (*game)->mlx); //east
		load_texture(*game, 3, (*game)->textures[3].path, (*game)->mlx); //west
	}
	else
		game_init(*game);
	img_init(*game);
}

int	main(int argc, char **argv)
{
	t_game	*game;

	game = NULL;
	check_arguments(argc, argv, game);
	init_game(&game, argv);
	mlx_hook(game->win, X_EVENT_KEY_PRESS, 1L << 0, &deal_key, game);
	mlx_hook(game->win, X_EVENT_KEY_EXIT, 1L << 2, &ft_close, game);
	mlx_loop_hook(game->mlx, &main_loop, game);
	mlx_loop(game->mlx);
}

// int main(void)
// {
//     void *mlx_ptr;
//     void *win_ptr;
//     t_texture *img_ptr;
// 	int width;
// 	int height;

//     mlx_ptr = mlx_init();
//     win_ptr = mlx_new_window(mlx_ptr, 800, 600, "My Window");
//     img_ptr = mlx_xpm_file_to_image(mlx_ptr, "./img/we.xpm", &width, &height);

//     mlx_put_image_to_window(mlx_ptr, win_ptr, img_ptr, 100, 100);

//     mlx_loop(mlx_ptr);

//     return (0);
// }