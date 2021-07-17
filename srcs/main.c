#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "libft.h"

typedef struct s_game
{
	int		height;
	int		width;
	char	**map;
} t_game;

int	main(int ac, char **av)
{
	char	*extension;
	t_game	game;
	int		fd;
	char	*line;
	int		ret;
	int		i;
	int		empty;
	int		starting_spaces;

	ft_memset(&game, 0, sizeof(game));
	if (ac != 2)
	{
		ft_putendl_fd("Error\nWrong number of arguments", 2);
		return (0);
	}
	extension = ft_strrchr(av[1], '.');
	if (!extension || ft_strcmp(extension, ".ber"))
	{
		ft_putendl_fd("Error\nWrong configuration file's name (must have .ber extension)", 2);
		return (0);
	}
	fd = open(av[1], O_RDONLY);
	if (fd < 2)
	{
		ft_putendl_fd("Error\nCouldn't open file. Check if the file exist or if it has read access", 2);
		return (0);
	}
	ret = 1;
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
		while (line[i] == ' ')
			i++;
		if (!empty && line[i])
			empty++;
		else if (empty == 1 && !line[i])
		{
			empty++;
			free(line);
			continue ;
		}
		else if (empty == 2 && line[i])
		{
			free(line);
		    get_next_line(fd, &line, 2);
			ft_putendl_fd("Error\nFile contains more than one map", 2);
			close(fd);
			return (0);
		}
		else if (!line[i])
		{
			free(line);
			continue ;
		}
		if (starting_spaces == -1)
			starting_spaces = i;
		else if (i != starting_spaces)
		{
			ft_putendl_fd("Error\nFile's map isn't rectangular", 2);
		    get_next_line(fd, &line, 2);
			free(line);
			close(fd);
			return (0);
		}
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
	close(fd);
	return (0);
}
