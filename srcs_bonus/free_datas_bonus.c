#include <stdlib.h>
#include "mlx.h"
#include "libft.h"
#include "so_long_structures_bonus.h"
#include "so_long_bonus.h"

void	free_player_datas(t_game *game, t_character *player)
{
	if (player->front_1.img)
		mlx_destroy_image(game->mlx, player->front_1.img);
	if (player->front_2.img)
		mlx_destroy_image(game->mlx, player->front_2.img);
	if (player->front_3.img)
		mlx_destroy_image(game->mlx, player->front_3.img);
	if (player->back_1.img)
		mlx_destroy_image(game->mlx, player->back_1.img);
	if (player->back_2.img)
		mlx_destroy_image(game->mlx, player->back_2.img);
	if (player->back_3.img)
		mlx_destroy_image(game->mlx, player->back_3.img);
	if (player->right_1.img)
		mlx_destroy_image(game->mlx, player->right_1.img);
	if (player->right_2.img)
		mlx_destroy_image(game->mlx, player->right_2.img);
	if (player->right_3.img)
		mlx_destroy_image(game->mlx, player->right_3.img);
	if (player->left_1.img)
		mlx_destroy_image(game->mlx, player->left_1.img);
	if (player->left_2.img)
		mlx_destroy_image(game->mlx, player->left_2.img);
	if (player->left_3.img)
		mlx_destroy_image(game->mlx, player->left_3.img);
}

void	free_ennemies_datas(t_game *game, t_ennemies *ennemies)
{
	if (ennemies->front_1.img)
		mlx_destroy_image(game->mlx, ennemies->front_1.img);
	if (ennemies->front_2.img)
		mlx_destroy_image(game->mlx, ennemies->front_2.img);
	if (ennemies->front_3.img)
		mlx_destroy_image(game->mlx, ennemies->front_3.img);
	if (ennemies->back_1.img)
		mlx_destroy_image(game->mlx, ennemies->back_1.img);
	if (ennemies->back_2.img)
		mlx_destroy_image(game->mlx, ennemies->back_2.img);
	if (ennemies->back_3.img)
		mlx_destroy_image(game->mlx, ennemies->back_3.img);
	if (ennemies->right_1.img)
		mlx_destroy_image(game->mlx, ennemies->right_1.img);
	if (ennemies->right_2.img)
		mlx_destroy_image(game->mlx, ennemies->right_2.img);
	if (ennemies->right_3.img)
		mlx_destroy_image(game->mlx, ennemies->right_3.img);
	if (ennemies->left_1.img)
		mlx_destroy_image(game->mlx, ennemies->left_1.img);
	if (ennemies->left_2.img)
		mlx_destroy_image(game->mlx, ennemies->left_2.img);
	if (ennemies->left_3.img)
		mlx_destroy_image(game->mlx, ennemies->left_3.img);
}

void	free_player_ennemies_map(t_game *game)
{
	ft_free_tab((void **)(game->screen.map));
	ft_free_tab((void **)(game->map));
	free_player_datas(game, &game->player);
	free_ennemies_datas(game, &game->ennemies);
	ft_lstclear(&game->ennemies.infos, &free);
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
	if (game->font.img)
		mlx_destroy_image(game->mlx, game->font.img);
	if (game->letter.img)
		mlx_destroy_image(game->mlx, game->letter.img);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	free_player_ennemies_map(game);
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
	}
}
