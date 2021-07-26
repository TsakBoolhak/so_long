#include "libft.h"
#include "so_long_structures.h"
#include "so_long.h"
#include <unistd.h>
#include <fcntl.h>

void	init_parsing(t_game *game, t_parse *parse)
{
	ft_memset(parse, 0, sizeof(*parse));
	game->player.pos.x = -1;
	game->player.pos.y = -1;
}

void	handle_valid_char(char c, int i, t_game *game, t_parse *parse)
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
		if (!ret)
			break ;
		game->map = ft_add_str_to_tab(game->map, line);
		if (!game->map)
			return (return_error("Couldn't allocate enough memory", -1));
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
