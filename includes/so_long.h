#ifndef SO_LONG_H
# define SO_LONG_H
# include "so_long_structures.h"

/*
** print_1.c
*/
void	print_usage(void);
void	print_map_rules(void);
void	print_map(char **map, int fd);
void	print_moves(t_game *game);

/*
** print_2.c
*/
void	print_line_error(char *line, int row, int *no_print, char **map);
void	print_char_error(char c, int column, int *flag);
void	print_last_error(t_parse *parse, char **map, int no_spawn);
int		return_error(char *error_msg, int return_value);

/*
** parser_checks.c
*/
int		check_argument(int ac, char **av);
void	check_char(char *line, int i, t_game *game, t_parse *parse);
int		check_line(char *line, t_game *game, t_parse *parse);
void	last_parsing_checks(char *line, t_game *game, t_parse *parse);

/*
** parser.c
*/
void	init_parsing(t_game *game, t_parse *parse);
void	handle_valid_char(char c, int i, t_game *game, t_parse *parse);
int		map_parser(int fd, t_game *game, t_parse *parse);
int		parser(int ac, char **av, t_game *game);

/*
** textures.c
*/
void	fill_texture_img(t_img *pack, t_img *img, int x_start, int y_start);
void	setup_textures_addr(t_game *game, t_img *pack);
int		create_player_textures(t_game *game, t_character *player);
void	fill_player_texture(t_img *pack, t_character *player);
void	setup_player_addr(t_game *game);

/*
** init.c
*/
int		create_screen_map(t_game *game);
int		init_screen(t_game *game);
int		init_textures(t_game *game, char *pack_1);
int		init_player(t_game *game, char *pack_2);
int		init_game(t_game *game);

/*
** refresh.c
*/
void	put_img_to_img(t_img *dst, t_img *src, int x_start, int y_start);
void	handle_large_map(t_game *game, int *screen_x, int *map_x, int *len_x);
void	handle_tall_map(t_game *game, int *screen_y, int *map_y, int *len_y);
void	get_map_pos(t_game *game, t_coord *screen, t_coord *map, t_coord *len);
void	scroll_screen(t_game *game);

/*
** free_datas.c
*/
void	free_player_datas(t_game *game, t_character *player);
void	free_all_datas(t_game *game);

/*
** movement.c
*/
void	move_up(t_game *game);
void	move_down(t_game *game);
void	move_right(t_game *game);
void	move_left(t_game *game);
int		handle_keypress(int key, t_game *game);

/*
** render.c
*/
void	detect_collect_exit(t_game *game, char *game_map_c, char *screen_map_c);
int		render_frame(t_game *game);
void	fill_screen_buffer(t_game *game);

#endif
