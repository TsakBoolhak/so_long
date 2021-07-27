#include "so_long_structures.h"
#include "libft.h"
#include "mlx.h"
#include "stdlib.h"

void	free_player_datas(t_game *game, t_character *player)
{
	if (player->front.img)
		mlx_destroy_image(game->mlx, player->front.img);
	if (player->back.img)
		mlx_destroy_image(game->mlx, player->back.img);
	if (player->right.img)
		mlx_destroy_image(game->mlx, player->right.img);
	if (player->left.img)
		mlx_destroy_image(game->mlx, player->left.img);
}

void	free_all_datas(t_game *game)
{
	if (game->water.img)
		mlx_destroy_image(game->mlx, game->water.img);
	if (game->wall.img)
		mlx_destroy_image(game->mlx, game->wall.img);
	if (game->grass.img)
		mlx_destroy_image(game->mlx, game->grass.img);
	if (game->exit.img)
		mlx_destroy_image(game->mlx, game->exit.img);
	if (game->collect.img)
		mlx_destroy_image(game->mlx, game->collect.img);
	if (game->screen.img.img)
		mlx_destroy_image(game->mlx, game->screen.img.img);
	ft_free_tab((void **)(game->screen.map));
	ft_free_tab((void **)(game->map));
	free_player_datas(game, &game->player);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
	}
}
