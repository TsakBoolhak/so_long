#include "mlx.h"
#include "libft.h"
#include "so_long_structures_bonus.h"
#include "so_long_bonus.h"

int	create_ennemies_textures(t_game *game, t_ennemies *foes)
{
	foes->front_1.img = mlx_new_image(game->mlx, 32, 32);
	foes->front_2.img = mlx_new_image(game->mlx, 32, 32);
	foes->front_3.img = mlx_new_image(game->mlx, 32, 32);
	foes->back_1.img = mlx_new_image(game->mlx, 32, 32);
	foes->back_2.img = mlx_new_image(game->mlx, 32, 32);
	foes->back_3.img = mlx_new_image(game->mlx, 32, 32);
	foes->right_1.img = mlx_new_image(game->mlx, 32, 32);
	foes->right_2.img = mlx_new_image(game->mlx, 32, 32);
	foes->right_3.img = mlx_new_image(game->mlx, 32, 32);
	foes->left_1.img = mlx_new_image(game->mlx, 32, 32);
	foes->left_2.img = mlx_new_image(game->mlx, 32, 32);
	foes->left_3.img = mlx_new_image(game->mlx, 32, 32);
	if (!foes->front_1.img || !foes->front_2.img || !foes->front_3.img
		|| !foes->back_1.img || !foes->back_2.img || !foes->back_3.img
		|| !foes->right_1.img || !foes->right_2.img || !foes->right_3.img
		|| !foes->left_1.img || !foes->left_2.img || !foes->left_3.img)
		return (-1);
	return (0);
}

void	fill_ennemies_texture(t_img *pack, t_ennemies *ennemies)
{
	fill_texture_img(pack, &ennemies->front_1, 32, 0);
	fill_texture_img(pack, &ennemies->front_2, 0, 0);
	fill_texture_img(pack, &ennemies->front_3, 64, 0);
	fill_texture_img(pack, &ennemies->back_1, 32, 96);
	fill_texture_img(pack, &ennemies->back_2, 0, 96);
	fill_texture_img(pack, &ennemies->back_3, 64, 96);
	fill_texture_img(pack, &ennemies->right_1, 32, 64);
	fill_texture_img(pack, &ennemies->right_2, 0, 64);
	fill_texture_img(pack, &ennemies->right_3, 64, 64);
	fill_texture_img(pack, &ennemies->left_1, 32, 32);
	fill_texture_img(pack, &ennemies->left_2, 0, 32);
	fill_texture_img(pack, &ennemies->left_3, 64, 32);
}

void	setup_ennemies_addr(t_game *game, t_img *tmp)
{
	t_img	*img;

	tmp->addr = mlx_get_data_addr(tmp->img, &tmp->bpp, &tmp->width, &tmp->end);
	img = &game->ennemies.front_2;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->ennemies.front_3;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->ennemies.back_1;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->ennemies.back_2;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->ennemies.back_3;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->ennemies.right_1;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->ennemies.right_2;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->ennemies.right_3;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->ennemies.left_1;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->ennemies.left_2;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	img = &game->ennemies.left_3;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
}

int	init_ennemies(t_game *game, char *name)
{
	t_img		pack;
	t_ennemies	*ennemies;
	void		*mlx;

	mlx = game->mlx;
	ennemies = &game->ennemies;
	pack.img = mlx_xpm_file_to_image(mlx, name, &pack.width, &pack.height);
	if (!pack.img)
		return (return_error("Couldn't load the texture pack", -1));
	if (create_ennemies_textures(game, ennemies))
	{
		mlx_destroy_image(game->mlx, pack.img);
		return (return_error("Couldn't allocate enough memory", -1));
	}
	pack.addr = mlx_get_data_addr(pack.img, &pack.bpp, &pack.width, &pack.end);
	setup_ennemies_addr(game, &ennemies->front_1);
	fill_ennemies_texture(&pack, ennemies);
	mlx_destroy_image(game->mlx, pack.img);
	return (0);
}
