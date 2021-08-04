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

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bpp;
	int		width;
	int		height;
	int		end;
}t_img;

typedef struct s_infos
{
	int		dir;
	int		step;
	t_coord	map_pos;
	t_coord	screen_pos;
}t_infos;

typedef struct s_ennemies
{
	t_img	front_1;
	t_img	front_2;
	t_img	front_3;
	t_img	back_1;
	t_img	back_2;
	t_img	back_3;
	t_img	right_1;
	t_img	right_2;
	t_img	right_3;
	t_img	left_1;
	t_img	left_2;
	t_img	left_3;
	t_list	*infos;
}t_ennemies;

typedef struct s_character
{
	t_img	front_1;
	t_img	front_2;
	t_img	front_3;
	t_img	back_1;
	t_img	back_2;
	t_img	back_3;
	t_img	right_1;
	t_img	right_2;
	t_img	right_3;
	t_img	left_1;
	t_img	left_2;
	t_img	left_3;
	int		state;
	int		dir;
	t_coord	pos;
}t_character;

typedef struct s_screen
{
	t_coord	size;
	t_coord	player;
	t_img	img;
	char	**map;
}t_screen;

typedef struct s_game
{
	int			height;
	int			width;
	int			moves;
	int			collect_nb;
	int			quit;
	int			quit_time;
	int			sentence;
	char		**map;
	t_screen	screen;
	t_coord		res;
	t_img		water;
	t_img		wall;
	t_img		grass;
	t_img		exit;
	t_img		collect;
	t_img		font;
	t_img		letter;
	t_character	player;
	t_ennemies	ennemies;
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

int	return_error(char *error_msg, int return_value)
{
	ft_putendl_fd("Error", 2);
	ft_putendl_fd(error_msg, 2);
	return (return_value);
}

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
	ft_putendl_fd("- Map must be composed of only 5 possible characters :", 2);
	ft_putendl_fd("\t'0' (zero) for an empty space\n\t'1' (one) for a wall", 2);
	ft_putendl_fd("\t'C' for a collectible\n\t'E' for an exit\n", 2);
	ft_putendl_fd("\t'P' for the player's starting position", 2);
	ft_putendl_fd("\t'G' for a guard that will patrol", 2);
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

int	store_ennemies(t_game *game, int x, int y)
{
	t_list	*new;
	t_infos	*infos;

	infos = malloc(sizeof(t_infos));
	if (!infos)
		return (-1);
	new = ft_lstnew(infos);
	if (!new)
	{
		free(infos);
		return (-1);
	}
	infos->map_pos.x = x;
	infos->map_pos.y = y;
	infos->screen_pos.x = -1;
	infos->screen_pos.y = -1;
	infos->dir = 3;
	infos->step = 0;
	ft_lstadd_back(&game->ennemies.infos, new);
	return (0);
}

int	handle_valid_char(char c, int i, t_game *game, t_parse *parse)
{
	if (c == 'P')
	{
		game->player.pos.x = i;
		game->player.pos.y = game->height - 1;
	}
	else if (c == 'C')
		(parse->collectible)++;
	else if (c == 'E')
		(parse->exit)++;
	else if (c == 'G')
		return (store_ennemies(game, i, game->height - 1));
	return (0);
}

int	check_char(char *line, int i, t_game *game, t_parse *parse)
{
	if (line[i] != '1' && (!i || game->height == 1 || i == game->width - 1))
	{
		print_line_error(line, game->height, &parse->line_error, game->map);
		print_char_error(line[i], i + 1, &parse->char_error);
		ft_putstr_fd(" -Map is not closed", 2);
	}
	if (!ft_strchr("01ECPG", line[i]))
	{
		print_line_error(line, game->height, &parse->line_error, game->map);
		print_char_error(line[i], i + 1, &parse->char_error);
		ft_putstr_fd(" -Not a valid character", 2);
	}
	if (line[i] == 'P' && game->player.pos.x != -1)
	{
		print_line_error(line, game->height, &parse->line_error, game->map);
		print_char_error(line[i], i + 1, &parse->char_error);
		ft_putstr_fd(" -Map contain more than one player spawn", 2);
	}
	if	(handle_valid_char(line[i], i, game, parse))
			return (-1);
	if (parse->char_error)
		ft_putendl_fd("", 2);
	return (0);
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
		if (check_char(line, i, game, parse))
			return (return_error("Couldn't allocate enough memory", -1));
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
	if (!parse->collectible || !parse->exit || game->player.pos.x == -1)
		print_last_error(parse, game->map, game->player.pos.x);
	game->collect_nb = parse->collectible;
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
			return (return_error("Error occured while reading file.", -1));
		else if (!ret)
			break ;
		game->map = ft_add_str_to_tab(game->map, line);
		if (!game->map)
			return (return_error("Couldn't allocate enough memory", -1));
		(game->height)++;
		if (check_line(line, game, parse))
			return (-1);
	}
	if (game->height < 3)
	{
		ft_putendl_fd("Error\nFile is too small to be rectangular", 2);
		return (-1);
	}
	last_parsing_checks((game->map)[game->height - 1], game, parse);
	return (parse->line_error);
}

void	init_parsing(t_game *game, t_parse *parse)
{
	ft_memset(parse, 0, sizeof(*parse));
	game->player.pos.x = -1;
	game->player.pos.y = -1;
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
/*
void	put_img_to_img(t_img *dst, t_img *src, int x_start, int y_start)
{
	t_coord	dst_pos;
	t_coord	src_pos;
	char	*addr_dst;
	char	*addr_src;
	int		color;

	dst_pos.y = y_start;
	src_pos.y = 0;
	while (dst_pos.y < y_start + 32 )
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
*/

void	put_img_to_img(t_img *dst, t_img *src, int x_start, int y_start)
{
	t_coord	dst_pos;
	t_coord	src_pos;
	char	*addr_dst;
	char	*addr_src;
	int		color;

	dst_pos.y = y_start;
	src_pos.y = 0;
	if (y_start < 0)
	{
		dst_pos.y = 0;
		src_pos.y = -y_start;
	}
	while (dst_pos.y < y_start + 32 && dst_pos.y < dst->height)
	{
		dst_pos.x = x_start;
		src_pos.x = 0;
		if (x_start < 0)
		{
			dst_pos.x = 0;
			src_pos.x = -x_start;
		}
		while (dst_pos.x < x_start + 32 && dst_pos.x * 4 < dst->width)
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
	img.height = game->res.y;
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

int	ft_strchr_index(char *s, char c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	if (s[i])
		return (i);
	return (-1);
}

void	get_font_pos(char c, t_coord *pos)
{
	int i;

	if (c == ' ')
		return ;
	i = ft_strchr_index(" !\"#$%&'()*+,-.", c);
	if (i > -1)
	{
		pos->x = 40 * i + 4;
		pos->y = 4;
		return ;
	}
	i = ft_strchr_index("/0123456789:;<=", c);
	if (i > -1)
	{
		pos->x = 40 * i + 4;
		pos->y = 44;
		return ;
	}
	i = ft_strchr_index(">?@ABCDEFGHIJKL", c);
	if (i > -1)
	{
		pos->x = 40 * i + 4;
		pos->y = 84;
		return ;
	}
	i = ft_strchr_index("NMOPQRSTUVWXYZ|", c);
	if (i > -1)
	{
		pos->x = 40 * i + 4;
		pos->y = 124;
		return ;
	}
	i = ft_strchr_index("\\]^_`abcdefghij", c);
	if (i > -1)
	{
		pos->x = 40 * i + 4;
		pos->y = 164;
		return ;
	}
	i = ft_strchr_index("klmnopqrstuvwxy", c);
	if (i > -1)
	{
		pos->x = 40 * i + 4;
		pos->y = 204;
		return ;
	}
	if (c == 'z')
	{
		pos->x = 4;
		pos->y = 244;
	}
}

void	putstr_to_img(t_game *game, char *str, int x, int y)
{
	int	i;
	t_coord	pos;

	i = 0;
	while (str[i] && 32 * i < game->res.x - x)
	{
		ft_memset(&pos, -1, sizeof(t_coord));
		ft_memset(game->letter.addr, 0, 32 * 32 * 4);
		get_font_pos(str[i], &pos);
		if (pos.x >= 0)
		{
			fill_texture_img(&game->font, &game->letter, pos.x, pos.y);
			put_img_to_img(&game->screen.img, &game->letter, x + 32 * i, y);
		}
		i++;
	}
}

int	init_font(t_game *game, char *font)
{
	game->font.img = mlx_xpm_file_to_image(game->mlx, font, &game->font.width, &game->font.height);
	if (!game->font.img)
		return (return_error("Couldn't load the texture pack", -1));
	game->font.addr = mlx_get_data_addr(game->font.img, &game->font.bpp, &game->font.width, &game->font.end);
	game->letter.img = mlx_new_image(game->mlx, 32, 32);
	if (!game->letter.img)
		return (return_error("Couldn't allocate enough memory", -1));
	game->letter.addr = mlx_get_data_addr(game->letter.img, &game->letter.bpp, &game->letter.width, &game->letter.end);
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

int	create_ennemies_textures(t_game *game, t_ennemies *ennemies)
{
	ennemies->front_1.img = mlx_new_image(game->mlx, 32, 32);
	ennemies->front_2.img = mlx_new_image(game->mlx, 32, 32);
	ennemies->front_3.img = mlx_new_image(game->mlx, 32, 32);
	ennemies->back_1.img = mlx_new_image(game->mlx, 32, 32);
	ennemies->back_2.img = mlx_new_image(game->mlx, 32, 32);
	ennemies->back_3.img = mlx_new_image(game->mlx, 32, 32);
	ennemies->right_1.img = mlx_new_image(game->mlx, 32, 32);
	ennemies->right_2.img = mlx_new_image(game->mlx, 32, 32);
	ennemies->right_3.img = mlx_new_image(game->mlx, 32, 32);
	ennemies->left_1.img = mlx_new_image(game->mlx, 32, 32);
	ennemies->left_2.img = mlx_new_image(game->mlx, 32, 32);
	ennemies->left_3.img = mlx_new_image(game->mlx, 32, 32);
	if (!ennemies->front_1.img || !ennemies->front_2.img || !ennemies->front_3.img
		|| !ennemies->back_1.img || !ennemies->back_2.img || !ennemies->back_3.img
		|| !ennemies->right_1.img || !ennemies->right_2.img || !ennemies->right_3.img
		|| !ennemies->left_1.img || !ennemies->left_2.img || !ennemies->left_3.img)
		return (-1);
	return (0);
}

int	create_player_textures(t_game *game, t_character *player)
{
	player->front_1.img = mlx_new_image(game->mlx, 32, 32);
	player->front_2.img = mlx_new_image(game->mlx, 32, 32);
	player->front_3.img = mlx_new_image(game->mlx, 32, 32);
	player->back_1.img = mlx_new_image(game->mlx, 32, 32);
	player->back_2.img = mlx_new_image(game->mlx, 32, 32);
	player->back_3.img = mlx_new_image(game->mlx, 32, 32);
	player->right_1.img = mlx_new_image(game->mlx, 32, 32);
	player->right_2.img = mlx_new_image(game->mlx, 32, 32);
	player->right_3.img = mlx_new_image(game->mlx, 32, 32);
	player->left_1.img = mlx_new_image(game->mlx, 32, 32);
	player->left_2.img = mlx_new_image(game->mlx, 32, 32);
	player->left_3.img = mlx_new_image(game->mlx, 32, 32);
	if (!player->front_1.img || !player->front_2.img || !player->front_3.img
		|| !player->back_1.img || !player->back_2.img || !player->back_3.img
		|| !player->right_1.img || !player->right_2.img || !player->right_3.img
		|| !player->left_1.img || !player->left_2.img || !player->left_3.img)
		return (-1);
	return (0);
}

void	fill_ennemies_texture(t_img *pack, t_ennemies *ennemies)
{
	fill_texture_img(pack, &ennemies->front_1, 32, 0);
	fill_texture_img(pack, &ennemies->front_2, 0, 0);
	fill_texture_img(pack, &ennemies->front_3, 64, 0);
	fill_texture_img(pack, &ennemies->back_1, 32, 96);
	fill_texture_img(pack, &ennemies->back_2, 0, 96);
	fill_texture_img(pack, &ennemies->back_3, 64, 96);
	fill_texture_img(pack, &ennemies->right_1, 32, 64);
	fill_texture_img(pack, &ennemies->right_2, 0, 64);
	fill_texture_img(pack, &ennemies->right_3, 64, 64);
	fill_texture_img(pack, &ennemies->left_1, 32, 32);
	fill_texture_img(pack, &ennemies->left_2, 0, 32);
	fill_texture_img(pack, &ennemies->left_3, 64, 32);
}

void	fill_player_texture(t_img *pack, t_character *player)
{
	fill_texture_img(pack, &player->front_1, 32, 0);
	fill_texture_img(pack, &player->front_2, 0, 0);
	fill_texture_img(pack, &player->front_3, 64, 0);
	fill_texture_img(pack, &player->back_1, 32, 96);
	fill_texture_img(pack, &player->back_2, 0, 96);
	fill_texture_img(pack, &player->back_3, 64, 96);
	fill_texture_img(pack, &player->right_1, 32, 64);
	fill_texture_img(pack, &player->right_2, 0, 64);
	fill_texture_img(pack, &player->right_3, 64, 64);
	fill_texture_img(pack, &player->left_1, 32, 32);
	fill_texture_img(pack, &player->left_2, 0, 32);
	fill_texture_img(pack, &player->left_3, 64, 32);
}

void	setup_ennemies_addr(t_game *game, t_img *tmp)
{
	t_img	*img;

	tmp->addr = mlx_get_data_addr(tmp->img, &tmp->bpp, &tmp->width, &tmp->end);
	img = &game->ennemies.front_2;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->ennemies.front_3;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->ennemies.back_1;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->ennemies.back_2;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->ennemies.back_3;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->ennemies.right_1;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->ennemies.right_2;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->ennemies.right_3;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->ennemies.left_1;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->ennemies.left_2;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->ennemies.left_3;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
}

void	setup_player_addr(t_game *game, t_img *tmp)
{
	t_img	*img;

	tmp->addr = mlx_get_data_addr(tmp->img, &tmp->bpp, &tmp->width, &tmp->end);
	img = &game->player.front_2;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->player.front_3;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->player.back_1;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->player.back_2;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->player.back_3;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->player.right_1;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->player.right_2;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->player.right_3;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->player.left_1;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->player.left_2;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->player.left_3;
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
	setup_player_addr(game, &player->front_1);
	fill_player_texture(&pack, player);
	mlx_destroy_image(game->mlx, pack.img);
	return (0);
}

int	init_ennemies(t_game *game, char *name)
{
	t_img		pack;
	t_ennemies	*ennemies;
	void		*mlx;

	mlx = game->mlx;
	ennemies = &game->ennemies;
	pack.img = mlx_xpm_file_to_image(mlx, name, &pack.width, &pack.height);
	if (!pack.img)
		return (return_error("Couldn't load the texture pack", -1));
	if (create_ennemies_textures(game, ennemies))
	{
		mlx_destroy_image(game->mlx, pack.img);
		return (return_error("Couldn't allocate enough memory", -1));
	}
	pack.addr = mlx_get_data_addr(pack.img, &pack.bpp, &pack.width, &pack.end);
	setup_ennemies_addr(game, &ennemies->front_1);
	fill_ennemies_texture(&pack, ennemies);
	mlx_destroy_image(game->mlx, pack.img);
	return (0);
}

int	init_game(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		return (return_error("Couldn't allocate enough memory", -1));
	if (init_screen(game) || init_textures(game, "pack_1.xpm")
		|| init_player(game, "cat1.xpm") || init_font(game, "Font.xpm")
		|| init_ennemies(game, "dog1.xpm"))
		return (-1);
	return (0);
}

void	free_player_datas(t_game *game, t_character *player)
{
	if (player->front_1.img)
		mlx_destroy_image(game->mlx, player->front_1.img);
	if (player->front_2.img)
		mlx_destroy_image(game->mlx, player->front_2.img);
	if (player->front_3.img)
		mlx_destroy_image(game->mlx, player->front_3.img);
	if (player->back_1.img)
		mlx_destroy_image(game->mlx, player->back_1.img);
	if (player->back_2.img)
		mlx_destroy_image(game->mlx, player->back_2.img);
	if (player->back_3.img)
		mlx_destroy_image(game->mlx, player->back_3.img);
	if (player->right_1.img)
		mlx_destroy_image(game->mlx, player->right_1.img);
	if (player->right_2.img)
		mlx_destroy_image(game->mlx, player->right_2.img);
	if (player->right_3.img)
		mlx_destroy_image(game->mlx, player->right_3.img);
	if (player->left_1.img)
		mlx_destroy_image(game->mlx, player->left_1.img);
	if (player->left_2.img)
		mlx_destroy_image(game->mlx, player->left_2.img);
	if (player->left_3.img)
		mlx_destroy_image(game->mlx, player->left_3.img);
}

void	free_ennemies_datas(t_game *game, t_ennemies *ennemies)
{
	if (ennemies->front_1.img)
		mlx_destroy_image(game->mlx, ennemies->front_1.img);
	if (ennemies->front_2.img)
		mlx_destroy_image(game->mlx, ennemies->front_2.img);
	if (ennemies->front_3.img)
		mlx_destroy_image(game->mlx, ennemies->front_3.img);
	if (ennemies->back_1.img)
		mlx_destroy_image(game->mlx, ennemies->back_1.img);
	if (ennemies->back_2.img)
		mlx_destroy_image(game->mlx, ennemies->back_2.img);
	if (ennemies->back_3.img)
		mlx_destroy_image(game->mlx, ennemies->back_3.img);
	if (ennemies->right_1.img)
		mlx_destroy_image(game->mlx, ennemies->right_1.img);
	if (ennemies->right_2.img)
		mlx_destroy_image(game->mlx, ennemies->right_2.img);
	if (ennemies->right_3.img)
		mlx_destroy_image(game->mlx, ennemies->right_3.img);
	if (ennemies->left_1.img)
		mlx_destroy_image(game->mlx, ennemies->left_1.img);
	if (ennemies->left_2.img)
		mlx_destroy_image(game->mlx, ennemies->left_2.img);
	if (ennemies->left_3.img)
		mlx_destroy_image(game->mlx, ennemies->left_3.img);
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
	if (game->font.img)
		mlx_destroy_image(game->mlx, game->font.img);
	if (game->letter.img)
		mlx_destroy_image(game->mlx, game->letter.img);
	ft_free_tab((void **)(game->screen.map));
	ft_free_tab((void **)(game->map));
	free_player_datas(game, &game->player);
	free_ennemies_datas(game, &game->ennemies);
	ft_lstclear(&game->ennemies.infos, &free);
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

void	update_data(t_game *game , t_coord map, t_coord screen)
{
	t_list	*tmp;
	t_infos	*ptr;

	tmp = game->ennemies.infos;
	while (game->ennemies.infos)
	{
		ptr = game->ennemies.infos->content;
		if (ptr->map_pos.x >= map.x && ptr->map_pos.x - map.x + screen.x < game->screen.size.x)
			ptr->screen_pos.x = ptr->map_pos.x - map.x + screen.x;
		else
			ptr->screen_pos.x = -1;
		if (ptr->map_pos.y >= map.y && ptr->map_pos.y - map.y + screen.y < game->screen.size.y)
			ptr->screen_pos.y = ptr->map_pos.y - map.y + screen.y;
		else
			ptr->screen_pos.y = -1;
		game->ennemies.infos = game->ennemies.infos->next;
	}
	game->ennemies.infos = tmp;
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
	update_data(game , map_pos, screen_pos);
}

void	detect_collect_exit(t_game *game, char *game_map_c, char *screen_map_c)
{
	if (game->sentence != 5)
		game->sentence = 0;
	if (*screen_map_c == 'C' && !game->quit)
	{
		ft_putstr_fd("Hey, this stuff seem usefull, let's keep it ", 1);
		game->collect_nb--;
		*game_map_c = '0';
		*screen_map_c = '0';
		game->sentence = 1;
	}
	else if (*screen_map_c == 'E' && !game->quit)
	{
		if (game->collect_nb)
		{
			ft_putstr_fd("Hmmm, i think there are still things to do here ", 1);
			game->sentence = 2;
		}
		else
		{
			game->moves++;
			ft_putstr_fd("Yay! My job is done here!\n", 1);
			ft_putstr_fd("Congratulations, you completed this map with ", 1);
			ft_putnbr_fd(game->moves, 1);
			ft_putstr_fd(" moves!\n", 1);
			game->sentence = 3;
			game->quit = 1;
		}
	}
}

void	print_moves(t_game *game)
{
	if (!game->quit)
	{
		ft_putstr_fd("Total moves : ", 1);
		ft_putnbr_fd(game->moves, 1);
		ft_putendl_fd("", 1);
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
	if (map[pos.y][pos.x] == '1' && !game->quit)
	{
		game->sentence = 4;
		ft_putstr_fd("Outch, That's a wall!(doesn't count as a move) ", 1);
	}
	else
	{
		game->moves++;
		game->screen.player.y--;
		game->player.pos.y--;
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
	if (map[pos.y][pos.x] == '1' && !game->quit)
	{
		game->sentence = 4;
		ft_putstr_fd("Outch, That's a wall!(doesn't count as a move) ", 1);
	}
	else
	{
		game->moves++;
		game->screen.player.y++;
		game->player.pos.y++;
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
	if (map[pos.y][pos.x] == '1' && !game->quit)
	{
		game->sentence = 4;
		ft_putstr_fd("Outch, That's a wall!(doesn't count as a move) ", 1);
	}
	else
	{
		game->moves++;
		game->screen.player.x++;
		game->player.pos.x++;
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
	if (map[pos.y][pos.x] == '1' && !game->quit)
	{
		game->sentence = 4;
		ft_putstr_fd("Outch, That's a wall! (doesn't count as a move)\t", 1);
	}
	else
	{
		game->moves++;
		game->screen.player.x--;
		game->player.pos.x--;
	}
	print_moves(game);
}

int	handle_keypress(int key, t_game *game)
{
	if (key == 65307)
		mlx_loop_end(game->mlx);
	else if (key == 122 && !game->player.state && !game->quit)
	{
		game->player.dir = 2;
		game->player.state++;
	}
	else if (key == 115 && !game->player.state && !game->quit)
	{
		game->player.dir = 0;
		game->player.state++;
	}
	else if (key == 113 && !game->player.state && !game->quit)
	{
		game->player.dir = 3;
		game->player.state++;
	}
	else if (key == 100 && !game->player.state && !game->quit)
	{
		game->player.dir = 1;
		game->player.state++;
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

t_img	*player_top_left(t_game *game, int frame, t_coord *increm, int mul)
{
	if (game->player.dir == 2)
	{
		increm->y -= (4 * frame);
		mul = ((game->map)[game->player.pos.y - 1][game->player.pos.x] != '1');
		increm->y *= mul;
		if (!frame || frame % 2 == 0)
			return (&game->player.back_1);
		else if (frame == 1 || frame == 5)
			return (&game->player.back_2);
		else
			return (&game->player.back_3);
	}
	else
	{
		increm->x -= (4 * frame);
		mul = ((game->map)[game->player.pos.y][game->player.pos.x - 1] != '1');
		increm->x *= mul;
		if (!frame || frame % 2 == 0)
			return (&game->player.left_1);
		else if (frame == 1 || frame == 5)
			return (&game->player.left_2);
		else
			return (&game->player.left_3);
	}
}

t_img	*player_down_right(t_game *game, int frame, t_coord *increm, int mul)
{
	if (game->player.dir == 0)
	{
		increm->y += (4 * frame);
		mul = ((game->map)[game->player.pos.y + 1][game->player.pos.x] != '1');
		increm->y *= mul;
		if (!frame || frame % 2 == 0)
			return (&game->player.front_1);
		else if (frame == 1 || frame == 5)
			return (&game->player.front_2);
		else
			return (&game->player.front_3);
	}
	else
	{
		increm->x += (4 * frame);
		mul = ((game->map)[game->player.pos.y][game->player.pos.x + 1] != '1');
		increm->x *= mul;
		if (!frame || frame % 2 == 0)
			return (&game->player.right_1);
		else if (frame == 1 || frame == 5)
			return (&game->player.right_2);
		else
			return (&game->player.right_3);
	}
}

t_img	*ennemies_top_left(t_game *game, int frame, t_coord *increm, t_infos *content)
{
	int	mul;

	mul = 0;
	if (content->dir == 2)
	{
		increm->y -= (4 * frame);
		mul = ((game->map)[content->map_pos.y - 1][content->map_pos.x] != '1');
		increm->y *= mul;
		if (!frame || frame % 2 == 0)
			return (&game->ennemies.back_1);
		else if (frame == 1 || frame == 5)
			return (&game->ennemies.back_2);
		else
			return (&game->ennemies.back_3);
	}
	else
	{
		increm->x -= (4 * frame);
		mul = ((game->map)[content->map_pos.y][content->map_pos.x - 1] != '1');
		increm->x *= mul;
		if (!frame || frame % 2 == 0)
			return (&game->ennemies.left_1);
		else if (frame == 1 || frame == 5)
			return (&game->ennemies.left_2);
		else
			return (&game->ennemies.left_3);
	}
}

t_img	*ennemies_down_right(t_game *game, int frame, t_coord *increm, t_infos *content)
{
	int	mul;

	mul = 0;
	if (content->dir == 0)
	{
		increm->y += (4 * frame);
		mul = ((game->map)[content->map_pos.y + 1][content->map_pos.x] != '1');
		increm->y *= mul;
		if (!frame || frame % 2 == 0)
			return (&game->ennemies.front_1);
		else if (frame == 1 || frame == 5)
			return (&game->ennemies.front_2);
		else
			return (&game->ennemies.front_3);
	}
	else
	{
		increm->x += (4 * frame);
		mul = ((game->map)[content->map_pos.y][content->map_pos.x + 1] != '1');
		increm->x *= mul;
		if (!frame || frame % 2 == 0)
			return (&game->ennemies.right_1);
		else if (frame == 1 || frame == 5)
			return (&game->ennemies.right_2);
		else
			return (&game->ennemies.right_3);
	}
}

void	draw_ennemies(t_game *game)
{
	t_list	*tmp;
	t_infos	*content;
	int		frame_chunk;
	t_coord	pos;
	t_img	*img;
	t_coord	increm;

	tmp = game->ennemies.infos;
	if (game->player.state == 0)
		frame_chunk = 0;
	else
		frame_chunk = game->player.state / 8 + 1;
	while (game->ennemies.infos)
	{
		content = game->ennemies.infos->content;
		if (content->screen_pos.x >= 0 && content->screen_pos.y >= 0) 
		{
			ft_memset(&increm, 0, sizeof(t_coord));
			pos.x = content->screen_pos.x * 32;
			pos.y = content->screen_pos.y * 32;
			if (content->dir == 0 || content->dir == 1)
				img = ennemies_down_right(game, frame_chunk, &increm, content);
			else
				img = ennemies_top_left(game, frame_chunk, &increm, content);
			put_img_to_img(&game->screen.img, img, pos.x + increm.x, pos.y + increm.y);
		}
		game->ennemies.infos = game->ennemies.infos->next;
	}
	game->ennemies.infos = tmp;
}

/*
void	draw_ennemies(t_game *game)
{
	t_list	*tmp;
	t_infos	*infos;

	tmp = game->ennemies.infos;
	while (game->ennemies.infos)
	{
		infos = (t_infos*)(game->ennemies.infos->content);
		if (infos->screen_pos.x > -1 && infos->screen_pos.y > -1)
			put_img_to_img(&game->screen.img, &game->ennemies.front_1, infos->screen_pos.x * 32 , infos->screen_pos.y * 32);
		game->ennemies.infos = game->ennemies.infos->next;
	}
	game->ennemies.infos = tmp;
}
*/

void	draw_player(t_game *game)
{
	t_img	*img;
	t_coord	increm;
	int		frame_chunk;
	int		mul;
	t_coord	pos;

	pos.x = game->screen.player.x * 32;
	pos.y = game->screen.player.y * 32;
	ft_memset(&increm, 0, sizeof(t_coord));
	if (game->player.state == 0)
		frame_chunk = 0;
	else
		frame_chunk = game->player.state / 8 + 1;
	mul = 0;
	if (game->player.dir == 0 || game->player.dir == 1)
		img = player_down_right(game, frame_chunk, &increm, mul);
	else
		img = player_top_left(game, frame_chunk, &increm, mul);
	put_img_to_img(&game->screen.img, img, pos.x + increm.x, pos.y + increm.y);
}

t_coord	get_next_pos(t_game *game, t_infos *infos)
{
	t_coord pos;

	pos.x = infos->map_pos.x;
	pos.y = infos->map_pos.y;
	if (game)
		pos.x = infos->map_pos.x;
	if (infos->dir == 0 && game->map[pos.y + 1][pos.x] != '1')
		pos.y++;
	else if (infos->dir == 1 && game->map[pos.y][pos.x + 1] != '1')
		pos.x++;
	else if (infos->dir == 2 && game->map[pos.y - 1][pos.x] != '1')
		pos.y--;
	else if (infos->dir == 3 && game->map[pos.y][pos.x - 1] != '1')
		pos.x--;
	return (pos);
}

int		check_other_moves(t_game *game, t_coord step, t_list *lst)
{
	t_infos	*infos;
	t_coord	pos;

	while (lst)
	{
		infos = (t_infos *)(lst->content);
		pos = get_next_pos(game, infos);
		if (pos.x == step.x && pos.y == step.y)
			return (-1);
		lst = lst->next;
	}
	return (0);
}

int		check_next_step(t_game *game, t_infos *infos, t_coord step, t_list *lst)
{
	t_infos	player;
	t_coord	pos;

	player.map_pos.x = game->player.pos.x;
	player.map_pos.y = game->player.pos.y;
	player.dir = game->player.dir;
	pos = get_next_pos(game, &player);
	if (pos.x == step.x && pos.y == step.y)
	{
		ft_putendl_fd("Outch! I've been bitten by a dog! GAME OVER", 1);
		game->quit = 1;
		game->sentence = 5;
		return (-1);
	}
	else if (game->map[step.y][step.x] == '1')
		return (-1);
	else if (step.x == game->player.pos.x && step.y == game->player.pos.y)
	{
		if ((infos->dir == 0 && game->player.dir == 2) || (infos->dir == 2 && game->player.dir == 0) || (infos->dir == 1 && game->player.dir == 3) || (infos->dir == 3 && game->player.dir == 1))
		{
			ft_putendl_fd("Outch! I've been bitten by a dog! GAME OVER", 1);
			game->quit = 1;
			game->sentence = 5;
			return (-1);
		}
		/*
		else if (infos->dir == 2 && game->player.dir == 0)
			return (-1);
		else if (infos->dir == 1 && game->player.dir == 3)
			return (-1);
		else if (infos->dir == 3 && game->player.dir == 1)
			return (-1);
		*/
	}
	else if (check_other_moves(game, step, lst))
		return (-1);
	return (0);
}

void	move_ennemies(t_game *game)
{
	t_list	*tmp;
	t_infos	*infos;
	t_coord	pos;

	tmp = game->ennemies.infos;
	while (game->ennemies.infos)
	{
		infos = (t_infos *)(game->ennemies.infos->content);
		pos = get_next_pos(game, infos);
		if (!check_next_step(game, infos, pos, game->ennemies.infos->next))
			infos->map_pos = pos;
		infos->step++;
		if (infos->step == 5)
		{
			infos->dir++;
			if (infos->dir == 4)
				infos->dir = 0;
			infos->step = 0;
		}
		game->ennemies.infos = game->ennemies.infos->next;
	}
	game->ennemies.infos = tmp;
}

int	render_frame(t_game *game)
{
	char	*nbr;

	scroll_screen(game);
	fill_screen_buffer(game);
	draw_player(game);
	draw_ennemies(game);
	putstr_to_img(game, "MOVES : ", 0, 0);
	nbr = ft_itoa(game->moves - game->quit);
	if (!nbr)
	{
		ft_putendl_fd("Error\nMemory allocation failed", 2);
		mlx_loop_end(game->mlx);
	}
	putstr_to_img(game, nbr, 8 * 32, 0);
	free(nbr);
	if (game->sentence == 1)
		putstr_to_img(game, "HEY! THIS STUFF SEEMS USEFUL!", 0, game->res.y - 32);
	else if (game->sentence == 2)
		putstr_to_img(game, "I THINK I STILL HAVE THINGS TO DO", 0, game->res.y - 32);
	else if (game->sentence == 3)
		putstr_to_img(game, "MY JOB IS DONE HERE!", 0, game->res.y - 32);
	else if (game->sentence == 4)
		putstr_to_img(game, "AWW!! THIS WALL IS STRONGER THAN ME!", 0, game->res.y - 32);
	else if (game->sentence == 5)
		putstr_to_img(game, "GAME OVER", game->res.x / 2 - 5 * (game->res.x / 2 - 5 >= 0), game->res.y / 2);
	mlx_put_image_to_window(game->mlx, game->win, game->screen.img.img, 0, 0);
	if (game->player.state == 55)
	{
		move_ennemies(game);
		if (game->player.dir == 0)
			move_down(game);
		else if (game->player.dir == 1)
			move_right(game);
		else if (game->player.dir == 2)
			move_up(game);
		else
			move_left(game);
		game->player.state = 0;
	}
	if (game->player.state > 0)
		game->player.state++;
	if (game->quit && game->quit_time == 50)
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
