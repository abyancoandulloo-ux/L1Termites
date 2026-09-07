#ifndef TERMITE_HPP
#define TERMITE_HPP

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>
#include <ostream>
#include <array>
using namespace std;
#include "coord.hpp"
#include "grille.hpp"
const int nbTermites=10;
const int dureeSablier=6;
const int probaTourner=0.1;
const int rayonNid=1;
using grille=array<array<Case,tailleGrille>,tailleGrille>;

class Termite{
	int idT;
	Coord pos;
	Direction d;
	bool brindille;
	int nid;
	public:
		Termite(int id,Coord position);
		Termite(int id,Coord position,Direction direct);
		int getIdT()const{return idT;}
		Coord getPos()const{return pos;}
		Direction getDirection()const{return d;}
		bool porteBrindille(){return brindille;}
		void setBrindille(bool b){brindille=b;}
		Coord devant()const;
		void tourneADroite(){d=aDroite(d);}
		void tourneAGauche(){d=aGauche(d);}
		void tourneAleat();

		bool laVoieEstLibre(Grille &g)const;
		bool brindilleEnFace(Grille &g)const;
		int voisinsLibre(Grille &g);
		void avance(Grille &g);
		void marcheAleatoire(Grille &g);
		void chargeBrindille(Grille &g);
		void dechargeBrindille(Grille &g);
		void chargeRassembleBrindille(Grille &g);
		void dechargeRassembleBrindille(Grille &g);
		void tunnel(Grille &g);
		void rassemblerBrindille(Grille &g,int &i);
		void vieTermite(Grille &g,int &i);

		int voisinsdeBrindilleEnFace(Grille &g)const;
		Termite(int id,Coord position,Direction direct,int nidT);
		int getNid()const{return nid;}
		void chargeBrindilleNid(Grille &g,Coord cNid);
		void dechargeBrindilleNid(Grille &g,Coord cNid);
		void rassemblerBrindilleNid(Grille &g,int &i,Coord cNid);

		void attaqueNid(Grille &g,int &gagnant);
		void naissance(Grille &g);
		void avanceVers(Coord c,Grille &g);
		void vieTermiteNid(Grille &g,int &i,Coord cNid);
};
using colonnie=vector<Termite>;
bool coherence(colonnie &termites,Grille &g);
void supprimeTermite(int id,colonnie &termites,Grille &g);
void ajouteTermite(Coord nid,colonnie &termites,Grille &g);
colonnie colonnieDeTermites(Grille &g);
colonnie colonnieDeTermitesNid(Grille &g,int nid);
void attaque(Grille &g,colonnie &termitesEnnemi,Termite &t);
void transmettreBrindille(colonnie &colonnieTermite);
void affichage(Grille &g,colonnie &c1,colonnie &c2);
bool affichageBrindille(colonnie &c1,Coord c);
Direction affichageDirection(colonnie &c1,Coord c);
#endif
