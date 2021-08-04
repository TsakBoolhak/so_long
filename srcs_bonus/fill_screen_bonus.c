#include "libft.h"
#include "so_long_structures_bonus.h"
#include "so_long_bonus.h"

void	set_pos(int *dst_pos, int *src_pos, int start)
{
	*dst_pos = start;
	*src_pos = 0;
	if (start < 0)
	{
		*dst_pos = 0;
		*src_pos = -start;
	}
}

void	put_img_to_img(t_img *dst, t_img *src, int x_start, int y_start)
{
	t_coord	dst_pos;
	t_coord	src_pos;
	char	*addr_dst;
	char	*addr_src;
	int		color;

	set_pos(&dst_pos.y, &src_pos.y, y_start);
	while (dst_pos.y < y_start + 32 && dst_pos.y < dst->height)
	{
		set_pos(&dst_pos.x, &src_pos.x, x_start);
		while (dst_pos.x < x_start + 32 && dst_pos.x * 4 < dst->width)
		{
			addr_dst = dst->addr + (dst_pos.y * dst->width + dst_pos.x * 4);
			addr_src = src->addr + (src_pos.y * src->width + src_pos.x * 4);
			color = (int)(*addr_src);
			if (color && color != (int)(*addr_dst))
				ft_memcpy(addr_dst, addr_src, 4);
			dst_pos.x++;
			src_pos.x++;
		}
		dst_pos.y++;
		src_pos.y++;
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
