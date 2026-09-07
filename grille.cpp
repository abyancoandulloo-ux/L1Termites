#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>
#include <ostream>
#include <random>
#include <array>
using namespace std;
#include "coord.hpp"
#include "grille.hpp"
#include "doctest.h"

Case::Case(int y):contient{y}{
	if(y<-1)throw invalid_argument("Nombre<-1");
}
// void Grille::marqueNid(Coord c,int nid){
// 	if(t[c.getLig()][c.getCol()].contient==0){
// 		t[c.getLig()][c.getCol()].contient=nid;
// 	}
// 	else{
// 		throw invalid_argument("La case pour poser le nid n'est pas vide");
// 	}
	
// }

void Grille::marqueNid(Coord c,int nid){
	t[c.getLig()][c.getCol()].odeur=nid;
}

void Grille::poseBrindille(Coord c,int nid){
	poseBrindille(c);
	marqueNid(c,nid);
}
void Grille::enleveBrindilleNid(Coord c,int nid){
	enleveBrindille(c);
	marqueNid(c,nid);
}

void Grille::initialiseNid(Coord c,int nid){
	marqueNid(c,nid);
	t[c.getLig()][c.getCol()].contient=1000;
	
	
}
void Grille::poseBrindille(Coord c){
	if(t[c.getLig()][c.getCol()].contient!=-1){
		t[c.getLig()][c.getCol()].contient=-1;
	}
	else if(t[c.getLig()][c.getCol()].contient==-1){
		throw invalid_argument("Il y a deja une brindille");
	}
}

void Grille::enleveBrindille(Coord c){
	if(t[c.getLig()][c.getCol()].contient==-1)t[c.getLig()][c.getCol()].contient=0;
	else{
		throw invalid_argument("Il n'y a pas de brindille");
	}
}

bool Grille::contientBrindille(Coord c)const{
	return t[c.getLig()][c.getCol()].contient==-1;
	
}

bool Grille::estVide(Coord c)const{
	return t[c.getLig()][c.getCol()].contient==0;
}

void Grille::poseTermite(Coord c,int idT){
	if(estVide(c)){t[c.getLig()][c.getCol()].contient=idT;}
	else{
		throw invalid_argument("La case n'est pas vide pour poser une termite");
	}
}
void Grille::poseTermite(Coord c,int idT,int nid){
	if(estVide(c)){
		t[c.getLig()][c.getCol()].contient=idT;
		t[c.getLig()][c.getCol()].odeur=nid;
	}
	else{
		throw invalid_argument("La case n'est pas vide pour poser une termite");
	}
}
void Grille::enleveTermite(Coord c){
	if(t[c.getLig()][c.getCol()].contient>0){
		t[c.getLig()][c.getCol()].contient=0;
		t[c.getLig()][c.getCol()].odeur=0;
	}
	else{
		throw invalid_argument("Il n'y a pas de termite a enlever "+c.getCol());
	}
}

int Grille::numeroTermite(Coord c)const{
	if(t[c.getLig()][c.getCol()].contient>0)return t[c.getLig()][c.getCol()].contient;
	else{
		throw invalid_argument("Il n'y a pas de termite a cette coordonnée/numero de termite");
	}
}

std::ostream &operator<<(std::ostream &sortie,Grille g){
	// cout<<" ";
	// for(int i=0;i<tailleGrille;i++){
	// 	cout<<"  "<<to_string(i);
	// }
	// cout<<endl;
	cout<<"  0 1 2 3 4 5 6 7 8 9"<<endl;
	for(int i=0;i<tailleGrille;i++){
		sortie<<i<<" ";
		for(int j=0;j<tailleGrille;j++){
			if(g.gett()[i][j].contient==1000)
				sortie<<"\033[33;1m"<<"# "<<"\033[0m";
			else if(g.gett()[i][j].contient>0 and g.gett()[i][j].odeur>=0){
				if(g.gett()[i][j].odeur==1)
				sortie<<"\033[31;1m"<<"T"<<"\033[0m"<<" ";//<<to_string(g.gett()[i][j].odeur)
				else
					sortie<<"\033[34;1m"<<"T"<<"\033[0m"<<" ";//<<to_string(g.gett()[i][j].odeur)
			}
			else if(g.gett()[i][j].contient==-1){
				sortie<<"\033[32;1m"<<"/ "<<"\033[0m"<<" ";
			}
			
			else{sortie<<"  ";}
		}
		sortie<<endl;
	}
	return sortie;
}
void Grille::placeBrindille(){
	int nbBrindille=densiteBrindille*tailleGrille*tailleGrille;
	int i=0;
	while(i!=nbBrindille){
		int x=rand()%tailleGrille;
		int y=rand()%tailleGrille;
		if(estVide(Coord{x,y})){
			poseBrindille(Coord{x,y});
			i++;
		}
	}
	
	
}
TEST_CASE("Case constructeur"){
	CHECK(Case{4}.contient==4);
	CHECK(Case{6}.contient==6);
}

TEST_CASE("Pose et enleve brindille"){
	Grille g;
	
	g.poseBrindille(Coord{2,2});
	CHECK(g.gett()[2][2].contient==-1);
	CHECK_THROWS_AS(g.poseBrindille(Coord{2,2}),std::invalid_argument);
	
	g.poseBrindille(Coord{3,2});
	CHECK(g.gett()[3][2].contient==-1);
	
	g.enleveBrindille(Coord{2,2});
	CHECK_FALSE(g.gett()[2][2].contient==-1);
	CHECK_THROWS_AS(g.enleveBrindille(Coord{2,2}),std::invalid_argument);
	
	CHECK_FALSE(g.contientBrindille(Coord{1,1}));
	CHECK(g.contientBrindille(Coord{3,2}));
	
	CHECK(g.estVide(Coord{9,9}));
	
	g.poseTermite(Coord{7,3},3);
	CHECK(g.gett()[7][3].contient==3);
	CHECK_THROWS_AS(g.poseTermite(Coord{7,3},3),std::invalid_argument);

	g.enleveTermite(Coord{7,3});
	CHECK(g.gett()[7][3].contient==0);
	CHECK_THROWS_AS(g.enleveTermite(Coord{4,3}),std::invalid_argument);

	g.poseTermite(Coord{8,3},3);
	CHECK(g.numeroTermite(Coord{8,3})==3);
	CHECK_THROWS_AS(g.numeroTermite(Coord{4,3}),std::invalid_argument);

}
TEST_CASE("Affichage"){
	// Initialisation
	std::random_device rd;
	std::mt19937 gen(rd());
	
	// Entier entre 0 ou 1
	std::uniform_int_distribution<> dis(0, 1);
	//int x = dis(gen);
	Grille g1;
	for(int i=0;i<tailleGrille;i++){
		for(int j=0;j<tailleGrille;j++){
			int x = dis(gen);
			if(x==0)g1.poseTermite(Coord{i, j}, i + j);
			else{g1.poseBrindille(Coord{i, j});}
			
		}
	}
	//cout << g1;
	
}
int Grille::nbBrindilleVoisins(Coord c)const{
	int nbBrindille=0;
	for(int i=-1;i<2;i++){
		for(int j=-1;j<2;j++){
			if(i==0 and j==0)continue;
			else{
				if(t[c.getLig()+i][c.getCol()+j].contient==-1)nbBrindille++;
			}
		}
	}
	return nbBrindille;
}
TEST_CASE("Nombre voisins brindille"){
	Grille g;
	g.poseBrindille(Coord{5,5});
	g.poseBrindille(Coord{4,5});
	g.poseBrindille(Coord{6,5});
	g.poseBrindille(Coord{5,4});
	CHECK(g.nbBrindilleVoisins(Coord {5,5})==3);
	//cout<<g<<endl;
}