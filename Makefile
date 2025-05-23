NAME		= flappy_bird

FILES		= main.cpp player.cpp

SRC_PATH	= src/
OBJ_PATH	= obj/
INC_PATH	= include/

OBJ_DIRS	= obj/

CC			= g++
CFLAGS		= -Wall -Wextra -Werror
CFLAGS		+= -I $(INC_PATH)

RM			= rm -rf

SRC			= $(addprefix $(SRC_PATH), $(FILES))
OBJ			= $(addprefix $(OBJ_PATH), $(FILES:.cpp=.o))

COLOR_W			= \e[00m
COLOR_R			= \e[31m
COLOR_G			= \e[32m

all : $(NAME)

$(OBJ_DIRS):
	mkdir -p $@

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ_DIRS) $(OBJ)
	@printf "$(COLOR_G)Making $(NAME)\n$(COLOR_W)"
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)
	@printf "$(COLOR_G)Done\n$(COLOR_W)"

clean :
	@printf "$(COLOR_R)Cleaning $(NAME) objects...\n$(COLOR_W)"
	$(RM) $(OBJ)
	$(RM) $(OBJ_DIRS)
	@printf "$(COLOR_G)Done\n$(COLOR_W)"

fclean : clean
	@printf "$(COLOR_R)Cleaning $(NAME)\n$(COLOR_W)"
	$(RM) $(NAME)
	@printf "$(COLOR_G)Done\n$(COLOR_W)"

re : fclean all

.PHONY: all clean fclean re