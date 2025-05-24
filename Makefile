# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/06 16:16:34 by mgrandia          #+#    #+#              #
#    Updated: 2025/05/24 16:08:36 by mgrandia         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ─────────────────────────────────────────────────────────────
# CONFIGURACIÓN GENERAL
# ─────────────────────────────────────────────────────────────
NAME = pipex
CC = cc
AR = ar rcs
RM = rm -f
FLAGS = -Wall -Wextra -Werror -g 

# ─────────────────────────────────────────────────────────────
# COLORES
# ─────────────────────────────────────────────────────────────
GREEN = \033[38;5;114m   
SLATE = \033[38;5;145m 
TEAL = \033[38;5;123m 
RED = \033[0;31m
RESET = \033[0m

# ─────────────────────────────────────────────────────────────
# DIRECTORIOS
# ─────────────────────────────────────────────────────────────
DIR_INC = includes/ 
DIR_OBJS = objs/
DIR_SRC = srcs/

# ─────────────────────────────────────────────────────────────
# ARCHIVOS
# ─────────────────────────────────────────────────────────────
SRC = srcs/pipex.c srcs/pipex_utils.c srcs/path.c
OBJS = $(addprefix $(DIR_OBJS), $(notdir $(SRC:%.c=%.o)))
DEP_FILES = $(OBJS:%.o=%.d)
LIBFT = ./libft/libft.a

# ─────────────────────────────────────────────────────────────
# REGLAS
# ─────────────────────────────────────────────────────────────
all: $(LIBFT) $(NAME)

$(LIBFT):
	make -sC ./libft

$(NAME): $(DIR_OBJS) $(OBJS) $(LIBFT)
	@ $(CC) $(FLAGS) $(OBJS) -o $(NAME) $(LIBFT)	
	@echo
	@echo "$(GREEN)$(BOLD)✅ Library $(NAME) successfully created!$(RESET)"
	@echo "$(TEAL) ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(RESET)"
	@echo "$(SLATE)  ➤ pipex files compiled: $(words $(SRC))$(RESET)"
	@echo "$(TEAL) ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(RESET)"
	@echo

$(DIR_OBJS):
	@echo "$(SLATE) 🛠️  Creating objs directory..."
	@mkdir -p $(DIR_OBJS)

# Regla para compilar archivos .o desde archivos .c
$(DIR_OBJS)%.o: $(DIR_SRC)%.c Makefile includes/pipex.h
	@echo "$(SLATE) ⚙️  Compiling $(@F:%.o=%.c)..."
	@$(CC) $(FLAGS) -MD -I$(DIR_INC) -c $< -o $@

clean:
	@echo "$(SLATE) 🧹 Removing object files at pipex..."
	@$(RM) -r $(DIR_OBJS)
	@make -sC ./libft clean
	@echo "$(SLATE) ✅ Object files removed!"

fclean: clean
	@echo "$(SLATE) 🗑️  Removing library pipex"
	@$(RM) $(NAME)
	@make -sC ./libft fclean
	@echo "$(SLATE) ✅ fclean complete!"

re: fclean all
	@echo "$(SLATE) 🎉 Full recompilation complete!"

.PHONY: all clean fclean re

-include $(DEP_FILES)
