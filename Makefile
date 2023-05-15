NAME		= webserv

CXX			= c++
CXXFLAGS 	= -Wall -Wextra -Werror -std=c++98
INCLUDES 	= Client/inc Config/inc ConfigParser/inc RequestParser/inc Server/inc
CXXFLAGS 	+= $(addprefix -I, $(INCLUDES))

SRCS 		= $(filter-out Tester/*, $(wildcard */src/*.cpp)) main.cpp
OBJS 		= $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ -g

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -g

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re