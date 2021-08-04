#ifndef SO_LONG_BONUS_H
# define SO_LONG_BONUS_H
# include "so_long_structures_bonus.h"

/*
** print_1_bonus.c
*/
void	print_usage(void);
void	print_map_rules(void);
void	print_map(char **map, int fd);
void	print_moves(t_game *game);

/*
** print_2_bonus.c
*/
void	print_line_error(char *line, int row, int *no_print, char **map);
void	print_char_error(char c, int column, int *flag);
void	print_last_error(t_parse *parse, char **map, int no_spawn);
int		return_error(char *error_msg, int return_value);

/*
** parser_checker_bonus.c
*/
int		check_argument(int ac, char **av);
int		check_char(char *line, int i, t_game *game, t_parse *parse);
int		check_line(char *line, t_game *game, t_parse *parse);
void	last_parsing_checks(char *line, t_game *game, t_parse *parse);

/*
** parser_bonus.c
*/
int		store_ennemies(t_game *game, int x, int y);
int		handle_valid_char(char c, int i, t_game *game, t_parse *parse);
int		map_parser(int fd, t_game *game, t_parse *parse);
void	init_parsing(t_game *game, t_parse *parse);
int		parser(int ac, char **av, t_game *game);

/*
** textures_bonus.c
*/
void	fill_texture_img(t_img *pack, t_img *img, int x_start, int y_start);
void	setup_textures_addr(t_game *game, t_img *pack);
int		create_player_textures(t_game *game, t_character *player);
void	fill_player_texture(t_img *pack, t_character *player);
void	setup_player_addr(t_game *game, t_img *tmp);

/*
** init_bonus.c
*/
int		create_screen_map(t_game *game);
int		init_screen(t_game *game);
int		init_textures(t_game *game, char *pack_1);
int		init_player(t_game *game, char *pack_2);
int		init_game(t_game *game);

/*
** refresh_bonus.c
*/
void	handle_large_map(t_game *game, int *screen_x, int *map_x, int *len_x);
void	handle_tall_map(t_game *game, int *screen_y, int *map_y, int *len_y);
void	get_map_pos(t_game *game, t_coord *screen, t_coord *map, t_coord *len);
void	update_data(t_game *game, t_coord map, t_coord screen);
void	scroll_screen(t_game *game);

/*
** free_datas_bonus.c
*/
void	free_player_datas(t_game *game, t_character *player);
void	free_ennemies_datas(t_game *game, t_ennemies *ennemies);
void	free_player_ennemies_map(t_game *game);
void	free_all_datas(t_game *game);

/*
** movements_bonus.c
*/
void	move_up(t_game *game);
void	move_down(t_game *game);
void	move_right(t_game *game);
void	move_left(t_game *game);
int		handle_keypress(int key, t_game *game);

/*
** font_bonus.c
*/
void	set_font_pos(t_coord *pos, int index, int row);
void	get_font_pos(char c, t_coord *pos);
void	putstr_to_img(t_game *game, char *str, int x, int y);
int		init_font(t_game *game, char *name);

/*
** init_ennemies_bonus.c
*/
int		create_ennemies_textures(t_game *game, t_ennemies *foes);
void	fill_ennemies_texture(t_img *pack, t_ennemies *ennemies);
void	setup_ennemies_addr(t_game *game, t_img *tmp);
int		init_ennemies(t_game *game, char *name);

/*
** draw_ennemies_bonus.c
*/
t_img	*foes_top_left(t_game *game, int frame, t_coord *pos, t_infos *infos);
t_img	*foes_bot_right(t_game *game, int frame, t_coord *pos, t_infos *infos);
void	draw_ennemies_loop(t_game *game, t_infos *content, int frame_chunk);
void	draw_ennemies(t_game *game);

/*
** draw_player_bonus.c
*/
t_img	*player_top_left(t_game *game, int frame, t_coord *increm, int mul);
t_img	*player_down_right(t_game *game, int frame, t_coord *increm, int mul);
void	draw_player(t_game *game);

/*
** movements_ennemies_bonus.c
*/
t_coord	get_next_pos(t_game *game, t_infos *infos);
int		check_other_moves(t_game *game, t_coord step, t_list *lst);
int		crossing_player(t_game *game, t_infos *infos);
int		check_next_step(t_game *game, t_infos *info, t_coord step, t_list *lst);
void	move_ennemies(t_game *game);

/*
** draw_bonus.c
*/
void	draw_move_count(t_game *game);
void	draw_msg(t_game *game);

/*
** fill_screen_bonus.c
*/
void	set_pos(int *dst_pos, int *src_pos, int start);
void	put_img_to_img(t_img *dst, t_img *src, int x_start, int y_start);
void	fill_screen_buffer(t_game *game);

/*
** render_bonus.c
*/
void	handle_reach_exit(t_game *game);
void	detect_collect_exit(t_game *game, char *game_map_c, char *screen_map_c);
void	check_last_anim_frame(t_game *game);
int		render_frame(t_game *game);

#endif
