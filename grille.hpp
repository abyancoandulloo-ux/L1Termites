#ifndef GRILLE_HPP
#define GRILLE_HPP
#include <ostream>
#include <array>
#include "coord.hpp"

const float densiteBrindille=0.1;
using namespace std;
struct Case{
	int contient;
	int odeur;
	Case():contient{0},odeur{0}{};
	Case(int y);
};
//Termite>0-,Vide=0,Brindille=-1,Nid=1000
using grille=array<array<Case,tailleGrille>,tailleGrille>;
class Grille{
	private:
	array<array<Case,tailleGrille>,tailleGrille> t;
	public:
	grille gett(){return t;}
	void poseBrindille(Coord c);
	void enleveBrindille(Coord c);
	bool contientBrindille(Coord c)const;
	void poseTermite(Coord c,int idT);
	void poseTermite(Coord c,int idT,int nid);
	void enleveTermite(Coord c);
	int numeroTermite(Coord c)const;
	bool estVide(Coord c)const;
	int caseContient(Coord c)const{return t[c.getLig()][c.getCol()].contient;}
	
	void placeBrindille();
	int nbBrindilleVoisins(Coord c)const;

	void marqueNid(Coord c,int nid);
	void poseBrindille(Coord c,int nid);
	void enleveBrindilleNid(Coord c,int nid);
	int caseOdeur(Coord c)const{return t[c.getLig()][c.getCol()].odeur;}
	void initialiseNid(Coord c,int nid);
	
};
std::ostream &operator<<(std::ostream &sortie,Grille g);
#endif