#include "so_long_structures.h"
#include "so_long.h"
#include "libft.h"
#include "mlx.h"

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
