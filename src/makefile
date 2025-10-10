CXX=g++
CXXFLAGS=-Wall -Wextra -Werror

# Trouver automatiquement tous les fichiers .cpp
SOURCES = $(wildcard *.cpp)
# Générer la liste des .o correspondants
OBJECTS = $(SOURCES:.cpp=.o)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $< -c -o $@
Hrealms : $(OBJECTS)
	$(CXX) $^ -o $@
.PHONY : run clean
run : Hrealms
	./$<
clean :
	rm -f *.o *~ *.core Hrealms