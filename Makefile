NAME		= webserv

CXX			= c++
CXXFLAGS 	= -Wall -Wextra -Werror -std=c++98
INCLUDES 	= Client/inc Config/inc ConfigParser/inc RequestParser/inc Server/inc
CXXFLAGS 	+= $(addprefix -I, $(INCLUDES))

SRCS 		= $(wildcard Client/src/*.cpp) $(wildcard Config/src/*.cpp) $(wildcard ConfigParser/src/*.cpp) $(wildcard RequestParser/src/*.cpp) $(wildcard Server/src/*.cpp)
OBJS 		= $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re