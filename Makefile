# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/06 16:16:34 by mgrandia          #+#    #+#              #
#    Updated: 2025/05/24 17:08:21 by mgrandia         ###   ########.fr        #
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

.PHONY: all clean fclean re libft

# ALL: primero construir libft (siempre), luego pipex
all: libft $(NAME)

# Regla phony para libft: siempre ejecuta make dentro de libft
libft:
	@make -C ./libft

# Compilar pipex, que depende de objetos y libft.a
$(NAME): $(DIR_OBJS) $(OBJS) $(LIBFT)
	@$(CC) $(FLAGS) $(OBJS) -o $(NAME) $(LIBFT)
	@echo
	@echo "$(GREEN)✅ Ejecutable $(NAME) creado correctamente!$(RESET)"
	@echo

# Crear carpeta objs si no existe
$(DIR_OBJS):
	@echo "$(SLATE)🛠️  Creando carpeta de objetos...$(RESET)"
	@mkdir -p $(DIR_OBJS)

# Compilar los archivos .c en .o con generación de dependencias .d
$(DIR_OBJS)%.o: $(DIR_SRC)%.c Makefile includes/pipex.h | $(DIR_OBJS)
	@echo "$(SLATE)⚙️  Compilando $< ...$(RESET)"
	@$(CC) $(FLAGS) -MD -I$(DIR_INC) -c $< -o $@

# Limpiar objetos y limpiar libft también
clean:
	@echo "$(SLATE)🧹 Limpiando objetos de pipex y libft...$(RESET)"
	@$(RM) -r $(DIR_OBJS)
	@make clean -C ./libft
	@echo "$(SLATE)✅ Objetos limpiados.$(RESET)"

# Limpiar todo incluyendo ejecutable y librería
fclean: clean
	@echo "$(SLATE)🗑️  Limpiando ejecutable y librería libft...$(RESET)"
	@$(RM) $(NAME)
	@make fclean -C ./libft
	@echo "$(SLATE)✅ Limpieza total completada.$(RESET)"

# Recompilación completa
re: fclean all
	@echo "$(SLATE)🎉 Recompilación completa!$(RESET)"

# Incluir dependencias automáticas
-include $(DEP_FILES)

