#include "libft.h"
#include "so_long_structures_bonus.h"

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
	ft_putendl_fd("\t'C' for a collectible\n\t'E' for an exit", 2);
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

void	print_moves(t_game *game)
{
	if (!game->quit)
	{
		ft_putstr_fd("Total moves : ", 1);
		ft_putnbr_fd(game->moves, 1);
		ft_putendl_fd("", 1);
	}
}
