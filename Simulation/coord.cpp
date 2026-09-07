#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>
#include <ostream>
#include <cmath>
using namespace std;


#include "coord.hpp"
Coord::Coord(int a,int b):lig{a},col{b}{
		if(a<0 or b<0){
			throw invalid_argument("Param negatif");
		}
	};
int Coord::getLig()const{
	return lig;
}

int Coord::getCol()const{
	return col;
}

std::ostream &operator<<(std::ostream &sortie,Coord c){
	sortie<<"("<<c.getLig()<<","<<c.getCol()<<")";
	return sortie;
}

bool operator==(Coord a,Coord b){
	return a.getLig()==b.getLig() and a.getCol()==b.getCol();
}

std::ostream &operator<<(std::ostream &sortie,Direction d){
	switch(d){
		case Direction::N :sortie<<"Nord";break;
		case Direction::NE :sortie<<"Nord Est";break;
		case Direction::NW :sortie<<"Nord Ouest";break;
		case Direction::S :sortie<<"Sud";break;
		case Direction::SE :sortie<<"Sud Est";break;
		case Direction::SW :sortie<<"Sud Ouest";break;
		case Direction::W :sortie<<"Ouest";break;
		case Direction::E :sortie<<"Est";break;
	}
	return sortie;
}

Direction aGauche(Direction d){
	if(int(d)==0){
		return Direction(7);
	}
	return Direction(int(d)-1);
}

Direction aDroite(Direction d){
	if(int(d)==7){
		return Direction(0);
	}
	return Direction(int(d)+1);
}

Coord devantCoord(Coord c,Direction d){
	int x=c.getLig();
	int y=c.getCol();
	if(int(d)==0 or int(d)==1 or int(d)==7)x++;//x++
	if(int(d)>=1 and int(d)<=3)y++;
	if(int(d)>=3 and int(d)<=5)x--;//x--
	if(int(d)>=5 and int(d)<=7)y--;
	return Coord{x,y};
}
int distance(Coord c1,Coord c2){
	return std::sqrt((c1.getLig()-c2.getLig())*(c1.getLig()-c2.getLig())+(c1.getCol()-c2.getCol())*(c1.getCol()-c2.getCol()));
}


