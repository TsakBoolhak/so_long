#include "mlx.h"
#include "libft.h"
#include "so_long_structures.h"
#include "so_long.h"

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
	player->front.img = mlx_new_image(game->mlx, 32, 32);
	player->back.img = mlx_new_image(game->mlx, 32, 32);
	player->right.img = mlx_new_image(game->mlx, 32, 32);
	player->left.img = mlx_new_image(game->mlx, 32, 32);
	if (!player->front.img || !player->back.img || !player->right.img
		|| !player->left.img)
		return (-1);
	return (0);
}

void	fill_player_texture(t_img *pack, t_character *player)
{
	fill_texture_img(pack, &player->front, 32, 0);
	fill_texture_img(pack, &player->back, 32, 96);
	fill_texture_img(pack, &player->right, 32, 64);
	fill_texture_img(pack, &player->left, 32, 32);
}

void	setup_player_addr(t_game *game)
{
	t_img	*img;

	img = &game->player.front;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->player.back;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->player.right;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->player.left;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
}
