CXX=g++
CXXFLAGS=-std=c++17 -Wall -Wextra -I.

SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
EXEC=jeu

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $@

clean:
	rm -f $(OBJ) $(EXEC)
	