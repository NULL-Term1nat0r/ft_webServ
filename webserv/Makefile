
# NAME = webserv

# SRC_DIR = src
# OBJ_DIR = objects

# SOURCES =  src/client.cpp \
# 			src/config.cpp \
# 			src/postRequest.cpp \
# 			src/getRequest.cpp \
# 			src/deleteRequest.cpp \
# 			src/serverConfig1.cpp \
# 			src/serverConfig2.cpp \
# 			src/request.cpp \
# 			src/response.cpp \
# 			src/server.cpp \
# 			src/Eike.cpp \
# 			src/parsing.cpp \
# 			src/cgiRequest.cpp \
# 			src/ft_utils.cpp
# OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

# HEADER_DIR = includes

# HEADERS = includes/config.hpp \
# 			includes/postRequest.hpp \
# 			includes/getRequest.hpp \
# 			includes/deleteRequest.hpp \
# 			includes/serverConf.hpp \
# 			includes/request.hpp \
# 			includes/response.hpp \
# 			includes/server.hpp \
# 			includes/parsing.hpp \
# 			includes/cgiRequest.hpp \
# 			includes/ft_utils.hpp \
# 			includes/Header.h

# CC = c++
# FLAGS = -g -Wextra -Wall -Werror -std=c++98

# all: $(NAME)

# $(NAME): $(OBJECTS)
# 	$(CC) $(FLAGS) $(OBJECTS) -o $(NAME)

# $(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) | $(OBJ_DIR)
# 	$(CC) $(FLAGS) -c $< -o $@

# $(OBJ_DIR):
# 	mkdir -p $(OBJ_DIR)

# clean:
# 	rm -rf $(OBJ_DIR)

# fclean: clean
# 	rm -f $(NAME)

# re: fclean all

# .PHONY: all clean fclean re


NAME = webserv
SRC_DIR = src
OBJ_DIR = objects

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

HEADER_DIR = includes
HEADERS = $(wildcard $(HEADER_DIR)/*.hpp)

CC = g++
FLAGS = -g -Wextra -Wall -Werror -std=c++98

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(FLAGS) $(OBJECTS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) | $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
