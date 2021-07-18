#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "libft.h"

typedef struct s_game
{
	int		height;
	int		width;
	char	**map;
}t_game;

typedef struct s_parse
{
	int	exit;
	int	start;
	int	collectible;
	int	error;
}t_parse;

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
	ft_putstr_fd(": '0'(zero) for an empty space, '1' (one) for a wall, ", 2);
	ft_putstr_fd("'C' for a collectible, 'E' for map exit and ", 2);
	ft_putendl_fd("'P' for the player's starting position.", 2);
	ft_putendl_fd("- Map must be closed/surrounded by walls", 2);
	ft_putendl_fd("- Map must contain at least 1 exit and 1 collectible", 2);
	ft_putendl_fd("- Map must be rectangular", 2);
	ft_putstr_fd("- Map must contain one and only one ", 2);
	ft_putendl_fd("player's starting position\n", 2);
	ft_putendl_fd("Here is an exemple of a simple valid map", 2);
	ft_putendl_fd("1111111\n1C01EP1\n10C11C1\n1C00001\n1111111", 2);
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

void	print_line_error(char *line, int row, int *no_print)
{
	if (!(*no_print))
	{
		ft_putstr_fd("Error\nMap is not valid\n\nError occured in line #", 2);
		ft_putnbr_fd(row, 2);
		ft_putstr_fd(":\n\"", 2);
		ft_putstr_fd(line, 2);
		ft_putendl_fd("\"", 2);
		*no_print = -1;
	}
}

int	check_line(char **line, t_game *game, t_parse *parse)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(*line);
	if (len < 3)
	{
		print_line_error(*line, game->height, &parse->error);
		ft_putendl_fd("-Line is too short to contain a closed map", 2);
	}
	if (!game->width)
		game->width = len;
	else if (len != game->width)
	{
		print_line_error(*line, game->height, &parse->error);
		ft_putendl_fd("-Map is not rectangular", 2);
	}
	return (parse->error);
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
		{
			get_next_line(fd, &line, 2);
			ft_putendl_fd("Error\nError occured while reading file.", 2);
			return (-1);
		}
		else if (!ret)
			break;
		game->map = ft_add_str_to_tab(game->map, line);
		(game->height)++;
		if (check_line(&line, game, parse))
		{
			get_next_line(fd, &line, 2);
			break;
		}
	}
	return (parse->error);
}

void	init_parsing(t_game *game, t_parse *parse)
{
	game->height = 0;
	game->width = 0;
	game->map = NULL;
	ft_memset(parse, 0, sizeof(*parse));
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
		close(fd);
		ft_free_tab((void **)(game->map));
		print_map_rules();
		return (-1);
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_game	game;
/*	int		fd;
	char	*line;
	int		ret;
	int		i;
	int		empty;
	int		starting_spaces;
*/
	if (parser(ac, av, &game))
		return (0);
/*	ret = 1;
	empty = 0;
	starting_spaces = -1;
	game.width = -1;
	line = NULL;
	while (ret > 0)
	{
		ret = get_next_line(fd, &line, 0);
		if (ret == -1)
		{
			ft_putendl_fd("Error\nError occured while reading file.", 2);
			close(fd);
			return (0);
		}
		else if (!ret)
			break;
		i = 0;
		ft_putendl_fd(line, 1);
		while (line[i] && line[i] != ' ')
		{
			if (!ft_strchr("01ECP", line[i]))
			{
				free(line);
		    	get_next_line(fd, &line, 2);
				ft_putendl_fd("Error\nFile's map contain invalid character", 2);
				close(fd);
				return (0);
			}
			i++;
		}
		if (game.width == -1)
			game.width = i - starting_spaces;
		else if (game.width != i - starting_spaces)
		{
		    get_next_line(fd, &line, 2);
			free(line);
			ft_putendl_fd("Error\nFile's map isn't rectangular", 2);
			close(fd);
			return (0);
		}
		while (line[i] == ' ')
			i++;
		if (line[i])
		{
		    get_next_line(fd, &line, 2);
			free(line);
			ft_putendl_fd("Error\nFile contains more than one map", 2);
			close(fd);
			return (0);
		}
		free(line);
	}
	close(fd);*/
	ft_free_tab((void **)(game.map));
	return (0);
}
