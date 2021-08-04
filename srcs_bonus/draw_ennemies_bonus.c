#include "libft.h"
#include "so_long_structures_bonus.h"
#include "so_long_bonus.h"

t_img	*foes_top_left(t_game *game, int frame, t_coord *pos, t_infos *infos)
{
	int	mul;

	if (infos->dir == 2)
	{
		pos->y -= (4 * frame);
		mul = ((game->map)[infos->map_pos.y - 1][infos->map_pos.x] != '1');
		pos->y *= mul;
		if (!frame || frame % 2 == 0)
			return (&game->ennemies.back_1);
		else if (frame == 1 || frame == 5)
			return (&game->ennemies.back_2);
		return (&game->ennemies.back_3);
	}
	else
	{
		pos->x -= (4 * frame);
		mul = ((game->map)[infos->map_pos.y][infos->map_pos.x - 1] != '1');
		pos->x *= mul;
		if (!frame || frame % 2 == 0)
			return (&game->ennemies.left_1);
		else if (frame == 1 || frame == 5)
			return (&game->ennemies.left_2);
		return (&game->ennemies.left_3);
	}
}

t_img	*foes_bot_right(t_game *game, int frame, t_coord *pos, t_infos *infos)
{
	int	mul;

	if (infos->dir == 0)
	{
		pos->y += (4 * frame);
		mul = ((game->map)[infos->map_pos.y + 1][infos->map_pos.x] != '1');
		pos->y *= mul;
		if (!frame || frame % 2 == 0)
			return (&game->ennemies.front_1);
		else if (frame == 1 || frame == 5)
			return (&game->ennemies.front_2);
		return (&game->ennemies.front_3);
	}
	else
	{
		pos->x += (4 * frame);
		mul = ((game->map)[infos->map_pos.y][infos->map_pos.x + 1] != '1');
		pos->x *= mul;
		if (!frame || frame % 2 == 0)
			return (&game->ennemies.right_1);
		else if (frame == 1 || frame == 5)
			return (&game->ennemies.right_2);
		return (&game->ennemies.right_3);
	}
}

void	draw_ennemies_loop(t_game *game, t_infos *content, int frame_chunk)
{
	t_coord	increm;
	t_coord	pos;
	t_img	*img;

	if (content->screen_pos.x >= 0 && content->screen_pos.y >= 0)
	{
		ft_memset(&increm, 0, sizeof(t_coord));
		pos.x = content->screen_pos.x * 32;
		pos.y = content->screen_pos.y * 32;
		if (content->dir == 0 || content->dir == 1)
			img = foes_bot_right(game, frame_chunk, &increm, content);
		else
			img = foes_top_left(game, frame_chunk, &increm, content);
		put_img_to_img(&game->screen.img, img, pos.x + increm.x, pos.y
			+ increm.y);
	}
}

void	draw_ennemies(t_game *game)
{
	t_list	*tmp;
	int		frame_chunk;
	t_infos	*content;

	tmp = game->ennemies.infos;
	if (game->player.state == 0)
		frame_chunk = 0;
	else
		frame_chunk = game->player.state / 8 + 1;
	while (game->ennemies.infos)
	{
		content = game->ennemies.infos->content;
		draw_ennemies_loop(game, content, frame_chunk);
		game->ennemies.infos = game->ennemies.infos->next;
	}
	game->ennemies.infos = tmp;
}
