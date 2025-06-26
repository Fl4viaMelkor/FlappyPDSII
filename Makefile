NAME          = flappy_bird

#----------------------------------------------------------------#
#               ARQUIVOS DE CÓDIGO FONTE DO JOGO                 #
#----------------------------------------------------------------#
FILES         = main.cpp player.cpp objeto_cano.cpp util.cpp sprite.cpp\
                controladores/keybind.cpp \
                dados/logger.cpp dados/dados.cpp dados/database.cpp \
                fisica/fisica.cpp fisica/hitbox.cpp  \
                interface/tela_jogo.cpp interface/tela_fimdejogo.cpp \
                interface/camada_background.cpp  interface/parallax_background.cpp  \
                interface/TelaInicial.cpp interface/TelaHighScore.cpp \
                interface/TelaCadastro.cpp

#----------------------------------------------------------------#
#               CONFIGURAÇÕES DE COMPILAÇÃO                      #
#----------------------------------------------------------------#
SRC_PATH      = src/
OBJ_PATH      = obj/
INC_PATH      = include/

CC            = g++
CFLAGS        = -std=c++11 -Wall -g -I$(INC_PATH)
LIBS          = $(shell pkg-config --libs --cflags allegro-5 allegro_main-5 allegro_audio-5 allegro_image-5 allegro_font-5 allegro_primitives-5 allegro_acodec-5 allegro_ttf-5 ) -lsqlite3
RM            = rm -rf

#----------------------------------------------------------------#
#          VARIÁVEIS AUTOMÁTICAS PARA O CÓDIGO DO JOGO           #
#----------------------------------------------------------------#
OBJ_DIRS      = $(patsubst %,$(OBJ_PATH)%,$(sort $(dir $(FILES))))
SRC           = $(addprefix $(SRC_PATH), $(FILES))
OBJ           = $(addprefix $(OBJ_PATH), $(FILES:.cpp=.o))
DB_FILE       = jogadores.db

# Cores para o terminal
COLOR_W       = \e[00m
COLOR_R       = \e[31m
COLOR_G       = \e[32m
COLOR_B       = \e[34m

#----------------------------------------------------------------#
#                     REGRAS DE COMPILAÇÃO DO JOGO               #
#----------------------------------------------------------------#
all: $(NAME)


$(OBJ_DIRS):
	mkdir -p $@

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp | $(OBJ_DIRS)
	@mkdir -p $(dir $@)
	@printf "Compilando: $<\n"
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	@printf "$(COLOR_G)Making $(NAME)\n$(COLOR_W)"
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBS)
	@printf "$(COLOR_G)Done\n$(COLOR_W)"

#----------------------------------------------------------------#
#                REGRAS PARA TESTES UNITÁRIOS                    #
#----------------------------------------------------------------#
TEST_RUNNER   = test_runner
TEST_SRC_PATH = tests/
TEST_OBJ_PATH = obj/tests/

# Encontra todos os arquivos de teste e converte para caminhos de objeto
TEST_FILES    = $(wildcard $(TEST_SRC_PATH)*.cpp)
TEST_OBJS     = $(TEST_FILES:$(TEST_SRC_PATH)%.cpp=$(TEST_OBJ_PATH)%.o)

# Regra principal para executar os testes
test: $(TEST_RUNNER)
	@printf "$(COLOR_B)Running tests...\n$(COLOR_W)"
	./$(TEST_RUNNER)

# Regra para criar o executável de teste ('test_runner')
# Depende de todos os objetos do jogo (exceto main.o) e de todos os objetos de teste.
$(TEST_RUNNER): $(filter-out $(OBJ_PATH)main.o, $(OBJ)) $(TEST_OBJS)
	@printf "$(COLOR_G)Making $(TEST_RUNNER)\n$(COLOR_W)"
	$(CC) $(CFLAGS) -o $(TEST_RUNNER) $^ $(LIBS)
	@printf "$(COLOR_G)Done\n$(COLOR_W)"

# Regra genérica para compilar qualquer arquivo .cpp da pasta de testes
$(TEST_OBJ_PATH)%.o: $(TEST_SRC_PATH)%.cpp | $(OBJ_DIRS)
	@mkdir -p $(dir $@)
	@printf "Compilando teste: $<\n"
	$(CC) $(CFLAGS) -c $< -o $@

#----------------------------------------------------------------#
#                       REGRAS DE LIMPEZA                        #
#----------------------------------------------------------------#
clean:
	@printf "$(COLOR_R)Cleaning $(NAME) objects...\n$(COLOR_W)"
	$(RM) $(OBJ_PATH)
	$(RM) $(DB_FILE)
	@printf "$(COLOR_G)Done\n$(COLOR_W)"

fclean: clean
	@printf "$(COLOR_R)Cleaning binaries...\n$(COLOR_W)"
	$(RM) $(NAME)
	$(RM) $(TEST_RUNNER)
	@printf "$(COLOR_G)Done\n$(COLOR_W)"

re: fclean all

# Declara os alvos que não geram arquivos
.PHONY: all clean fclean re test