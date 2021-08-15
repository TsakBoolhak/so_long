#include <stdlib.h>
#include "mlx.h"
#include "libft.h"
#include "so_long_structures_bonus.h"
#include "so_long_bonus.h"

void	draw_move_count(t_game *game)
{
	char	*nbr;

	putstr_to_img(game, "MOVES : ", 0, 0);
	nbr = ft_itoa(game->moves - game->quit);
	if (!nbr)
	{
		ft_putendl_fd("Error\nMemory allocation failed", 2);
		mlx_loop_end(game->mlx);
	}
	putstr_to_img(game, nbr, 8 * 32, 0);
	free(nbr);
}

void	draw_msg(t_game *game)
{
	int	bottom;

	bottom = game->res.y - 32;
	if (game->sentence == 1)
		putstr_to_img(game, "I'LL TAKE THIS", 0, bottom);
	else if (game->sentence == 2)
		putstr_to_img(game, "I STILL HAVE STUFF TO TAKE", 0, bottom);
	else if (game->sentence == 3)
		putstr_to_img(game, "MY JOB IS DONE HERE!", game->res.x / 2
			- 320 * (game->res.x / 2 - 320 >= 0), game->res.y / 2);
	else if (game->sentence == 4)
		putstr_to_img(game, "AW! THIS WALL HURTS ME!", 0, bottom);
	else if (game->sentence == 5)
		putstr_to_img(game, "GAME OVER", game->res.x / 2
			- 160 * (game->res.x / 2 - 160 >= 0), game->res.y / 2);
}
