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

#endif
