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
#include "doctest.h"
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
TEST_CASE("Constructeur"){
	Coord c{1,2};
	CHECK(c.getLig()==1);
	CHECK(c.getCol()==2);
	CHECK_THROWS_AS((Coord{-1,0}),std::invalid_argument);
}

TEST_CASE("Affichage Coord"){
	ostringstream ch;
	SUBCASE("affiche (1,2)"){
		ch<<Coord{1,2};
		CHECK(ch.str()=="(1,2)");
	}
}
TEST_CASE("Egalite"){
	CHECK(Coord{0,0}==Coord{0,0});
	CHECK(Coord{1,2}==Coord{1,2});
	CHECK_FALSE(Coord{0,0}==Coord{1,0});
	CHECK_FALSE(Coord{0,9}==Coord{0,0});
}

TEST_CASE("Affichage Direction"){
	ostringstream ch;
	SUBCASE(" affiche Nord"){
		Direction d=Direction::N;
		ch<<d;
		CHECK(ch.str()=="Nord");
	}
	SUBCASE(" affiche Sud"){
		Direction d=Direction::S;
		ch<<d;
		CHECK(ch.str()=="Sud");
	}
}

TEST_CASE("A gauche"){
	CHECK(aGauche(Direction::N)==Direction::NW);
	CHECK(aGauche(Direction::S)==Direction::SE);
	
}

TEST_CASE("A droite"){
	CHECK(aDroite(Direction::N)==Direction::NE);
	CHECK(aDroite(Direction::S)==Direction::SW);
	
}

TEST_CASE("Directions-droite,gauche"){
	CHECK(aGauche(aDroite(Direction::N))==Direction::N);
	CHECK(aDroite(aGauche(Direction::S))==Direction::S);
	Direction d=Direction::N;
	for(int i=0;i<8;i++){
		d=aDroite(d);
	}
	CHECK(d==Direction::N);
	for(int i=0;i<8;i++){
		d=aGauche(d);
	}
	CHECK(d==Direction::N);
}

TEST_CASE("Devant coord"){
	CHECK(devantCoord(Coord{2,2},Direction::NE)==Coord{3,3});
	CHECK(devantCoord(Coord{2,7},Direction::SW)==Coord{1,6});
	CHECK(devantCoord(Coord{8,7},Direction::SE)==Coord{7,8});
	CHECK(devantCoord(Coord{2,7},Direction::NW)==Coord{3,6});
	CHECK_THROWS_AS(devantCoord(Coord{1,0},Direction::SW),std::invalid_argument);
	Coord c{5,5};
	c=devantCoord(c,Direction::N);
	c=devantCoord(c,Direction::E);
	c=devantCoord(c,Direction::S);
	c=devantCoord(c,Direction::W);
	CHECK(c==Coord{5,5});
}
TEST_CASE("Distance"){
	CHECK(distance(Coord{0,0},Coord{8,6})==10);
	CHECK(distance(Coord{1,1},Coord{4,5})==5);
}

