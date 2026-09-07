
#ifndef COORD_HPP
#define COORD_HPP
#include <ostream>
const int tailleGrille=0;
class Coord{
	int lig,col;
	
	public:
		Coord(int a,int b);
		int getLig()const;
		int getCol()const;
		
};
enum class Direction {
    N,   // Nord
    NE,  // Nord-Est
    E,   // Est
    SE,  // Sud-Est
    S,   // Sud
    SW,  // Sud-Ouest
    W,   // Ouest
    NW   // Nord-Ouest
};
std::ostream &operator<<(std::ostream &sortie,Coord c);
std::ostream &operator<<(std::ostream &sortie,Direction d);
bool operator==(Coord a,Coord b);
Direction aGauche(Direction d);
Direction aDroite(Direction d);
Coord devantCoord(Coord c,Direction d);
int distance(Coord c1,Coord c2);
#endif