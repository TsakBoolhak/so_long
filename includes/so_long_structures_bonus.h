#ifndef SO_LONG_STRUCTURES_BONUS_H
# define SO_LONG_STRUCTURES_BONUS_H

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

typedef struct s_infos
{
	int		dir;
	int		step;
	t_coord	map_pos;
	t_coord	screen_pos;
}t_infos;

typedef struct s_ennemies
{
	t_img	front_1;
	t_img	front_2;
	t_img	front_3;
	t_img	back_1;
	t_img	back_2;
	t_img	back_3;
	t_img	right_1;
	t_img	right_2;
	t_img	right_3;
	t_img	left_1;
	t_img	left_2;
	t_img	left_3;
	t_list	*infos;
}t_ennemies;

typedef struct s_character
{
	t_img	front_1;
	t_img	front_2;
	t_img	front_3;
	t_img	back_1;
	t_img	back_2;
	t_img	back_3;
	t_img	right_1;
	t_img	right_2;
	t_img	right_3;
	t_img	left_1;
	t_img	left_2;
	t_img	left_3;
	int		state;
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
	int			height;
	int			width;
	int			moves;
	int			collect_nb;
	int			quit;
	int			quit_time;
	int			sentence;
	char		**map;
	t_screen	screen;
	t_coord		res;
	t_img		water;
	t_img		wall;
	t_img		grass;
	t_img		exit;
	t_img		collect;
	t_img		font;
	t_img		letter;
	t_character	player;
	t_ennemies	ennemies;
	void		*win;
	void		*mlx;
}t_game;

typedef struct s_parse
{
	int	exit;
	int	collectible;
	int	line_error;
	int	char_error;
}t_parse;

#endif
