#include "mlx.h"
#include "libft.h"
#include "so_long_structures_bonus.h"
#include "so_long_bonus.h"

void	handle_reach_exit(t_game *game)
{
	if (game->collect_nb)
	{
		ft_putstr_fd("Hmmm, i think there are still things to do here ", 1);
		game->sentence = 2;
	}
	else
	{
		game->moves++;
		ft_putstr_fd("Yay! My job is done here!\n", 1);
		ft_putstr_fd("Congratulations, you completed this map with ", 1);
		ft_putnbr_fd(game->moves, 1);
		ft_putstr_fd(" moves!\n", 1);
		game->sentence = 3;
		game->quit = 1;
	}
}

void	detect_collect_exit(t_game *game, char *game_map_c, char *screen_map_c)
{
	if (game->sentence != 5)
		game->sentence = 0;
	if (*screen_map_c == 'C' && !game->quit)
	{
		ft_putstr_fd("Hey, this stuff seem useful, let's keep it ", 1);
		game->collect_nb--;
		*game_map_c = '0';
		*screen_map_c = '0';
		game->sentence = 1;
	}
	else if (*screen_map_c == 'E' && !game->quit)
		handle_reach_exit(game);
}

void	check_last_anim_frame(t_game *game)
{
	if (game->player.state == 55)
	{
		move_ennemies(game);
		if (game->player.dir == 0)
			move_down(game);
		else if (game->player.dir == 1)
			move_right(game);
		else if (game->player.dir == 2)
			move_up(game);
		else
			move_left(game);
		game->player.state = 0;
	}
}

int	render_frame(t_game *game)
{
	scroll_screen(game);
	fill_screen_buffer(game);
	draw_player(game);
	draw_ennemies(game);
	draw_move_count(game);
	draw_msg(game);
	mlx_put_image_to_window(game->mlx, game->win, game->screen.img.img, 0, 0);
	check_last_anim_frame(game);
	if (game->player.state > 0)
		game->player.state++;
	if (game->quit && game->quit_time == 50)
		mlx_loop_end(game->mlx);
	else if (game->quit)
		game->quit_time++;
	return (0);
}
