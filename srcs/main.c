#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "libft.h"
#include "mlx.h"
#include "so_long_structures.h"
#include "so_long.h"

void	fill_texture_img(t_img *pack, t_img *img, int x_start, int y_start)
{
	int		x;
	int		y;
	int		i;
	char	*addr;

	i = 0;
	y = y_start;
	while (y < y_start + 32)
	{
		x = x_start;
		while (x < x_start + 32)
		{
			addr = pack->addr + (y * pack->width + x * (pack->bpp / 8));
			if ((int)(*addr))
				ft_memcpy(img->addr + i, addr, 4);
			i += 4;
			x++;
		}
		y++;
	}
}

void	put_img_to_img(t_img *dst, t_img *src, int x_start, int y_start)
{
	t_coord	dst_pos;
	t_coord	src_pos;
	char	*addr_dst;
	char	*addr_src;
	int		color;

	dst_pos.y = y_start;
	src_pos.y = 0;
	while (dst_pos.y < y_start + 32)
	{
		dst_pos.x = x_start;
		src_pos.x = 0;
		while (dst_pos.x < x_start + 32)
		{
			addr_dst = dst->addr + (dst_pos.y * dst->width + dst_pos.x * 4);
			addr_src = src->addr + (src_pos.y * src->width + src_pos.x * 4);
			color = (int)(*addr_src);
			if (color && color != (int)(*addr_dst))
				ft_memcpy(addr_dst, addr_src, 4);
			dst_pos.x++;
			src_pos.x++;
		}
		dst_pos.y++;
		src_pos.y++;
	}
}

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

void	setup_textures_addr(t_game *game, t_img *pack)
{
	t_img	*img;

	img = &game->wall;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->grass;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->exit;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->collect;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->water;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	fill_texture_img(pack, &game->wall, 64, 64);
	fill_texture_img(pack, &game->water, 32, 64);
	fill_texture_img(pack, &game->grass, 0, 64);
	fill_texture_img(pack, &game->exit, 64, 0);
	fill_texture_img(pack, &game->collect, 128, 0);
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

int	create_player_textures(t_game *game, t_character *player)
{
	player->front.img = mlx_new_image(game->mlx, 32, 32);
	player->back.img = mlx_new_image(game->mlx, 32, 32);
	player->right.img = mlx_new_image(game->mlx, 32, 32);
	player->left.img = mlx_new_image(game->mlx, 32, 32);
	if (!player->front.img || !player->back.img || !player->right.img
		|| !player->left.img)
		return (-1);
	return (0);
}

void	fill_player_texture(t_img *pack, t_character *player)
{
	fill_texture_img(pack, &player->front, 32, 0);
	fill_texture_img(pack, &player->back, 32, 96);
	fill_texture_img(pack, &player->right, 32, 64);
	fill_texture_img(pack, &player->left, 32, 32);
}

void	setup_player_addr(t_game *game)
{
	t_img	*img;

	img = &game->player.front;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->player.back;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->player.right;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->player.left;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
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
	if (init_screen(game) || init_textures(game, "pack_1.xpm")
		|| init_player(game, "cat1.xpm"))
		return (-1);
	return (0);
}

void	free_player_datas(t_game *game, t_character *player)
{
	if (player->front.img)
		mlx_destroy_image(game->mlx, player->front.img);
	if (player->back.img)
		mlx_destroy_image(game->mlx, player->back.img);
	if (player->right.img)
		mlx_destroy_image(game->mlx, player->right.img);
	if (player->left.img)
		mlx_destroy_image(game->mlx, player->left.img);
}

void	free_all_datas(t_game *game)
{
	if (game->water.img)
		mlx_destroy_image(game->mlx, game->water.img);
	if (game->wall.img)
		mlx_destroy_image(game->mlx, game->wall.img);
	if (game->grass.img)
		mlx_destroy_image(game->mlx, game->grass.img);
	if (game->exit.img)
		mlx_destroy_image(game->mlx, game->exit.img);
	if (game->collect.img)
		mlx_destroy_image(game->mlx, game->collect.img);
	if (game->screen.img.img)
		mlx_destroy_image(game->mlx, game->screen.img.img);
	ft_free_tab((void **)(game->screen.map));
	ft_free_tab((void **)(game->map));
	free_player_datas(game, &game->player);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
	}
}

void	handle_large_map(t_game *game, int *screen_x, int *map_x, int *len_x)
{
	if (game->player.pos.x < game->screen.size.x / 2 + 1)
	{
		*screen_x = 1;
		*map_x = 0;
		*len_x = game->screen.size.x - 1;
		*len_x += (game->player.pos.x == game->screen.size.x - 3);
	}
	else if (game->width - game->player.pos.x < game->screen.size.x / 2)
	{
		*screen_x = 0;
		*map_x = game->width - game->screen.size.x + 1;
		*len_x = game->width - *map_x;
	}
	else
	{
		*screen_x = 0;
		*map_x = game->player.pos.x - game->screen.size.x / 2 - 1;
		*len_x = game->screen.size.x;
	}
}

void	handle_tall_map(t_game *game, int *screen_y, int *map_y, int *len_y)
{
	if (game->player.pos.y < game->screen.size.y / 2 + 1)
	{
		*screen_y = 1;
		*map_y = 0;
		*len_y = game->screen.size.y - 1;
		*len_y += (game->player.pos.y == game->screen.size.y - 3);
	}
	else if (game->height - game->player.pos.y < game->screen.size.y / 2 - 1)
	{
		*screen_y = 0;
		*map_y = game->height - game->screen.size.y + 1;
		*len_y = game->height - *map_y - 1;
	}
	else
	{
		*screen_y = 0;
		*map_y = game->player.pos.y - game->screen.size.y / 2 - 1;
		*len_y = game->screen.size.y;
	}
}

void	get_map_pos(t_game *game, t_coord *screen, t_coord *map, t_coord *len)
{
	if (game->width < game->screen.size.x - 1)
	{
		screen->x = ((game->screen.size.x - game->width) % 2 != 0);
		screen->x += (game->screen.size.x - game->width) / 2;
		map->x = 0;
		len->x = game->width;
	}
	else
		handle_large_map(game, &screen->x, &map->x, &len->x);
	if (game->height < game->screen.size.y - 1)
	{
		screen->y = ((game->screen.size.y - game->height) % 2 != 0);
		screen->y += (game->screen.size.y - game->height) / 2;
		map->y = 0;
		len->y = game->height - 1;
	}
	else
		handle_tall_map(game, &screen->y, &map->y, &len->y);
}

void	scroll_screen(t_game *game)
{
	int		y;
	t_coord	map_pos;
	t_coord	screen_pos;
	t_coord	len;
	char	**map;

	get_map_pos(game, &screen_pos, &map_pos, &len);
	game->screen.player.x = game->player.pos.x - map_pos.x + screen_pos.x;
	game->screen.player.y = game->player.pos.y - map_pos.y + screen_pos.y;
	y = 0;
	map = game->screen.map;
	while (y < game->screen.size.y)
	{
		if (y < screen_pos.y || y > screen_pos.y + len.y)
			ft_memset(map[y], 'W', game->screen.size.x);
		else
		{
			ft_memset(map[y], 'W', screen_pos.x);
			ft_memcpy(map[y] + screen_pos.x, (game->map)[map_pos.y + y
				- screen_pos.y] + map_pos.x, len.x);
			ft_memset(map[y] + screen_pos.x
				+ len.x, 'W', game->screen.size.x - screen_pos.x - len.x);
		}
		y++;
	}
}

int	render_frame(t_game *game);

void	detect_collect_exit(t_game *game, char *game_map_c, char *screen_map_c)
{
	if (*screen_map_c == 'C')
	{
		ft_putstr_fd("Hey, this stuff seem usefull, let's keep it ", 1);
		game->collect_nb--;
		*game_map_c = '0';
		*screen_map_c = '0';
	}
	else if (*screen_map_c == 'E')
	{
		if (game->collect_nb)
			ft_putstr_fd("Hmmm, i think there are still things to do here ", 1);
		else
		{
			game->moves++;
			ft_putstr_fd("Yay! My job is done here!\n", 1);
			ft_putstr_fd("Congratulations, you completed this map with ", 1);
			ft_putnbr_fd(game->moves, 1);
			ft_putstr_fd(" moves!\n", 1);
			game->quit = 1;
		}
	}
}

void	move_up(t_game *game)
{
	char	**map;
	char	**map_2;
	t_coord	pos;
	t_coord	pos_2;

	map = game->screen.map;
	pos.x = game->screen.player.x;
	pos.y = game->screen.player.y - 1;
	map_2 = game->map;
	pos_2.x = game->player.pos.x;
	pos_2.y = game->player.pos.y - 1;
	detect_collect_exit(game, &map[pos.y][pos.x], &map_2[pos_2.y][pos_2.x]);
	if (map[pos.y][pos.x] == '1')
		ft_putstr_fd("Outch, That's a wall!(doesn't count as a move) ", 1);
	else
	{
		game->moves++;
		game->screen.player.y--;
		game->player.pos.y--;
		scroll_screen(game);
	}
	print_moves(game);
}

void	move_down(t_game *game)
{
	char	**map;
	char	**map_2;
	t_coord	pos;
	t_coord	pos_2;

	map = game->screen.map;
	pos.x = game->screen.player.x;
	pos.y = game->screen.player.y + 1;
	map_2 = game->map;
	pos_2.x = game->player.pos.x;
	pos_2.y = game->player.pos.y + 1;
	detect_collect_exit(game, &map[pos.y][pos.x], &map_2[pos_2.y][pos_2.x]);
	if (map[pos.y][pos.x] == '1')
		ft_putstr_fd("Outch, That's a wall!(doesn't count as a move) ", 1);
	else
	{
		game->moves++;
		game->screen.player.y++;
		game->player.pos.y++;
		scroll_screen(game);
	}
	print_moves(game);
}

void	move_right(t_game *game)
{
	char	**map;
	char	**map_2;
	t_coord	pos;
	t_coord	pos_2;

	map = game->screen.map;
	pos.x = game->screen.player.x + 1;
	pos.y = game->screen.player.y;
	map_2 = game->map;
	pos_2.x = game->player.pos.x + 1;
	pos_2.y = game->player.pos.y;
	detect_collect_exit(game, &map[pos.y][pos.x], &map_2[pos_2.y][pos_2.x]);
	if (map[pos.y][pos.x] == '1')
		ft_putstr_fd("Outch, That's a wall!(doesn't count as a move) ", 1);
	else
	{
		game->moves++;
		game->screen.player.x++;
		game->player.pos.x++;
		scroll_screen(game);
	}
	print_moves(game);
}

void	move_left(t_game *game)
{
	char	**map;
	char	**map_2;
	t_coord	pos;
	t_coord	pos_2;

	map = game->screen.map;
	pos.x = game->screen.player.x - 1;
	pos.y = game->screen.player.y;
	map_2 = game->map;
	pos_2.x = game->player.pos.x - 1;
	pos_2.y = game->player.pos.y;
	detect_collect_exit(game, &map[pos.y][pos.x], &map_2[pos_2.y][pos_2.x]);
	if (map[pos.y][pos.x] == '1')
		ft_putstr_fd("Outch, That's a wall!(doesn't count as a move) ", 1);
	else
	{
		game->moves++;
		game->screen.player.x--;
		game->player.pos.x--;
		scroll_screen(game);
	}
	print_moves(game);
}

int	handle_keypress(int key, t_game *game)
{
	if (key == 65307)
		mlx_loop_end(game->mlx);
	else if (key == 122 && !game->quit)
	{
		game->player.dir = 2;
		move_up(game);
	}
	else if (key == 115 && !game->quit)
	{
		game->player.dir = 0;
		move_down(game);
	}
	else if (key == 113 && !game->quit)
	{
		game->player.dir = 3;
		move_left(game);
	}
	else if (key == 100 && !game->quit)
	{
		game->player.dir = 1;
		move_right(game);
	}
	return (0);
}

void	fill_screen_buffer(t_game *game)
{
	char	c;
	int		x;
	int		y;

	y = 0;
	while (y < game->res.y)
	{
		x = 0;
		while (x < game->res.x)
		{
			c = (game->screen.map)[y / 32][x / 32];
			if (c == 'W')
				put_img_to_img(&game->screen.img, &game->water, x, y);
			else if (c == '1')
				put_img_to_img(&game->screen.img, &game->wall, x, y);
			else
				put_img_to_img(&game->screen.img, &game->grass, x, y);
			if (c == 'C')
				put_img_to_img(&game->screen.img, &game->collect, x, y);
			else if (c == 'E')
				put_img_to_img(&game->screen.img, &game->exit, x, y);
			x += 32;
		}
		y += 32;
	}
}

int	render_frame(t_game *game)
{
	int	x;
	int	y;

	fill_screen_buffer(game);
	x = game->screen.player.x * 32;
	y = game->screen.player.y * 32;
	if (game->player.dir == 0)
		put_img_to_img(&game->screen.img, &game->player.front, x, y);
	else if (game->player.dir == 1)
		put_img_to_img(&game->screen.img, &game->player.right, x, y);
	else if (game->player.dir == 2)
		put_img_to_img(&game->screen.img, &game->player.back, x, y);
	else
		put_img_to_img(&game->screen.img, &game->player.left, x, y);
	mlx_put_image_to_window(game->mlx, game->win, game->screen.img.img, 0, 0);
	if (game->quit && game->quit_time == 20)
		mlx_loop_end(game->mlx);
	else if (game->quit)
		game->quit_time++;
	return (0);
}

int	main(int ac, char **av)
{
	t_game	game;

	ft_memset(&game, 0, sizeof(game));
	if (parser(ac, av, &game) || init_game(&game))
	{
		free_all_datas(&game);
		return (0);
	}
	scroll_screen(&game);
	mlx_loop_hook(game.mlx, &render_frame, &game);
	mlx_hook(game.win, 2, 1L << 0, &handle_keypress, &game);
	mlx_hook(game.win, 33, 1L << 17, &mlx_loop_end, game.mlx);
	mlx_loop(game.mlx);
	free_all_datas(&game);
	return (0);
}
