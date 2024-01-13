/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegoh <jegoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 21:45:15 by jegoh             #+#    #+#             */
/*   Updated: 2024/01/12 23:06:09 by jegoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "cub3d.h"

void	free_game(t_game *game)
{
	int	i;

	if (game == NULL)
		return ;
	if (game->mlx != NULL)
		free(game->mlx);
	if (game->img.data != NULL)
		free(game->img.data);
	if (game->img.img != NULL)
		free(game->img.img);
	i = 0;
	while (i < 4)
	{
		if (game->textures[i].data != NULL)
			free(game->textures[i].data);
		i++;
	}
	free(game);
}

int	ft_close(t_game *game)
{
	mlx_destroy_image(game->mlx, game->img.img);
	mlx_destroy_window(game->mlx, game->win);
	mlx_destroy_display(game->mlx);
	free_game(game);
	exit(0);
}

int	deal_key(int key_code, t_game *game)
{
	double	leftx;
	double	lefty;

	if (key_code == KEY_ESC)
		ft_close(game);
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
		double rightX = game->player.x + game->player.plane_x * MOVE_SPEED;
		double rightY = game->player.y + game->player.plane_y * MOVE_SPEED;
		if (!game->map[(int)rightX][(int)game->player.y])
			game->player.x = rightX;
		if (!game->map[(int)game->player.x][(int)rightY])
			game->player.y = rightY;
	}
	if (key_code == KEY_RIGHT) // Rotate to the left
	{
		// Rotating both the direction vector and the camera plane
		double oldDirX = game->player.dir_x;
		game->player.dir_x = game->player.dir_x * cos(ROT_SPEED) - game->player.dir_y * sin(ROT_SPEED);
		game->player.dir_y = oldDirX * sin(ROT_SPEED) + game->player.dir_y * cos(ROT_SPEED);

        printf("dir_x: %f, dir_y: %f\n", game->player.dir_x, game->player.dir_y);
		
        double oldPlaneX = game->player.plane_x;
		game->player.plane_x = game->player.plane_x * cos(ROT_SPEED) - game->player.plane_y * sin(ROT_SPEED);
		game->player.plane_y = oldPlaneX * sin(ROT_SPEED) + game->player.plane_y * cos(ROT_SPEED);
        
        printf("plane_x: %f, plane_y: %f\n", game->player.plane_x, game->player.plane_y);
	}
	else if (key_code == KEY_LEFT) // Rotate to the right
	{
		// Rotating both the direction vector and the camera plane
		double oldDirX = game->player.dir_x;
		game->player.dir_x = game->player.dir_x * cos(-ROT_SPEED) - game->player.dir_y * sin(-ROT_SPEED);
		game->player.dir_y = oldDirX * sin(-ROT_SPEED) + game->player.dir_y * cos(-ROT_SPEED);
        
        // printf("dir AFTER : dir_x: %f, dir_y: %f\n", game->player.dir_x, game->player.dir_y);
        double oldPlaneX = game->player.plane_x;
		game->player.plane_x = game->player.plane_x * cos(-ROT_SPEED) - game->player.plane_y * sin(-ROT_SPEED);
		game->player.plane_y = oldPlaneX * sin(-ROT_SPEED) + game->player.plane_y * cos(-ROT_SPEED);

        // printf("plane AFTER : plane_x: %f, plane_y: %f\n", game->player.plane_x, game->player.plane_y);
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
        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (game->player.x - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - game->player.x) * deltaDistX;
        }
        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (game->player.y - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - game->player.y) * deltaDistY;
        }

        // DDA Algorithm
        while (hit == 0) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            // Check if ray has hit a wall
            if (game->map[mapX][mapY] > 0) hit = 1;
        }

        // Calculate distance to the wall
        if (side == 0) perpWallDist = (mapX - game->player.x + (1 - stepX) / 2) / rayDirX;
        else perpWallDist = (mapY - game->player.y + (1 - stepY) / 2) / rayDirY;

        // Calculate height of line to draw on screen
        int lineHeight = (int)(HEIGHT / perpWallDist);

        // Calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + HEIGHT / 2;
        if (drawEnd >= HEIGHT) drawEnd = HEIGHT - 1;

        // Choose wall texture
        int texNum = game->map[mapX][mapY] - 1; // assuming 0 is an empty space and textures are 1, 2, 3, 4...
        if (texNum > 3) texNum = 3; // Prevents segfaults (for now...), temporary solution
        // Calculate texture coordinates
        double wallX;
        if (side == 0) wallX = game->player.y + perpWallDist * rayDirY;
        else wallX = game->player.x + perpWallDist * rayDirX;
        wallX -= floor(wallX);

		int texWidth = game->textures[texNum].width;
		int texHeight = game->textures[texNum].height;

        int texX = (int)(wallX * (double)(texWidth));
        if (side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
        if (side == 1 && rayDirY < 0) texX = texWidth - texX - 1;

        // How much to increase the texture coordinate per screen pixel
        double step = 1.0 * texHeight / lineHeight;
        // Starting texture coordinate
        double texPos = (drawStart - HEIGHT / 2 + lineHeight / 2) * step;
        for (int y = drawStart; y < drawEnd; y++)
        {
            // Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
            int texY = (int)texPos & (texHeight - 1);
            texPos += step;
            int color = game->textures[texNum].data[texHeight * texY + texX];
            // make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
            // if (side == 1) color = (color >> 1) & 8355711;
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
				game->player.x = j;
				game->player.y = i;
				game->player.dir_x = 0.5;
				game->player.dir_y = 1;
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
void	load_texture(t_game *game, int tex_index, char *path, void *mlx)
{
	t_texture	*tex = &game->textures[tex_index];
	int	width;
	int	height;

    // Using mlx_xpm_file_to_image to load the texture
    tex->data = (int *)mlx_xpm_file_to_image(mlx, path, &width, &height);
	printf("%i\n", *(tex->data));
    if (!tex->data)
    {
        ft_putstr_fd("Error loading texture\n", 2);
        exit(EXIT_FAILURE);
    }
    tex->width = width;
    tex->height = height;
}

void	game_init(t_game *game)
{
    int map[ROWS][COLS] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1},
	{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1},
	{1, 1, 1, 1, 0, 0, 0, 'N', 0, 0, 1, 0, 1, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	};

	ft_memcpy(game->map, map, sizeof(int) * ROWS * COLS);
	player_init(game);
	load_texture(game, 0, "./img/so.xpm", game->mlx); //north
    load_texture(game, 1, "./img/so.xpm", game->mlx); //south
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
	// if (game->img.img != NULL) {
    //     mlx_destroy_image(game->mlx, game->img.img);
    // }
    raycasting(game);
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

void	init_game(t_game **game, char **argv)
{
	int		i;
	int		fd;
	char	*buff;

	*game = (t_game *)malloc(sizeof(t_game));
	if (*game == NULL)
		exit(1);
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
	}
	window_init(*game);
	game_init(*game);
	img_init(*game);
}

int	main(int argc, char **argv)
{
	t_game	*game;

	check_arguments(argc, argv);
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
//     void *img_ptr;
// 	int width;
// 	int height;

//     mlx_ptr = mlx_init();
//     win_ptr = mlx_new_window(mlx_ptr, 800, 600, "My Window");
//     img_ptr = mlx_xpm_file_to_image(mlx_ptr, "./img/we.xpm", &width, &height);

//     mlx_put_image_to_window(mlx_ptr, win_ptr, img_ptr, 100, 100);

//     mlx_loop(mlx_ptr);

//     return (0);
// }
