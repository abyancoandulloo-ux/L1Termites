#coord.o:coord.hpp coord.cpp
#	g++ -Wall -c coord.cpp
#grille.o:grille.cpp grille.hpp coord.hpp termite.hpp
#	g++ -Wall -c grille.cpp
#termite.o:termite.cpp termite.hpp grille.hpp coord.hpp
#	g++ -Wall -c termite.cpp
#projet.o:coord.hpp grille.hpp projet.cpp
#	g++ -Wall -c projet.cpp
#projet:projet.o coord.o grille.o termite.o
#	g++ projet.o coord.o grille.o termite.o -o projet
#clean:
#	rm -f *.o test projet
CXX = g++
CXXFLAGS = -Wall -std=c++17
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Fichiers objets
OBJS = coord.o grille.o termite.o projet.o

# Cible par dÃ©faut
all: projet

# RÃ¨gles de compilation
coord.o: coord.hpp coord.cpp
	$(CXX) $(CXXFLAGS) -c coord.cpp

grille.o: grille.cpp grille.hpp coord.hpp termite.hpp
	$(CXX) $(CXXFLAGS) -c grille.cpp

termite.o: termite.cpp termite.hpp grille.hpp coord.hpp
	$(CXX) $(CXXFLAGS) -c termite.cpp

projet.o: coord.hpp grille.hpp projet.cpp
	$(CXX) $(CXXFLAGS) -c projet.cpp

# Lien final
projet: $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)
main.o:coord.hpp grille.hpp main.cpp
	g++ -Wall -c main.cpp
main:main.o coord.o grille.o termite.o
	g++ main.o coord.o grille.o termite.o -o main
# Nettoyage
clean:
	rm -f *.o projet

# ExÃ©cution
run: projet
	./projet

.PHONY: all clean run