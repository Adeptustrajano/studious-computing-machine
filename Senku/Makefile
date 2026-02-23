CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -MMD -MP

# OBJS: Lista de archivos objeto que se generarán.
# DEPS: Lista de archivos (.d) generados automáticamente.
OBJS = mainGSenku.o GSenku.o
DEPS = $(OBJS:.o=.d)

# Compila y genera el ejecutable principal `mainGSenku`.
all: mainGSenku

# $^: Representa todos los prerequisitos (en este caso, los archivos .o).
mainGSenku: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# $<: Representa el primer prerequisito (archivo fuente .cpp).
# $@: Representa el objetivo (archivo objeto .o).
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Borra los archivos objeto y el ejecutable. y .o.
clean:
	$(RM) $(OBJS) $(DEPS) mainGSenku

# PHONY.
# Esto asegura que `clean` no entre en conflicto con un archivo llamado `clean`.
.PHONY: all clean

# si un archivo fuente o cabecera cambia, se recompilarán los archivos afectados.
-include $(DEPS) 
# Lista de archivos .d generados automáticamente.
