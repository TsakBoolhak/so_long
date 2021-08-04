#include "libft.h"
#include "so_long_structures_bonus.h"
#include "so_long_bonus.h"

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
