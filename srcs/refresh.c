#include "libft.h"
#include "so_long_structures.h"
#include "so_long.h"

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
		else if ((game->map)[map_pos.y + y - screen_pos.y] + map_pos.x)
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
