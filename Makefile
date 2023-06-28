NAME =  ircserv

OBJECTS = objects

HEADERS = $(wildcard inc/*.hpp) $(wildcard src/**/*.hpp) $(wildcard src/httpserver/**/*.hpp);

INC = inc/

C++ = c++

CPP_FLAGS = -Wall -Wextra -Werror -std=c++98 -I$(INC)

SRC =	$(wildcard *.cpp) $(wildcard src/*.cpp) $(wildcard src/**/*.cpp) $(wildcard src/httpserver/**/*.cpp);

OBJ =  $(addprefix objects/, $(SRC:.cpp=.o))

all: $(NAME)
$(NAME) : $(OBJ)
	@$(C++) $(CPP_FLAGS) $(OBJ) -o $(NAME)
	@echo "\033[1;32m    Successfully Maked    \033[0m"

$(OBJECTS)/%.o: %.cpp $(HEADERS)
	@mkdir -p $(@D)
	@$(C++) $(CPP_FLAGS) -c $< -o $@
	
clean:
	@rm -rf $(OBJECTS)
	@echo "\033[1;31m    Cleaned    \033[0m"

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY : re fclean clean all
