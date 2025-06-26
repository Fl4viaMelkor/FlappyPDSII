NAME		= flappy_bird

FILES		= main.cpp player.cpp objeto_cano.cpp util.cpp sprite.cpp\
			controladores/keybind.cpp \
			dados/logger.cpp dados/dados.cpp dados/database.cpp \
			fisica/fisica.cpp fisica/hitbox.cpp  \
			interface/tela_jogo.cpp interface/tela_fimdejogo.cpp \
			interface/camada_background.cpp  interface/parallax_background.cpp  \
			interface/TelaInicial.cpp interface/GerenciadorHighScores.cpp interface/TelaHighScore.cpp \
			interface/TelaCadastro.cpp


SRC_PATH	= src/
OBJ_PATH	= obj/
INC_PATH	= include/

OBJ_DIRS	= obj/

CC			= g++
CFLAGS		= -std=c++11 -Wall -g
LIBS		+= $(shell pkg-config --libs --cflags allegro-5 allegro_main-5 allegro_audio-5 allegro_image-5 allegro_font-5 allegro_primitives-5 allegro_acodec-5 allegro_ttf-5 ) -lsqlite3
CFLAGS		+= -I $(INC_PATH)

RM			= rm -rf

SRC			= $(addprefix $(SRC_PATH), $(FILES))
OBJ			= $(addprefix $(OBJ_PATH), $(FILES:.cpp=.o))

DB_FILE		= jogadores.db

COLOR_W			= \e[00m
COLOR_R			= \e[31m
COLOR_G			= \e[32m

all : $(NAME)

$(OBJ_DIRS):
	mkdir -p $@

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ_DIRS) $(OBJ)
	@printf "$(COLOR_G)Making $(NAME)\n$(COLOR_W)"
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBS)
	@printf "$(COLOR_G)Done\n$(COLOR_W)"

clean :
	@printf "$(COLOR_R)Cleaning $(NAME) objects...\n$(COLOR_W)"
	$(RM) $(OBJ)
	$(RM) $(OBJ_DIRS)
	$(RM) $(DB_FILE)
	@printf "$(COLOR_G)Done\n$(COLOR_W)"

fclean : clean
	@printf "$(COLOR_R)Cleaning $(NAME)\n$(COLOR_W)"
	$(RM) $(NAME)
	@printf "$(COLOR_G)Done\n$(COLOR_W)"

re : fclean all

.PHONY: all clean fclean re