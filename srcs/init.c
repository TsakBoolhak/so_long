#include "so_long_structures.h"
#include "libft.h"
#include "so_long.h"
#include "mlx.h"
#include <stdlib.h>

int	create_screen_map(t_game *game)
{
	char	**map;
	int		y;

	map = malloc(sizeof(char *) * (game->screen.size.y + 1));
	if (!map)
		return (-1);
	map[game->screen.size.y] = NULL;
	y = 0;
	while (y < game->screen.size.y)
	{
		map[y] = malloc(sizeof(char) * (game->screen.size.x + 1));
		if (!map[y])
		{
			ft_free_tab((void **)(map));
			return (-1);
		}
		map[y][game->screen.size.x] = '\0';
		y++;
	}
	game->screen.map = map;
	return (0);
}

int	init_screen(t_game *game)
{
	t_img	img;

	mlx_get_screen_size(game->mlx, &game->res.x, &game->res.y);
	game->res.x = game->res.x / 32 - 1;
	game->res.y = game->res.y / 32 - 1;
	if (game->res.x < 1 || game->res.y < 1)
		return (return_error("The screen is less than 32 * 32 pixels", -1));
	game->screen.size.x = game->res.x;
	game->screen.size.y = game->res.y;
	game->res.x *= 32;
	game->res.y *= 32;
	game->win = mlx_new_window(game->mlx, game->res.x, game->res.y, "so_long");
	img.img = mlx_new_image(game->mlx, game->res.x, game->res.y);
	if (!game->win || !img.img || create_screen_map(game))
	{
		if (img.img)
			mlx_destroy_image(game->mlx, img.img);
		return (return_error("Couldnt allocate enough memory", -1));
	}
	img.addr = mlx_get_data_addr(img.img, &img.bpp, &img.width, &img.end);
	game->screen.img = img;
	return (0);
}

int	init_textures(t_game *game, char *pack_1)
{
	t_img	pack;
	void	*mlx;

	mlx = game->mlx;
	pack.img = mlx_xpm_file_to_image(mlx, pack_1, &pack.width, &pack.height);
	if (!pack.img)
		return (return_error("Couldn't load the texture pack", -1));
	game->water.img = mlx_new_image(mlx, 32, 32);
	game->wall.img = mlx_new_image(mlx, 32, 32);
	game->grass.img = mlx_new_image(mlx, 32, 32);
	game->exit.img = mlx_new_image(mlx, 32, 32);
	game->collect.img = mlx_new_image(mlx, 32, 32);
	if (!game->wall.img || !game->grass.img || !game->exit.img
		|| !game->collect.img || !game->water.img)
	{
		mlx_destroy_image(game->mlx, pack.img);
		return (return_error("Couldn't allocate enough memory", -1));
	}
	pack.addr = mlx_get_data_addr(pack.img, &pack.bpp, &pack.width, &pack.end);
	setup_textures_addr(game, &pack);
	mlx_destroy_image(game->mlx, pack.img);
	return (0);
}

int	init_player(t_game *game, char *pack_2)
{
	t_img		pack;
	void		*mlx;
	t_character	*player;

	mlx = game->mlx;
	player = &game->player;
	pack.img = mlx_xpm_file_to_image(mlx, pack_2, &pack.width, &pack.height);
	if (!pack.img)
		return (return_error("Couldn't load the texture pack", -1));
	if (create_player_textures(game, player))
	{
		mlx_destroy_image(game->mlx, pack.img);
		return (return_error("Couldn't allocate enough memory", -1));
	}
	pack.addr = mlx_get_data_addr(pack.img, &pack.bpp, &pack.width, &pack.end);
	setup_player_addr(game);
	fill_player_texture(&pack, player);
	mlx_destroy_image(game->mlx, pack.img);
	return (0);
}

int	init_game(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		return (return_error("Couldn't allocate enough memory", -1));
	if (init_screen(game) || init_textures(game, "textures/pack.xpm")
		|| init_player(game, "textures/cat.xpm"))
		return (-1);
	return (0);
}
