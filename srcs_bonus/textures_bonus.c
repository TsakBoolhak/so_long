#include "mlx.h"
#include "libft.h"
#include "so_long_structures_bonus.h"

void	fill_texture_img(t_img *pack, t_img *img, int x_start, int y_start)
{
	int		x;
	int		y;
	int		i;
	char	*addr;

	i = 0;
	y = y_start;
	while (y < y_start + 32)
	{
		x = x_start;
		while (x < x_start + 32)
		{
			addr = pack->addr + (y * pack->width + x * (pack->bpp / 8));
			if ((int)(*addr))
				ft_memcpy(img->addr + i, addr, 4);
			i += 4;
			x++;
		}
		y++;
	}
}

void	setup_textures_addr(t_game *game, t_img *pack)
{
	t_img	*img;

	img = &game->wall;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->grass;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->exit;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->collect;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->water;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	fill_texture_img(pack, &game->wall, 64, 64);
	fill_texture_img(pack, &game->water, 32, 64);
	fill_texture_img(pack, &game->grass, 0, 64);
	fill_texture_img(pack, &game->exit, 64, 0);
	fill_texture_img(pack, &game->collect, 128, 0);
}

int	create_player_textures(t_game *game, t_character *player)
{
	player->front_1.img = mlx_new_image(game->mlx, 32, 32);
	player->front_2.img = mlx_new_image(game->mlx, 32, 32);
	player->front_3.img = mlx_new_image(game->mlx, 32, 32);
	player->back_1.img = mlx_new_image(game->mlx, 32, 32);
	player->back_2.img = mlx_new_image(game->mlx, 32, 32);
	player->back_3.img = mlx_new_image(game->mlx, 32, 32);
	player->right_1.img = mlx_new_image(game->mlx, 32, 32);
	player->right_2.img = mlx_new_image(game->mlx, 32, 32);
	player->right_3.img = mlx_new_image(game->mlx, 32, 32);
	player->left_1.img = mlx_new_image(game->mlx, 32, 32);
	player->left_2.img = mlx_new_image(game->mlx, 32, 32);
	player->left_3.img = mlx_new_image(game->mlx, 32, 32);
	if (!player->front_1.img || !player->front_2.img || !player->front_3.img
		|| !player->back_1.img || !player->back_2.img || !player->back_3.img
		|| !player->right_1.img || !player->right_2.img || !player->right_3.img
		|| !player->left_1.img || !player->left_2.img || !player->left_3.img)
		return (-1);
	return (0);
}

void	fill_player_texture(t_img *pack, t_character *player)
{
	fill_texture_img(pack, &player->front_1, 32, 0);
	fill_texture_img(pack, &player->front_2, 0, 0);
	fill_texture_img(pack, &player->front_3, 64, 0);
	fill_texture_img(pack, &player->back_1, 32, 96);
	fill_texture_img(pack, &player->back_2, 0, 96);
	fill_texture_img(pack, &player->back_3, 64, 96);
	fill_texture_img(pack, &player->right_1, 32, 64);
	fill_texture_img(pack, &player->right_2, 0, 64);
	fill_texture_img(pack, &player->right_3, 64, 64);
	fill_texture_img(pack, &player->left_1, 32, 32);
	fill_texture_img(pack, &player->left_2, 0, 32);
	fill_texture_img(pack, &player->left_3, 64, 32);
}

void	setup_player_addr(t_game *game, t_img *tmp)
{
	t_img	*img;

	tmp->addr = mlx_get_data_addr(tmp->img, &tmp->bpp, &tmp->width, &tmp->end);
	img = &game->player.front_2;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->player.front_3;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->player.back_1;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->player.back_2;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->player.back_3;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->player.right_1;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->player.right_2;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->player.right_3;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->player.left_1;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->player.left_2;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->player.left_3;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
}
