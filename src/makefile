CXX=g++
CXXFLAGS=-Wall -Wextra -Werror -std=c++17

# Répertoires
MODELDIR = model
VIEWDIR = view
CONTROLLERDIR = controller
OBJDIR = obj

# Création automatique des listes de fichiers
MODEL_SOURCES = $(wildcard $(MODELDIR)/*.cpp)
VIEW_SOURCES = $(wildcard $(VIEWDIR)/*.cpp $(VIEWDIR)/*/*.cpp)
CONTROLLER_SOURCES = $(wildcard $(CONTROLLERDIR)/*.cpp)
MAIN_SOURCES = herorealms.cpp

# Génération des objets avec chemins préservés
MODEL_OBJECTS = $(MODEL_SOURCES:$(MODELDIR)/%.cpp=$(OBJDIR)/model_%.o)
VIEW_OBJECTS = $(VIEW_SOURCES:%.cpp=$(OBJDIR)/%.o)
CONTROLLER_OBJECTS = $(CONTROLLER_SOURCES:$(CONTROLLERDIR)/%.cpp=$(OBJDIR)/controller_%.o)
MAIN_OBJECTS = $(MAIN_SOURCES:%.cpp=$(OBJDIR)/%.o)

ALL_OBJECTS = $(MODEL_OBJECTS) $(VIEW_OBJECTS) $(CONTROLLER_OBJECTS) $(MAIN_OBJECTS)

# Créer les répertoires de sortie
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Règles de compilation
$(OBJDIR)/model_%.o : $(MODELDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -I. $< -c -o $@

$(OBJDIR)/view_%.o : $(VIEWDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -I. $< -c -o $@

$(OBJDIR)/controller_%.o : $(CONTROLLERDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -I. $< -c -o $@

$(OBJDIR)/%.o : %.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -I. $< -c -o $@

# Compilation de l'exécutable
Hrealms : $(ALL_OBJECTS)
	$(CXX) $^ -o $@

.PHONY : run clean

run : Hrealms
	./Hrealms

clean :
	rm -rf $(OBJDIR) Hrealms