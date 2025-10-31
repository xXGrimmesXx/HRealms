CXX=g++
CXXFLAGS=-Wall -Wextra -Werror -std=c++17 -Iinclude

# Répertoires
MODELDIR = src/model
VIEWDIR = src/view
CONTROLLERDIR = src/controller
SERIALIZATIONDIR = src/serialization
BUILDDIR = build

# Création automatique des listes de fichiers
MODEL_SOURCES = $(wildcard $(MODELDIR)/*.cpp)
VIEW_SOURCES = $(wildcard $(VIEWDIR)/*.cpp)
VIEW_CONSOLE_SOURCES = $(wildcard $(VIEWDIR)/console/*.cpp)
CONTROLLER_SOURCES = $(wildcard $(CONTROLLERDIR)/*.cpp)
SERIALIZATION_SOURCES = $(wildcard $(SERIALIZATIONDIR)/*.cpp)
MAIN_SOURCES = src/herorealms.cpp

# Génération des objets - CORRIGÉ
MODEL_OBJECTS = $(MODEL_SOURCES:$(MODELDIR)/%.cpp=$(BUILDDIR)/model_%.o)
VIEW_OBJECTS = $(VIEW_SOURCES:$(VIEWDIR)/%.cpp=$(BUILDDIR)/view_%.o)
VIEW_CONSOLE_OBJECTS = $(VIEW_CONSOLE_SOURCES:$(VIEWDIR)/console/%.cpp=$(BUILDDIR)/view_console_%.o)
CONTROLLER_OBJECTS = $(CONTROLLER_SOURCES:$(CONTROLLERDIR)/%.cpp=$(BUILDDIR)/controller_%.o)
SERIALIZATION_OBJECTS = $(SERIALIZATION_SOURCES:$(SERIALIZATIONDIR)/%.cpp=$(BUILDDIR)/serialization_%.o)
MAIN_OBJECTS = $(MAIN_SOURCES:src/%.cpp=$(BUILDDIR)/%.o)

ALL_OBJECTS = $(MODEL_OBJECTS) $(VIEW_OBJECTS) $(VIEW_CONSOLE_OBJECTS) $(CONTROLLER_OBJECTS) $(SERIALIZATION_OBJECTS) $(MAIN_OBJECTS)

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

$(BUILDDIR)/serialization_%.o : $(SERIALIZATIONDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -I. $< -c -o $@

$(BUILDDIR)/herorealms.o : src/herorealms.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -I. $< -c -o $@

$(BUILDDIR)/%.o : %.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -I. $< -c -o $@

# Compilation de l'exécutable
Hrealms : $(ALL_OBJECTS)
	$(CXX) $^ -o $@

.PHONY : run clean debug

run : Hrealms
	./Hrealms

debug : 
	@echo "MODEL_SOURCES: $(MODEL_SOURCES)"
	@echo "VIEW_SOURCES: $(VIEW_SOURCES)"
	@echo "VIEW_CONSOLE_SOURCES: $(VIEW_CONSOLE_SOURCES)"
	@echo "ALL_OBJECTS: $(ALL_OBJECTS)"

clean :
	rm -rf $(BUILDDIR) Hrealms