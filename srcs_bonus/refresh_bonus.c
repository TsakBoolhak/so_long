#include "libft.h"
#include "so_long_structures_bonus.h"
#include "so_long_bonus.h"

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

void	update_data(t_game *game, t_coord map, t_coord screen)
{
	t_list	*tmp;
	t_infos	*ptr;

	tmp = game->ennemies.infos;
	while (game->ennemies.infos)
	{
		ptr = game->ennemies.infos->content;
		if (ptr->map_pos.x >= map.x
			&& ptr->map_pos.x - map.x + screen.x < game->screen.size.x)
			ptr->screen_pos.x = ptr->map_pos.x - map.x + screen.x;
		else
			ptr->screen_pos.x = -1;
		if (ptr->map_pos.y >= map.y
			&& ptr->map_pos.y - map.y + screen.y < game->screen.size.y)
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
	y = -1;
	map = game->screen.map;
	while (++y < game->screen.size.y)
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
	}
	update_data(game, map_pos, screen_pos);
}
