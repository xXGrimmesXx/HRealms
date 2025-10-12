CXX=g++
CXXFLAGS=-Wall -Wextra -Werror -std=c++17

# Répertoires
MODELDIR = model
VIEWDIR = view
CONTROLLERDIR = controller
BUILDDIR = build

# Création automatique des listes de fichiers
MODEL_SOURCES = $(wildcard $(MODELDIR)/*.cpp)
VIEW_SOURCES = $(wildcard $(VIEWDIR)/*.cpp )
VIEW_CONSOLE_SOURCES = $(wildcard $(VIEWDIR)/console/*.cpp) 
CONTROLLER_SOURCES = $(wildcard $(CONTROLLERDIR)/*.cpp)
MAIN_SOURCES = herorealms.cpp

# Génération des objets avec chemins préservés
MODEL_OBJECTS = $(MODEL_SOURCES:$(MODELDIR)/%.cpp=$(BUILDDIR)/model_%.o)
VIEW_OBJECTS = $(patsubst $(VIEWDIR)/%.cpp,$(BUILDDIR)/view_%.o,$(filter $(VIEWDIR)/%.cpp,$(VIEW_SOURCES)))
VIEW_CONSOLE_OBJECTS = $(patsubst $(VIEWDIR)/console/%.cpp,$(BUILDDIR)/view_console_%.o,$(filter $(VIEWDIR)/console/%.cpp,$(VIEW_SOURCES)))
CONTROLLER_OBJECTS = $(CONTROLLER_SOURCES:$(CONTROLLERDIR)/%.cpp=$(BUILDDIR)/controller_%.o)
MAIN_OBJECTS = $(MAIN_SOURCES:%.cpp=$(BUILDDIR)/%.o)

ALL_OBJECTS = $(MODEL_OBJECTS) $(VIEW_OBJECTS) $(VIEW_CONSOLE_OBJECTS) $(CONTROLLER_OBJECTS) $(MAIN_OBJECTS)

# Créer les répertoires de sortie
$(BUILDDIR):
	mkdir -p $(BUILDDIR)


# Règles de compilation
$(BUILDDIR)/model_%.o : $(MODELDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -I. $< -c -o $@

$(BUILDDIR)/view_%.o : $(VIEWDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -I. $< -c -o $@

$(BUILDDIR)/view_console_%.o : $(VIEWDIR)/console/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -I. $< -c -o $@

$(BUILDDIR)/controller_%.o : $(CONTROLLERDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -I. $< -c -o $@

$(BUILDDIR)/%.o : %.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -I. $< -c -o $@

# Compilation de l'exécutable
Hrealms : $(ALL_OBJECTS)
	$(CXX) $^ -o $@

.PHONY : run clean

run : Hrealms
	./Hrealms

clean :
	rm -rf $(BUILDDIR) Hrealms

# Aide pour déboguer
print-% :
	@echo '$*=$($*)'