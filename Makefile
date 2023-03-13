

CC = c++

CFLAGS = -Wall -Werror -Wextra -std=c++98 #-g -fsanitize=address,undefined,integer

CFILES = main.cpp\
		client.cpp\
		miniserver.cpp\
		server.cpp\


OFILES = $(addprefix $(OBJ_DIR)/,$(CFILES:.cpp=.o))

OBJ_DIR = ./OBJ

SRC_DIR = .

INC = *.hpp

NAME = webserv

$(NAME) : $(OBJ_DIR) $(OFILES)
	@$(CC) $(CFLAGS) $(OFILES) -o $(NAME)
	@echo "done for webserv"

$(OBJ_DIR):
	@mkdir OBJ

$(OFILES): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp $(INC)
	@$(CC) -c $(CFLAGS) $< -o $@

clean :
	@rm -rf $(OBJ_DIR)

fclean : clean
	@rm -rf $(NAME)

re : fclean all

all : $(NAME)

.PHONY : re all clean fclean