#include "so_long_structures.h"
#include "so_long.h"
#include "libft.h"
#include "mlx.h"

int	main(int ac, char **av)
{
	t_game	game;

	ft_memset(&game, 0, sizeof(game));
	if (parser(ac, av, &game) || init_game(&game))
	{
		free_all_datas(&game);
		return (0);
	}
	scroll_screen(&game);
	mlx_loop_hook(game.mlx, &render_frame, &game);
	mlx_hook(game.win, 2, 1L << 0, &handle_keypress, &game);
	mlx_hook(game.win, 33, 1L << 17, &mlx_loop_end, game.mlx);
	mlx_loop(game.mlx);
	free_all_datas(&game);
	return (0);
}
