#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "libft.h"
#include "mlx.h"

typedef struct s_coord
{
	int	x;
	int	y;
}t_coord;

typedef struct s_screen
{
	t_coord	size;
	t_coord	player;
	char	**map;
}t_screen;

typedef struct s_game
{
	int			height;
	int			width;
	char		**map;
	t_screen	screen;
	t_coord		player;
	t_coord		res;
	void		*win;
	void		*mlx;
}t_game;

typedef struct s_parse
{
	int	exit;
	int	collectible;
	int	line_error;
	int	char_error;
}t_parse;

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bpp;
	int		width;
	int		height;
	int		endian;
}t_img;

void	print_usage(void)
{
	ft_putendl_fd("Usage: \"./so_long file.ber\"", 2);
	ft_putendl_fd("where file.ber is a valid map file :", 2);
	ft_putstr_fd("file must have .ber extension, having read access on, ", 2);
	ft_putstr_fd("must not be a folder and ", 2);
	ft_putendl_fd("must contain a map that respect map rules", 2);
}

void	print_map_rules(void)
{
	ft_putendl_fd("\nMap rules:", 2);
	ft_putstr_fd("- Map must be composed of only 5 possible characters", 2);
	ft_putstr_fd(": '0' (zero) for an empty space, '1' (one) for a wall, ", 2);
	ft_putstr_fd("'C' for a collectible, 'E' for map exit and ", 2);
	ft_putendl_fd("'P' for the player's starting position.", 2);
	ft_putendl_fd("- Map must be closed/surrounded by walls", 2);
	ft_putstr_fd("- Map must contain at least one exit and one ", 2);
	ft_putendl_fd("collectible\n- Map must be rectangular", 2);
	ft_putstr_fd("- Map must contain one and only one ", 2);
	ft_putendl_fd("player's starting position\n", 2);
	ft_putendl_fd("Here is an exemple of a simple valid map", 2);
	ft_putendl_fd("1111111\n1C01EP1\n10C11C1\n1C00001\n1111111", 2);
}

void	print_map(char **map, int fd)
{
	int	i;

	i = 0;
	while (map[i])
	{
		ft_putendl_fd(map[i], fd);
		i++;
	}
}

char	**ft_add_str_to_tab(char **tab, char *line)
{
	char	**new_tab;
	int		i;

	i = 0;
	while (tab && tab[i])
		i++;
	new_tab = (char **)malloc(sizeof(char *) * (i + 2));
	if (!new_tab)
		return (NULL);
	new_tab[i + 1] = NULL;
	new_tab[i] = line;
	while (i-- > 0)
		new_tab[i] = tab[i];
	free(tab);
	return (new_tab);
}

int	check_argument(int ac, char **av)
{
	char	*extension;

	if (ac != 2)
	{
		ft_putstr_fd("Error\nWrong number of arguments : ", 2);
		ft_putnbr_fd(ac - 1, 2);
		ft_putstr_fd(" argument", 2);
		write(2, "s", (ac > 1));
		ft_putendl_fd(" entered instead of 1\n", 2);
		print_usage();
		return (-1);
	}
	extension = ft_strrchr(av[1], '.');
	if (!extension || ft_strcmp(extension, ".ber"))
	{
		ft_putendl_fd("Error\nWrong map file's name\n", 2);
		print_usage();
		return (-1);
	}
	return (0);
}

void	print_line_error(char *line, int row, int *no_print, char **map)
{
	if (!(*no_print))
	{
		ft_putstr_fd("Error\nMap is not valid\n", 2);
		ft_putendl_fd("\nHere is the parsed map when the error occured:\n", 2);
		print_map(map, 2);
		ft_putstr_fd("\nError occured in line #", 2);
		ft_putnbr_fd(row, 2);
		ft_putstr_fd(":\n\"", 2);
		ft_putstr_fd(line, 2);
		ft_putendl_fd("\"\n", 2);
		*no_print = -1;
	}
}

void	print_char_error(char c, int column, int *flag)
{
	if (!(*flag))
	{
		ft_putstr_fd("-Wrong character : ", 2);
		if (ft_isprint(c))
		{
			ft_putstr_fd("'", 2);
			write(2, &c, 1);
			ft_putstr_fd("'", 2);
		}
		ft_putstr_fd("\t", 2);
		ft_putstr_fd("(ASCII value: ", 2);
		ft_putnbr_fd((int)c, 2);
		ft_putstr_fd(")\ton col #", 2);
		ft_putnbr_fd(column, 2);
		ft_putstr_fd("\t:", 2);
		*flag = -1;
	}
}

void	handle_valid_char(char c, int i, t_game *game, t_parse *parse)
{
	if (c == 'P')
	{
		game->player.x = i;
		game->player.y = game->width - 1;
	}
	else if (c == 'C')
		(parse->collectible)++;
	else if (c == 'E')
		(parse->exit)++;
}

void	check_char(char *line, int i, t_game *game, t_parse *parse)
{
	if (line[i] != '1' && (!i || game->height == 1 || i == game->width - 1))
	{
		print_line_error(line, game->height, &parse->line_error, game->map);
		print_char_error(line[i], i + 1, &parse->char_error);
		ft_putstr_fd(" -Map is not closed", 2);
	}
	if (!ft_strchr("01ECP", line[i]))
	{
		print_line_error(line, game->height, &parse->line_error, game->map);
		print_char_error(line[i], i + 1, &parse->char_error);
		ft_putstr_fd(" -Not a valid character", 2);
	}
	if (line[i] == 'P' && game->player.x != -1)
	{
		print_line_error(line, game->height, &parse->line_error, game->map);
		print_char_error(line[i], i + 1, &parse->char_error);
		ft_putstr_fd(" -Map contain more than one player spawn", 2);
	}
	handle_valid_char(line[i], i, game, parse);
	if (parse->char_error)
		ft_putendl_fd("", 2);
}

int	check_line(char *line, t_game *game, t_parse *parse)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(line);
	if (len < 3)
	{
		print_line_error(line, game->height, &parse->line_error, game->map);
		ft_putendl_fd("-Line is too short to contain a closed map", 2);
	}
	if (!game->width)
		game->width = len;
	else if (len != game->width)
	{
		print_line_error(line, game->height, &parse->line_error, game->map);
		ft_putendl_fd("-Map is not rectangular", 2);
	}
	while (line[i])
	{
		parse->char_error = 0;
		check_char(line, i, game, parse);
		i++;
	}
	return (parse->line_error);
}

void	print_last_error(t_parse *parse, char **map, int no_spawn)
{
	if (!parse->line_error)
	{
		ft_putendl_fd("Error\nMap is not valid", 2);
		ft_putendl_fd("\nHere is the parsed map when the error occured:\n", 2);
		print_map(map, 2);
		parse->line_error = -1;
	}
	if (!parse->exit)
		ft_putendl_fd("\n-Map has no exit", 2);
	if (!parse->collectible)
	{
		if (parse->exit)
			ft_putendl_fd("", 2);
		ft_putendl_fd("-Map has no collectible", 2);
	}
	if (no_spawn == -1)
	{
		if (parse->exit && parse->collectible)
			ft_putendl_fd("", 2);
		ft_putendl_fd("-Map has no spawn", 2);
	}
}

void	last_parsing_checks(char *line, t_game *game, t_parse *parse)
{
	int	i;

	i = 0;
	while (line[i])
	{
		parse->char_error = 0;
		if (line[i] != '1')
		{
			print_line_error(line, game->height, &parse->line_error, game->map);
			print_char_error(line[i], i + 1, &parse->char_error);
			ft_putendl_fd(" -Map is not closed", 2);
		}
		i++;
	}
	if (!parse->collectible || !parse->exit || game->player.x == -1)
		print_last_error(parse, game->map, game->player.x);
}

int	map_parser(int fd, t_game *game, t_parse *parse)
{
	int		ret;
	char	*line;

	ret = 1;
	while (ret > 0)
	{
		ret = get_next_line(fd, &line, 0);
		if (ret == -1)
			ft_putendl_fd("Error\nError occured while reading file.", 2);
		if (ret == -1)
			return (-1);
		else if (!ret)
			break ;
		game->map = ft_add_str_to_tab(game->map, line);
		(game->height)++;
		if (check_line(line, game, parse))
			return (-1);
	}
	if (!game->height)
	{
		ft_putendl_fd("Error\nFile is empty", 2);
		return (-1);
	}
	last_parsing_checks((game->map)[game->height - 1], game, parse);
	return (parse->line_error);
}

void	init_parsing(t_game *game, t_parse *parse)
{
	ft_memset(parse, 0, sizeof(*parse));
	ft_memset(game, 0, sizeof(*game));
	game->player.x = -1;
	game->player.y = -1;
}

int	parser(int ac, char **av, t_game *game)
{
	int		fd;
	t_parse	parse;

	if (check_argument(ac, av))
		return (-1);
	fd = open(av[1], O_RDONLY);
	if (fd < 2)
	{
		ft_putstr_fd("Error\nCouldn't open file. ", 2);
		ft_putendl_fd("Check if the file exist or if it has read access\n", 2);
		print_usage();
		return (-1);
	}
	init_parsing(game, &parse);
	if (map_parser(fd, game, &parse))
	{
		get_next_line(fd, NULL, 2);
		close(fd);
		ft_free_tab((void **)(game->map));
		print_map_rules();
		return (-1);
	}
	return (0);
}

void	fill_wall_img(t_img *pack, t_img *wall)
{
	int		x;
	int		y;
	int		i;
	char	*addr;

	y = 64;
	i = 0;
	while (y < 97)
	{
		x = 64;
		while (x < 96)
		{
			addr = pack->addr + (y * pack->width + x * (pack->bpp / 8));
			ft_memcpy(wall->addr + i, addr, 4);
			i += 4;
			x++;
		}
		y++;
	}
}

void	fill_water_img(t_img *pack, t_img *water)
{
	int		x;
	int		y;
	int		i;
	char	*addr;

	y = 64;
	i = 0;
	while (y < 97)
	{
		x = 32;
		while (x < 64)
		{
			addr = pack->addr + (y * pack->width + x * (pack->bpp / 8));
			ft_memcpy(water->addr + i, addr, 4);
			i += 4;
			x++;
		}
		y++;
	}
}

void	fill_grass_img(t_img *pack, t_img *grass)
{
	int		x;
	int		y;
	int		i;
	char	*addr;

	y = 64;
	i = 0;
	while (y < 97)
	{
		x = 0;
		while (x < 32)
		{
			addr = pack->addr + (y * pack->width + x * (pack->bpp / 8));
			ft_memcpy(grass->addr + i, addr, 4);
			i += 4;
			x++;
		}
		y++;
	}
}

void	fill_collect_img(t_img *pack, t_img *collect, t_img *grass)
{
	int		x;
	int		y;
	int		i;
	char	*addr;

	y = 0;
	i = 0;
	while (y < 32)
	{
		x = 128;
		while (x < 160)
		{
			addr = pack->addr + (y * pack->width + x * (pack->bpp / 8));
			if ((int)(*addr) == 0)
				addr = grass->addr + i;
			ft_memcpy(collect->addr + i, addr, 4);
			i += 4;
			x++;
		}
		y++;
	}
}

void	fill_exit_img(t_img *pack, t_img *collect, t_img *grass)
{
	int		x;
	int		y;
	int		i;
	char	*addr;

	y = 0;
	i = 0;
	while (y < 32)
	{
		x = 64;
		while (x < 96)
		{
			addr = pack->addr + (y * pack->width + x * (pack->bpp / 8));
			if ((int)(*addr) == 0)
				addr = grass->addr + i;
			ft_memcpy(collect->addr + i, addr, 4);
			i += 4;
			x++;
		}
		y++;
	}
}

void	fill_player_img(t_img *pack, t_img *player, t_img *grass)
{
	int		x;
	int		y;
	int		i;
	char	*addr;

	y = 0;
	i = 0;
	while (y < 32)
	{
		x = 32;
		while (x < 64)
		{
			addr = pack->addr + (y * pack->width + x * (pack->bpp / 8));
			if ((int)(*addr) == 0)
				addr = grass->addr + i;
			ft_memcpy(player->addr + i, addr, 4);
			i += 4;
			x++;
		}
		y++;
	}
}

int	main(int ac, char **av)
{
	t_game	game;
	t_img	txtrs1;
	t_img	txtrs2;
	t_img	grass;
	t_img	wall;
	t_img	water;
	t_img	collect;
	t_img	exit;
	t_img	player;
	int	x;
	int	y;

	if (parser(ac, av, &game))
		return (0);
	game.mlx = mlx_init();
	mlx_get_screen_size(game.mlx, &game.res.x, &game.res.y);
	game.res.x /= 32;
	game.res.x--;
	game.screen.size.x = game.res.x;
	game.res.x *= 32;
	game.res.y /= 32;
	game.res.y--;
	game.screen.size.y = game.res.y;
	game.res.y *= 32;

	game.win = mlx_new_window(game.mlx, game.res.x, game.res.y, "test");
	txtrs1.img = mlx_xpm_file_to_image(game.mlx, "./test1.xpm", &txtrs1.width, &txtrs1.height);
	txtrs1.addr = mlx_get_data_addr(txtrs1.img, &txtrs1.bpp, &txtrs1.width, &txtrs1.endian);
	txtrs2.img = mlx_xpm_file_to_image(game.mlx, "./cat1.xpm", &txtrs2.width, &txtrs2.height);
	txtrs2.addr = mlx_get_data_addr(txtrs2.img, &txtrs2.bpp, &txtrs2.width, &txtrs2.endian);
	grass.img = mlx_new_image(game.mlx, 32, 32);
	grass.addr = mlx_get_data_addr(grass.img, &grass.bpp, &grass.width, &grass.endian);
	wall.img = mlx_new_image(game.mlx, 32, 32);
	wall.addr = mlx_get_data_addr(wall.img, &wall.bpp, &wall.width, &wall.endian);
	water.img = mlx_new_image(game.mlx, 32, 32);
	water.addr = mlx_get_data_addr(water.img, &water.bpp, &water.width, &water.endian);
	collect.img = mlx_new_image(game.mlx, 32, 32);
	collect.addr = mlx_get_data_addr(collect.img, &collect.bpp, &collect.width, &collect.endian);
	exit.img = mlx_new_image(game.mlx, 32, 32);
	exit.addr = mlx_get_data_addr(exit.img, &exit.bpp, &exit.width, &exit.endian);
	player.img = mlx_new_image(game.mlx, 32, 32);
	player.addr = mlx_get_data_addr(player.img, &player.bpp, &player.width, &player.endian);
	fill_grass_img(&txtrs1, &grass);
	fill_wall_img(&txtrs1, &wall);
	fill_water_img(&txtrs1, &water);
	fill_collect_img(&txtrs1, &collect, &grass);
	fill_exit_img(&txtrs1, &exit, &grass);
	fill_player_img(&txtrs2, &player, &grass);

	game.screen.map = malloc(sizeof(char *) * (game.screen.size.y + 1));
	if (!game.screen.map)
	{
		//handle
		;
	}
	(game.screen.map)[game.screen.size.y] = NULL;
	y = 0;
	while (y < game.screen.size.y)
	{
		(game.screen.map)[y] = malloc(sizeof(char) * (game.screen.size.x + 1));
		if (!((game.screen.map)[y]))
		{
			//handle
			;
		}
		(game.screen.map)[y][game.screen.size.x] = '\0';
		y++;
	}

	int	i;
	int	j = 0;
	if (game.width <= game.screen.size.x)
	{
		i = game.screen.size.x - game.width;
		y = 0;
		while (y < game.screen.size.y)
		{
			ft_memset((game.screen.map)[y], 'W', (i / 2) + (i % 2 != 0));
			if (game.height <= game.screen.size.y)
			{
				if (!j)
					j = game.screen.size.y - game.height;
				if (y < (j / 2) + (j % 2 != 0) || y >= game.screen.size.y - j / 2)
					ft_memset((game.screen.map)[y] + (i / 2) + (i % 2 != 0), 'W', game.screen.size.x - (i / 2) - (i % 2 != 0));
				else
				{
					ft_memcpy((game.screen.map)[y] + (i / 2) + (i % 2 != 0), (game.map)[y - (j / 2) - (j % 2 != 0)], game.width);
					ft_memset((game.screen.map)[y] + game.screen.size.x - i / 2, 'W', i / 2);
				}
			}
			else
				ft_memset((game.screen.map)[y] + game.screen.size.x - i / 2, 'W', i / 2);
			y++;
		}
	}
	else
	{
		y = 0;
		while (y < game.screen.size.y)
		{
			ft_memset((game.screen.map)[y], 'W', game.screen.size.x);
			y++;
		}
	}
	
	ft_putendl_fd("screen :", 1);
	print_map(game.screen.map, 1);
	y = 0;
	char c;
	while (y  < game.res.y)
	{
		x = 0;
		while (x < game.res.x)
		{
			c = (game.screen.map)[y / 32][x / 32];
			if (c == 'W')
				mlx_put_image_to_window(game.mlx, game.win, water.img, x, y);
			else if (c == '1')
				mlx_put_image_to_window(game.mlx, game.win, wall.img, x, y);
			else if (c == 'C')
				mlx_put_image_to_window(game.mlx, game.win, collect.img, x, y);
			else if (c == 'E')
				mlx_put_image_to_window(game.mlx, game.win, exit.img, x, y);
			else if (c == 'P')
				mlx_put_image_to_window(game.mlx, game.win, player.img, x, y);
			else
				mlx_put_image_to_window(game.mlx, game.win, grass.img, x, y);
			x += 32;
		}
		y += 32;
	}
	mlx_loop(game.mlx);
	ft_free_tab((void **)(game.map));
	return (0);
}
