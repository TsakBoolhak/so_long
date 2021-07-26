#ifndef SO_LONG_STRUCTURES_H
# define SO_LONG_STRUCTURES_H

typedef struct s_coord
{
	int	x;
	int	y;
}t_coord;

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bpp;
	int		width;
	int		height;
	int		end;
}t_img;

typedef struct s_character
{
	t_img	front;
	t_img	back;
	t_img	right;
	t_img	left;
	int		dir;
	t_coord	pos;
}t_character;

typedef struct s_screen
{
	t_coord	size;
	t_coord	player;
	t_img	img;
	char	**map;
}t_screen;

typedef struct s_game
{
	int				height;
	int				width;
	unsigned long	moves;
	int				collect_nb;
	int				quit;
	int				quit_time;
	char			**map;
	t_screen		screen;
	t_coord			res;
	t_img			water;
	t_img			wall;
	t_img			grass;
	t_img			exit;
	t_img			collect;
	t_character		player;
	void			*win;
	void			*mlx;
}t_game;

typedef struct s_parse
{
	int	exit;
	int	collectible;
	int	line_error;
	int	char_error;
}t_parse;

#endif
