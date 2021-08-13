SRCFOLD		= srcs/

BONUSFOLD	= srcs_bonus/

SRCS_BONUS	=	${BONUSFOLD}main_bonus.c \
				${BONUSFOLD}print_1_bonus.c \
				${BONUSFOLD}print_2_bonus.c \
				${BONUSFOLD}parser_checker_bonus.c \
				${BONUSFOLD}parser_bonus.c \
				${BONUSFOLD}textures_bonus.c \
				${BONUSFOLD}init_bonus.c \
				${BONUSFOLD}refresh_bonus.c \
				${BONUSFOLD}free_datas_bonus.c \
				${BONUSFOLD}movements_bonus.c \
				${BONUSFOLD}font_bonus.c \
				${BONUSFOLD}init_ennemies_bonus.c \
				${BONUSFOLD}draw_ennemies_bonus.c \
				${BONUSFOLD}draw_player_bonus.c \
				${BONUSFOLD}movements_ennemies_bonus.c \
				${BONUSFOLD}draw_bonus.c \
				${BONUSFOLD}fill_screen_bonus.c \
				${BONUSFOLD}render_bonus.c

SRCS		=	${SRCFOLD}main.c \
				${SRCFOLD}print_1.c \
				${SRCFOLD}print_2.c \
				${SRCFOLD}parser_checks.c \
				${SRCFOLD}parser.c \
				${SRCFOLD}textures.c \
				${SRCFOLD}init.c \
				${SRCFOLD}refresh.c \
				${SRCFOLD}free_datas.c \
				${SRCFOLD}movements.c \
				${SRCFOLD}render.c

OBJS		= ${SRCS:.c=.o}

OBJS_BONUS	= ${SRCS_BONUS:.c=.o}

NAME		= so_long

NAME_BONUS	= so_long_bonus

CC			= @clang

CFLAGS		= -Wall -Wextra -Werror -g3

RM			= @rm -f

%.o: %.c
	${CC} ${CFLAGS} -I/usr/include -Imlx_linux -Ilibft -Iincludes -c $< -o ${<:.c=.o}

${NAME}:		 ${OBJS}
	@echo "Compiling ressources needed to create the executable.It should take few seconds.."
	@make --no-print-directory -C mlx_linux
	@make --no-print-directory -C libft
	@${CC} ${CFLAGS} ${OBJS} -Lmlx_linux -lmlx_Linux -L/usr/lib -Llibft -Imlx_linux -lft -lXext -lX11 -lm -o ${NAME}
	@echo "Game is ready to be launched\n\nTo launch, type \"so_long file.ber\" where file.ber is a configuration file containing the map"

${NAME_BONUS}:		 ${OBJS_BONUS}
	@echo "Compiling ressources needed to create the executable.It should take few seconds.."
	@make --no-print-directory -C mlx_linux
	@make --no-print-directory -C libft
	@${CC} ${CFLAGS} ${OBJS_BONUS} -Lmlx_linux -lmlx_Linux -L/usr/lib -Llibft -Imlx_linux -lft -lXext -lX11 -lm -o ${NAME_BONUS}
	@echo "Game is ready to be launched\n\nTo launch, type \"so_long_bonus file.ber\" where file.ber is a configuration file containing the map"

all:			${NAME}

bonus:			${NAME_BONUS}

clean:
	@echo "Cleaning files"
	${RM} ${OBJS}
	@make clean --no-print-directory -C libft/

clean_bonus:
	@echo "Cleaning files"
	${RM} ${OBJS_BONUS}
	@make clean --no-print-directory -C libft/

fclean:			clean
	${RM} ${NAME}
	@make clean --no-print-directory  -C mlx_linux/
	@make fclean --no-print-directory -C libft/

fclean_bonus:			clean_bonus
	${RM} ${NAME_BONUS}
	@make clean --no-print-directory  -C mlx_linux/
	@make fclean --no-print-directory -C libft/

re:				fclean all

re_bonus:		fclean_bonus bonus

.PHONY:			all clean fclean re bonus clean_bonus fclean_bonus re_bonus

