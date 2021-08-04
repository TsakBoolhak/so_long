#include "mlx.h"
#include "libft.h"
#include "so_long_structures_bonus.h"
#include "so_long_bonus.h"

void	set_font_pos(t_coord *pos, int index, int row)
{
	pos->x = 40 * index + 4;
	pos->y = 40 * row + 4;
}

void	get_font_pos(char c, t_coord *pos)
{
	int	i;

	if (c == ' ')
		return ;
	i = ft_strchr_index(" !\"#$%&'()*+,-.", c);
	if (i > -1)
		set_font_pos(pos, i, 0);
	i = ft_strchr_index("/0123456789:;<=", c);
	if (i > -1)
		set_font_pos(pos, i, 1);
	i = ft_strchr_index(">?@ABCDEFGHIJKL", c);
	if (i > -1)
		set_font_pos(pos, i, 2);
	i = ft_strchr_index("NMOPQRSTUVWXYZ|", c);
	if (i > -1)
		set_font_pos(pos, i, 3);
	i = ft_strchr_index("\\]^_`abcdefghij", c);
	if (i > -1)
		set_font_pos(pos, i, 4);
	i = ft_strchr_index("klmnopqrstuvwxy", c);
	if (i > -1)
		set_font_pos(pos, i, 5);
	else if (c == 'z')
		set_font_pos(pos, 0, 6);
}

void	putstr_to_img(t_game *game, char *str, int x, int y)
{
	int		i;
	t_coord	pos;

	i = 0;
	while (str[i] && 32 * i < game->res.x - x)
	{
		ft_memset(&pos, -1, sizeof(t_coord));
		ft_memset(game->letter.addr, 0, 32 * 32 * 4);
		get_font_pos(str[i], &pos);
		if (pos.x >= 0)
		{
			fill_texture_img(&game->font, &game->letter, pos.x, pos.y);
			put_img_to_img(&game->screen.img, &game->letter, x + 32 * i, y);
		}
		i++;
	}
}

int	init_font(t_game *game, char *name)
{
	t_img	*img;
	void	*mlx;

	mlx = game->mlx;
	img = &game->font;
	img->img = mlx_xpm_file_to_image(mlx, name, &img->width, &img->height);
	if (!img->img)
		return (return_error("Couldn't load the texture pack", -1));
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	game->letter.img = mlx_new_image(game->mlx, 32, 32);
	if (!game->letter.img)
		return (return_error("Couldn't allocate enough memory", -1));
	img = &game->letter;
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->width, &img->end);
	return (0);
}
