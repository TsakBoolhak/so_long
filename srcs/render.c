#include "mlx.h"
#include "libft.h"
#include "so_long_structures.h"
#include "so_long.h"

void	detect_collect_exit(t_game *game, char *game_map_c, char *screen_map_c)
{
	if (*screen_map_c == 'C')
	{
		ft_putstr_fd("Hey, this stuff seem usefull, let's keep it ", 1);
		game->collect_nb--;
		*game_map_c = '0';
		*screen_map_c = '0';
	}
	else if (*screen_map_c == 'E')
	{
		if (game->collect_nb)
			ft_putstr_fd("Hmmm, i think there are still things to do here ", 1);
		else
		{
			game->moves++;
			ft_putstr_fd("Yay! My job is done here!\n", 1);
			ft_putstr_fd("Congratulations, you completed this map with ", 1);
			ft_putnbr_fd(game->moves, 1);
			ft_putstr_fd(" moves!\n", 1);
			game->quit = 1;
		}
	}
}

void	fill_screen_buffer(t_game *game)
{
	char	c;
	int		x;
	int		y;

	y = 0;
	while (y < game->res.y)
	{
		x = 0;
		while (x < game->res.x)
		{
			c = (game->screen.map)[y / 32][x / 32];
			if (c == 'W')
				put_img_to_img(&game->screen.img, &game->water, x, y);
			else if (c == '1')
				put_img_to_img(&game->screen.img, &game->wall, x, y);
			else
				put_img_to_img(&game->screen.img, &game->grass, x, y);
			if (c == 'C')
				put_img_to_img(&game->screen.img, &game->collect, x, y);
			else if (c == 'E')
				put_img_to_img(&game->screen.img, &game->exit, x, y);
			x += 32;
		}
		y += 32;
	}
}

int	render_frame(t_game *game)
{
	int	x;
	int	y;

	fill_screen_buffer(game);
	x = game->screen.player.x * 32;
	y = game->screen.player.y * 32;
	if (game->player.dir == 0)
		put_img_to_img(&game->screen.img, &game->player.front, x, y);
	else if (game->player.dir == 1)
		put_img_to_img(&game->screen.img, &game->player.right, x, y);
	else if (game->player.dir == 2)
		put_img_to_img(&game->screen.img, &game->player.back, x, y);
	else
		put_img_to_img(&game->screen.img, &game->player.left, x, y);
	mlx_put_image_to_window(game->mlx, game->win, game->screen.img.img, 0, 0);
	if (game->quit && game->quit_time == 20)
		mlx_loop_end(game->mlx);
	else if (game->quit)
		game->quit_time++;
	return (0);
}
