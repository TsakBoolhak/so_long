#include "libft.h"
#include "so_long_structures_bonus.h"
#include "so_long_bonus.h"

t_coord	get_next_pos(t_game *game, t_infos *infos)
{
	t_coord	pos;

	pos.x = infos->map_pos.x;
	pos.y = infos->map_pos.y;
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

int	check_other_moves(t_game *game, t_coord step, t_list *lst)
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

int	crossing_player(t_game *game, t_infos *infos)
{
	if ((infos->dir == 0 && game->player.dir == 2)
		|| (infos->dir == 2 && game->player.dir == 0)
		|| (infos->dir == 1 && game->player.dir == 3)
		|| (infos->dir == 3 && game->player.dir == 1))
	{
		ft_putendl_fd("Outch! I've been bitten by a dog! GAME OVER", 1);
		game->quit = 1;
		game->sentence = 5;
		return (-1);
	}
	return (0);
}

int	check_next_step(t_game *game, t_infos *info, t_coord step, t_list *lst)
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
		if (crossing_player(game, info))
			return (-1);
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
