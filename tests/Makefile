.PHONY : all clean fclean re libft norminette valgrind supp_file

NAME = tester

# ╭━━━━━━━━━━━━══════════╕出 ❖ BASICS VARIABLES ❖ 力╒═══════════━━━━━━━━━━━━╮ #

CC				:=	cc

CFLAGS			:= -Wall -Wextra -Werror -MMD

RM				:=	rm	-rf

SHOW_MSG_CLEAN	=	true

MAKEFLAGS		+=	--no-print-directory

# ╰━━━━━━━━━━━━━━━━════════════════╛出 ❖ 力╘════════════════━━━━━━━━━━━━━━━━╯ #

# ╭━━━━━━━━━━━━══════════╕出 ❖ FILE TREE ❖ 力╒═══════════━━━━━━━━━━━━╮ #

# directories
D_SRC	=	../src/
D_INC	=	../inc/
D_OBJ	=	.obj/
D_DEP	=	.dep/
D_LFT	=	../libft/

D_EXE	=	$(D_SRC)exec/
D_UTL	=	$(D_SRC)utils/
D_TOK	=	$(D_SRC)token/
D_PAR	=	$(D_SRC)parsing/
D_BLT	=	$(D_SRC)builtins/
D_SIG	=	$(D_SRC)signal_handlers/

D_SRCS	= $(D_SRC) $(D_EXE) $(D_TOK) $(D_BLT) $(D_PAR) $(D_SIG) $(D_UTL)

# file lists
LST_SRC		=	token_ast.c

LST_SIG		=	sig_setup.c			\
				sig_handlers.c

LST_TOK		=	add_token.c			\
				expand_arg.c		\
				utils_token.c		\
				parenthesis.c		\
				tokenisation.c

LST_PAR		=	redir.c				\
				parser.c			\
				ast_builders.c

LST_EXE		=	env.c				\
				exec.c				\
				exec_err.c			\
				cmd_utils.c			\
				builtin_exec.c		\
				exec_behaviors.c

LST_BLT		=	ft_cd.c				\
				ft_env.c			\
				ft_pwd.c			\
				ft_exit.c			\
				ft_echo.c			\
				ft_unset.c			\
				ft_export.c			\
				double_dot.c		\
				ft_export_print.c	\
				ft_export_utils.c

LST_UTL		=	str_handle.c		\
				secure_exit.c		\
				secure_alloc.c		\
				fd_collector.c		\
				allocs_collector.c

LST_INC		=	lexing.h			\
				parsing.h			\
				sigaction.h			\
				minishell.h

LST_SRCS	=	$(LST_SRC) $(LST_EXE) $(LST_TOK) $(LST_BLT) $(LST_PAR) $(LST_SIG) $(LST_UTL)

INC			=	$(addprefix $(D_INC), $(LST_INC))

OBJ			=	$(addprefix $(D_OBJ), $(notdir $(LST_SRCS:.c=.o)))

DEPS		=	$(addprefix $(D_DEPS), $(notdir $(LST_SRCS:.c=.d)))

LIBS		:=	-L$(D_LFT) -lft -lreadline -lncurses

INCS		:=	-I$(D_INC) -I$(D_LFT)

SUPP_FILE	:=	readline.supp

# ╭━━━━━━━━━━━━══════════╕出 ❖ RULES ❖ 力╒═══════════━━━━━━━━━━━━╮ #

all:	$(NAME)

$(NAME):	libft $(OBJ) $(INC) | $(D_OBJ) $(D_DEP) Makefile
	@$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME)
	@clear
	@echo "\e[0;32m$(NAME) program created successfully ! 🧬\e[0m"

debug:	libft $(OBJ) $(INC) | $(D_OBJ) $(D_DEP) Makefile
	@$(CC) $(CFLAGS) -g3 $(OBJ) $(LIBS) -o $(NAME)
	@echo "\e[0;32m$(NAME) program created successfully ! 🧬\e[0m"

$(D_OBJ):
	@mkdir -p $@

$(D_DEP):
	@mkdir -p $(D_DEP)

vpath %.c $(D_SRCS)

$(D_OBJ)%.o: %.c | $(D_OBJ) $(D_DEP)
# @echo "Compiling $< → $@"
	@$(CC) $(CFLAGS) $(INCS) -c $< -o $@
	@mv $(@:.o=.d) $(D_DEP)

-include $(DEPS)

libft:	$(D_LFT)
	$(MAKE) -C $(D_LFT)

clean:
ifeq ($(SHOW_MSG_CLEAN), true)
	@echo "\e[0;36mAll $(NAME) objects have been removed 🧹\e[0m"
endif
	@$(MAKE) -s -C $(D_LFT) clean
	@$(RM) $(D_OBJ) $(D_DEP) $(D_DOC)

fclean:
	@$(MAKE) -s SHOW_MSG_CLEAN=false clean
	@$(MAKE) -s -C $(D_LFT) fclean
	@$(RM) $(NAME)
	@clear
	@echo "\e[0;34m$(NAME) executable deleted ! 🧼\e[0m"

re:
	@$(MAKE) fclean
	@$(MAKE) all
	@echo "\e[0;32m$(NAME) program recreated successfully ! 🫡\e[0m"

# documentation:
# 	doxygen Doxyfile

# html:
# 	$(MAKE) documentation
# 	xdg-open docs/html/index.html 

norminette:
	norminette $(D_SRC) $(D_INC)

supp_file: | $(D_OBJ)
	@echo "Generating $(SUPP_FILE)..."
	@printf '{\n	ignore_libreadline_leaks\n	Memcheck:Leak\n	...\n	obj:*/libreadline.so*\n}\n' > $(D_OBJ)$(SUPP_FILE)
	@echo "$(SUPP_FILE) successfully created !"

valgrind: supp_file
	@$(MAKE) debug
	valgrind							\
		--leak-check=full					\
		--show-leak-kinds=all				\
		--track-origins=yes 				\
		--track-fds=yes						\
		--suppressions=$(D_OBJ)$(SUPP_FILE)	\
		./$(NAME)