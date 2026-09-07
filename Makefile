coord.o:coord.hpp coord.cpp
	g++ -Wall -c coord.cpp
grille.o:grille.cpp grille.hpp coord.hpp termite.hpp
	g++ -Wall -c grille.cpp
termite.o:termite.cpp termite.hpp grille.hpp coord.hpp
	g++ -Wall -c termite.cpp
test.o:coord.hpp grille.hpp test.cpp
	g++ -Wall -c test.cpp
test: test.o coord.o grille.o termite.o
	g++ test.o coord.o grille.o termite.o -o test
all:
	test
clean:
	rm -f *.o test projet