NAME		= 	ircserv

OBJDIR		=	Other

SRCDIR		=	Sources

CC			= 	c++

STD			= 	-std=c++98

CXXFLAGS	= 	-Wall -Wextra -Werror

SRC			= 	main.cpp parsing.cpp\

INC			= 	Includes/ircserv.hpp 

OBJ			= 	$(addprefix $(OBJDIR)/, $(SRC:%.cpp=%.o))

##### COLORS ####################
WHITE		=	"\033[m"        #
RED			=	"\033[1;31m"    #
GREEN		=	"\033[1;32m"    #
YELLOW		=	"\033[1;33m"    #
BLUE		=	"\033[1;34m"    #
PURPLE		=	"\033[1;35m"    #
CYAN		=	"\033[1;36m"    #
EOC			=	"\033[0;0m"     #
LINE_CLEAR	=	"\x1b[1A\x1b[M" #
#################################

all : $(NAME) 

$(NAME) : $(OBJ)
	@echo $(GREEN) "Source files are compiled!\n" $(EOC)
	@echo $(WHITE) "Building $(NAME) for"$(YELLOW)"Mandatory"$(WHITE)"..." $(EOC)
	@$(CC) $(CXXFLAGS) $(STD) $(OBJ) -o $(NAME)
	@echo $(GREEN) "$(NAME) is created!\n" $(EOC) 

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(INC)
	@echo $(YELLOW) "Compiling...\t" $< $(EOC) $(LINE_CLEAR)
	@mkdir -p Other
	@$(CC) $(CXXFLAGS) -c $< -o $@ 

clean :
	@rm -rf $(OBJ)
	@echo $(RED) "Object files are cleaned!" $(EOC) 

fclean : clean
	@rm -rf $(NAME)
	@rm -rf $(OBJDIR)
	@echo $(RED) "$(NAME) is removed!" $(EOC) 

re : fclean all 