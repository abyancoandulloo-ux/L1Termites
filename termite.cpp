#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>
#include <ostream>
#include <array>
#include <ctime>
#include <cmath>
using namespace std;
#include "coord.hpp"
#include "grille.hpp"
#include "termite.hpp"
#include "doctest.h"

Termite::Termite(int id,Coord position):idT{id},pos{position}{
	if(id<=0)
		throw invalid_argument("Identifiant de termite invalide");
}

Termite::Termite(int id,Coord position,Direction direct):idT{id},pos{position},d{direct},brindille{false}{
	if(id<=0)
		throw invalid_argument("Identifiant de termite invalide");
}
Coord Termite::devant()const{
	return devantCoord(pos,d);
}

void Termite::tourneAleat(){
	int prob=rand()%100;
	if(probaTourner*100>=prob){
		int x=rand()%9;
		d=Direction(x);
	}
	
}

bool Termite::laVoieEstLibre(Grille &g)const{
	Coord caseDevant=devant();
	return g.estVide(caseDevant);
}

bool Termite::brindilleEnFace(Grille &g)const{
	Coord caseDevant=devant();
	return g.caseContient(caseDevant)==-1;
	
}

int Termite::voisinsLibre(Grille &g){
	int voisins=0;
	for(int i=0;i<8;i++){
		if(laVoieEstLibre(g))voisins++;
			tourneADroite();
	}
	return voisins;
	
}
void Termite::avance(Grille &g){
	Coord caseDevant=devant();
	if(g.estVide(caseDevant)){
		g.enleveTermite(pos);
		pos=caseDevant;
		g.poseTermite(caseDevant,idT,nid);//NID 
	}
}
void Termite::avanceVers(Coord c,Grille &g){
	if(pos.getLig()!=0 and pos.getCol()!=0 and pos.getLig()!=tailleGrille-1 and pos.getCol()!=tailleGrille-1){
	int dmin=distance(pos,c);
	Direction direct=d;
	Coord deplacement=pos;
	for(int i=0;i<8;i++){
		Coord caseDevant=devant();
		if(g.estVide(caseDevant)){
			int dist=distance(caseDevant,c);
			if(dist<dmin){
				dmin=dist;
				direct=d;
				deplacement=caseDevant;
			}
		}
		tourneADroite();
	}
	if(deplacement==pos)return;
		d=direct;
		g.enleveTermite(pos);
		pos=deplacement;
		g.poseTermite(deplacement,idT,nid);

	}
}
void Termite::marcheAleatoire(Grille &g){
	Coord initial=pos;
	
	for(int i=0;i<8;i++){
	//while(pos==initial){
		// if(pos.getLig()==0 or pos.getCol()==0){
		// 	g.enleveTermite(pos);
		// 	pos=Coord{tailleGrille/2,tailleGrille/2};
		// 	g.poseTermite(pos,idT);
		// }
		// if(pos.getLig()==0 or pos.getCol()==0 or pos.getLig()==tailleGrille-1 or pos.getCol()==tailleGrille-1){
		// 	tunnel(g);
		// }
		tunnel(g);
		tourneAleat();
		avance(g);
	//}
	}
}

void Termite::chargeBrindille(Grille &g){
	if(!porteBrindille() and brindilleEnFace(g)){
		Coord caseDevant=devant();
		g.enleveBrindille(caseDevant);
		brindille=true;
	}
}
void Termite::chargeRassembleBrindille(Grille &g){
	if(pos.getLig()!=0 and pos.getCol()!=0 and pos.getLig()!=tailleGrille-1 and pos.getCol()!=tailleGrille-1){
		Coord cmin=devant();
		int voisinsMin=g.nbBrindilleVoisins(devant());
		for(int i=-1;i<2;i++){
			for(int j=-1;j<2;j++){
				if(i==0 and j==0)continue;
				else{
					Coord courant{pos.getLig()+i,pos.getCol()+j};
					if(g.contientBrindille(courant)){
						if(g.nbBrindilleVoisins(courant)>voisinsMin and g.nbBrindilleVoisins(courant)<3){
							cmin=courant;
							voisinsMin=g.nbBrindilleVoisins(courant);
						}
					}
				}
			}
		}
		g.enleveBrindille(cmin);
		brindille=true;
	}
}
void Termite::chargeBrindilleNid(Grille &g,Coord cNid){
	if(pos.getLig()!=0 and pos.getCol()!=0 and pos.getLig()!=tailleGrille-1 and pos.getCol()!=tailleGrille-1){
		Coord cmax=cNid;
		int disMax=distance(cNid,cNid);
		for(int i=-1;i<2;i++){
			for(int j=-1;j<2;j++){
				if(i==0 and j==0){
					continue;
				}
				Coord courant{pos.getLig()+i,pos.getCol()+j};
				if(g.contientBrindille(courant)){
					if(distance(cNid,courant)<rayonNid){
						if(g.caseOdeur(courant)==nid)continue;
						else{g.marqueNid(courant,nid);}
					}else{
						if(distance(cNid,courant)>disMax){
							disMax=distance(cNid,courant);
							cmax=courant;
						}
					}
				}
			}
		}
		if(cmax==cNid){
			return;
		}
		else{
			g.enleveBrindille(cmax);
			g.marqueNid(cmax,0);
			brindille=true;
		}
	}
	
}
void Termite::dechargeBrindille(Grille &g){
	Direction initial=d;
	do{
		tourneADroite();
		Coord caseDevant=devant();
		//Evite de s enfermer
		if(g.estVide(caseDevant) and voisinsLibre(g)>1){
			g.poseBrindille(caseDevant);
			brindille=false;
		}
	}while(porteBrindille());
	d=initial;
}

void Termite::dechargeRassembleBrindille(Grille &g){
	if(pos.getLig()!=0 and pos.getCol()!=0 and pos.getLig()!=tailleGrille-1 and pos.getCol()!=tailleGrille-1){
		Coord cmax=devant();
		int voisinsMax=0;
		for(int i=-1;i<2;i++){
			for(int j=-1;j<2;j++){
				if(i==0 and j==0)continue;
				else{
					Coord courant{pos.getLig()+i,pos.getCol()+j};
					if(g.estVide(courant)){
						if( g.nbBrindilleVoisins(courant)>=2){
							//g.nbBrindilleVoisins(courant)>=voisinsMax or
							cmax=courant;
							voisinsMax=g.nbBrindilleVoisins(courant);
						}
					}
				}
			}
		}
		if(cmax.getLig()!=devant().getLig() and cmax.getCol()!=devant().getCol()){
			g.poseBrindille(cmax);
			brindille=false;
		}
	}
}

void Termite::dechargeBrindilleNid(Grille &g,Coord cNid){
	if(pos.getLig()!=0 and pos.getCol()!=0 and pos.getLig()!=tailleGrille-1 and pos.getCol()!=tailleGrille-1){
		
		Coord cmin{pos.getLig()+tailleGrille,pos.getCol()+tailleGrille};
		int disMin=distance(cmin,cNid);
		
		for(int i=-1;i<2;i++){
			for(int j=-1;j<2;j++){
				
				if(i==0 and j==0){
					continue;
				}
				
				Coord courant{pos.getLig()+i,pos.getCol()+j};
				if(g.estVide(courant)){
						if(distance(cNid,courant)<rayonNid){
							if(g.caseOdeur(courant)==nid)continue;
	 						else{g.marqueNid(courant,nid);}
						}else{
 						if(distance(cNid,courant)<disMin){
 							cmin=courant;
							disMin=distance(courant,cNid);
 						}
 					}
							
						}
					}
			}
		if( cmin==Coord{pos.getLig()+tailleGrille,pos.getCol()+tailleGrille}){
			return;
		}else{
			if(g.estVide(cmin)){
				g.poseBrindille(cmin,nid);
				brindille=false;
			}
		}
		}
	
}

// if(g.contientBrindille(courant)){
// 					if(distance(cNid,courant)<3){
// 						if(g.caseOdeur(courant)==nid)continue;
// 						else{g.marqueNid(courant,nid);}
// 					}else{
// 						if(distance(cNid,courant)>disMax){
// 							disMax=distance(cNid,courant);
// 							cmax=courant;
// 						}
// 					}
// 				}
void Termite::tunnel(Grille &g){
	if(pos.getLig()==0 or pos.getLig()==tailleGrille-1){
		for(int i=1;i<tailleGrille;i++){
			if(g.estVide(Coord{i,tailleGrille/2})){
				g.enleveTermite(pos);
		 		pos=Coord{i,tailleGrille/2};
				//cout<<"TUNNEL"<<endl;
		 		g.poseTermite(pos,idT,nid);
				break;
			}
			
		}
		
	 }
	else if(pos.getCol()==0 or pos.getCol()==tailleGrille-1){
		for(int i=1;i<tailleGrille;i++){
			if(g.estVide(Coord{tailleGrille/2,i})){
				g.enleveTermite(pos);
		 		pos=Coord{tailleGrille/2,i};
				//cout<<"TUNNEL"<<endl;
		 		g.poseTermite(pos,idT,nid);
				break;
			}
		}
	 }
}
void Termite::rassemblerBrindille(Grille &g,int &i){
		if(brindilleEnFace(g) and !porteBrindille()){
		 	//chargeRassembleBrindille(g);
			chargeBrindilleNid(g,Coord{5,5});
		 
         }
		if(i%dureeSablier==0 and porteBrindille()){
		 	//dechargeBrindille(g);
			//dechargeRassembleBrindille(g);
			dechargeBrindilleNid(g,Coord{5,5});
		}
}
void Termite::rassemblerBrindilleNid(Grille &g,int &i,Coord cNid){
	if(brindilleEnFace(g) and !porteBrindille()){
		 	//chargeRassembleBrindille(g);
			chargeBrindilleNid(g,cNid);
		 
         }
		if(i%dureeSablier==0 and porteBrindille() and voisinsLibre(g)>2){
		 	//dechargeBrindille(g);
			//dechargeRassembleBrindille(g);
			dechargeBrindilleNid(g,cNid);
		}
}

void Termite::vieTermite(Grille &g,int &i){
		int j=0;
		if(i>100)attaqueNid(g,j);
		tunnel(g);
		rassemblerBrindille(g,i);
		marcheAleatoire(g);
}
void Termite::vieTermiteNid(Grille &g,int &i,Coord cNid){
		tunnel(g);
		rassemblerBrindilleNid(g,i,cNid);
		if(porteBrindille() and i%2==0)avanceVers(cNid,g);
		
		marcheAleatoire(g);
}
bool coherence(colonnie &termites,Grille &g){
	for(size_t i=0;i<termites.size();i++){
		if(termites[i].getIdT()!=i+1)throw invalid_argument("Identifiant de termite mal initialisé");
		if(termites[i].getIdT()!=g.caseContient(termites[i].getPos()))throw invalid_argument("Incoherence de termite avec grille "+to_string(i));
	}
	return true;
}

colonnie colonnieDeTermites(Grille &g){
	colonnie t={};
	for(int i=0;i<nbTermites;i++){
		int x=rand()%9;
		int y=rand()%tailleGrille;
		int z=rand()%tailleGrille;
		Coord c{y,z};
		if(g.estVide(c)){
			t.push_back(Termite{i+1,c,Direction(x)});
			g.poseTermite(c,i+1);
		}else{
			while(!g.estVide(c)){
				y=rand()%tailleGrille;
				z=rand()%tailleGrille;
				c=Coord{y,z};
			}
			t.push_back(Termite{i+1,c,Direction(x)});
			g.poseTermite(c,i+1);
		}
	}
	return t;
}

Termite::Termite(int id,Coord position,Direction direct,int nidT):idT{id},pos{position},d{direct},brindille{false},nid{nidT}{
	if(id<=0)
		throw invalid_argument("Identifiant de termite invalide");
}

colonnie colonnieDeTermitesNid(Grille &g,int nid){
	colonnie t={};
	for(int i=0;i<nbTermites;i++){
		int x=rand()%9;
		int y=rand()%tailleGrille;
		int z=rand()%tailleGrille;
		Coord c{y,z};
		if(g.estVide(c)){
			t.push_back(Termite{i+1,c,Direction(x),nid});
			g.poseTermite(c,i+1,nid);
		}else{
			while(!g.estVide(c)){
				y=rand()%tailleGrille;
				z=rand()%tailleGrille;
				c=Coord{y,z};
			}
			t.push_back(Termite{i+1,c,Direction(x),nid});
			g.poseTermite(c,i+1,nid);
			
		}
	}
	return t;
}
void supprimeTermite(int id,colonnie &termites,Grille &g){

	int indice=-1;
	for(int i=0;i<termites.size();i++){
		
		if(termites[i].getIdT()==id){
			indice=i;
			break;
		}
	}
	if(indice==-1)return;
	Termite &tMort=termites[indice];
	Coord position=tMort.getPos();
	if(g.caseContient(position)>0){
		g.enleveTermite(position);
		if(tMort.porteBrindille()){
			g.poseBrindille(position);
		}
		swap(termites[indice],termites[termites.size()-1]);
		termites.pop_back();
	}
}

void attaque(Grille &g,vector<Termite> &termitesEnnemi,Termite &t){
	Coord pos=t.getPos();
	int nid=t.getNid();
	if(pos.getLig()!=0 and pos.getCol()!=0 and pos.getLig()!=tailleGrille-1 and pos.getCol()!=tailleGrille-1 and !t.porteBrindille()){
	Coord caseDevant=t.devant();
	if(g.caseContient(caseDevant)>0 and g.caseOdeur(caseDevant)!=nid){
		int id=g.caseContient(caseDevant);
		supprimeTermite(id,termitesEnnemi,g);
		}
	}
}
void Termite::attaqueNid(Grille &g,int &gagnant){
	if(pos.getLig()!=0 and pos.getCol()!=0 and pos.getLig()!=tailleGrille-1 and pos.getCol()!=tailleGrille-1){
		for(int i=-1;i<2;i++){
				for(int j=-1;j<2;j++){
					if(i==0 and j==0){
						continue;
					}
					Coord courant{pos.getLig()+i,pos.getCol()+j};
					if(g.caseContient(courant)==1000 and g.caseOdeur(courant)!=nid ){
						g.marqueNid(courant,nid);
						cout<<"Colonnie "<<nid<<" ont gagné"<<endl;
						gagnant=nid;
						break;
					}
				}
		}
	}
}
	
void ajouteTermite(Coord nid,colonnie &termites,Grille &g){
	int col=nid.getCol();
	int i;
	for(i=0;i<tailleGrille;i++){
		if(g.estVide(Coord{i,col}))break;
	}
	Coord pos{i,col};
	int x=rand()%9;
	Direction d=Direction(x);
	int id=termites.size()+1;
	Termite t{id,pos,d,g.caseOdeur(nid)};
	g.poseTermite(pos,id,g.caseOdeur(nid));
	termites.push_back(t);
}

void transmettreBrindille(colonnie &colonnieTermite){
	for(int i=0;i<colonnieTermite.size();i++){
		Termite courant=colonnieTermite[i];
		if(courant.porteBrindille()){
			for(int j=i;j<colonnieTermite.size();j++){
					Termite iter=colonnieTermite[j];
					if(!iter.porteBrindille() and distance(iter.getPos(),courant.getPos())<sqrt(2)){
						courant.setBrindille(false);
						iter.setBrindille(true);
					}
				}
		}
	}
	
}
TEST_CASE("Constructeur"){
	Termite t{1,Coord{1,2}};
	CHECK(t.getIdT()==1);
	CHECK_THROWS_AS((Termite{-1,Coord{1,2}}),invalid_argument);
	CHECK_THROWS_AS((Termite{1,Coord{-1,2}}),invalid_argument);

	Termite t1{1,Coord{1,2},Direction::N};
	CHECK(t1.getDirection()==Direction::N);
}

TEST_CASE("Devant"){
	CHECK(Termite{1,Coord{1,2},Direction::N}.devant()==Coord{2,2});
	CHECK(Termite{1,Coord{8,2},Direction::NW}.devant()==Coord{9,1});
	CHECK(Termite{1,Coord{7,6},Direction::SE}.devant()==Coord{6,7});
	CHECK_THROWS_AS((Termite{1,Coord{0,0},Direction::SE}.devant()),invalid_argument);
}

TEST_CASE("Tourner"){
	Termite t{1,Coord{1,2},Direction::N};
	for(int i=0;i<8;i++){
		t.tourneAGauche();
	}
	CHECK(t.getDirection()==Direction::N);
	t.tourneAGauche();
	CHECK(t.getDirection()==Direction::NW);

	for(int i=0;i<8;i++){
		t.tourneADroite();
	}
	CHECK(t.getDirection()==Direction::NW);
	t.tourneADroite();
	CHECK(t.getDirection()==Direction::N);
	// srand(time(NULL));
	// for(int i=0;i<8;i++){
	// 	t.tourneAleat();
	// 	cout<<t.getDirection()<<endl;
	// }
}
TEST_CASE("La voie est libre"){
	Grille g;
	g.poseTermite(Coord{2,2},1);
	Termite t{1,Coord{2,2},Direction::N};
	CHECK(t.laVoieEstLibre(g));
	g.poseTermite(Coord{3,2},2);
	CHECK_FALSE(t.laVoieEstLibre(g));
	Termite t1{3,Coord{0,0},Direction::S};
	g.poseTermite(Coord{0,0},3);
	CHECK_THROWS_AS(t1.laVoieEstLibre(g),invalid_argument);
}

TEST_CASE("Brindille en face"){
	Grille g;
	g.poseTermite(Coord{2,2},1);
	Termite t{1,Coord{2,2},Direction::N};
	g.poseBrindille(Coord{3,2});
	CHECK(t.brindilleEnFace(g));
	Termite t1{3,Coord{0,0},Direction::S};
	g.poseTermite(Coord{0,0},3);
	CHECK_THROWS_AS(t1.brindilleEnFace(g),invalid_argument);
}

TEST_CASE("Voisins libres"){
	Grille g;
	g.poseTermite(Coord{2,2},1);
	Termite t{1,Coord{2,2},Direction::N};
	g.poseBrindille(Coord{3,2});
	CHECK(t.voisinsLibre(g)==7);
	g.poseBrindille(Coord{2,1});
	g.poseBrindille(Coord{2,3});
	CHECK(t.voisinsLibre(g)==5);
	CHECK(t.getDirection()==Direction::N);
	Termite t1{1,Coord{0,2},Direction::N};
	CHECK_THROWS_AS(t1.voisinsLibre(g),invalid_argument);
}

TEST_CASE("Avance"){
	Grille g;
	g.poseTermite(Coord{2,2},1);
	Termite t{1,Coord{2,2},Direction::N};
	
	// g.poseBrindille(Coord{2,1});
	// g.poseBrindille(Coord{2,3});
	t.avance(g);
	CHECK(t.getPos()==Coord{3,2});
	CHECK(g.caseContient(Coord{3,2})==1);
	CHECK(g.caseContient(Coord{2,2})==0);

	t.tourneADroite();
	CHECK(t.getDirection()==Direction::NE);
	t.avance(g);
	CHECK(t.getPos()==Coord{4,3});
	CHECK(g.caseContient(Coord{3,2})==0);
	CHECK(g.caseContient(Coord{4,3})==1);
}
TEST_CASE("Charge et decharge brindille"){
	Grille g;
	g.poseTermite(Coord{2,2},1);
	Termite t{1,Coord{2,2},Direction::N};
	g.poseBrindille(Coord{3,2});
	t.chargeBrindille(g);
	CHECK(t.porteBrindille());
	CHECK(g.caseContient(Coord{3,2})==0);
	t.avance(g);
	CHECK(t.getPos()==Coord{3,2});
	CHECK(g.caseContient(Coord{3,2})==1);
	t.dechargeBrindille(g);
	CHECK_FALSE(t.porteBrindille());
	CHECK(g.caseContient(Coord{4,3})==-1);
}
TEST_CASE("Coherence"){
	Termite t1{1,Coord{2,2},Direction::N};
	Termite t2{2,Coord{4,2},Direction::S};
	Termite t3{3,Coord{9,2},Direction::E};
	colonnie termites={t1,t2,t3};
	Grille g;
	g.poseTermite(Coord{2,2},1);
	g.poseTermite(Coord{4,2},2);
	g.poseTermite(Coord{9,2},3);
	CHECK(coherence(termites,g));
	Termite t4{3,Coord{7,2},Direction::E};
	g.poseTermite(Coord{7,2},3);
	termites.push_back(t4);
	CHECK_THROWS_AS(coherence(termites,g),invalid_argument);
	g.enleveTermite(Coord{7,2});
	termites.pop_back();
	
	Termite t5{4,Coord{7,2},Direction::E};
	g.poseTermite(Coord{8,2},3);
	termites.push_back(t5);
	CHECK_THROWS_AS(coherence(termites,g),invalid_argument);
}

TEST_CASE("Tunnel"){
	Grille g;
	g.poseTermite(Coord{1,0},1);
	Termite t{1,Coord{1,0},Direction::S};
	//cout<<g;
	int i=1;
	t.tunnel(g);
	//cout<<g;
}
TEST_CASE("Affichage vie de termite"){
	Grille g;
	// g.poseBrindille(Coord{1,2});
	// g.poseBrindille(Coord{2,1});
	// g.poseBrindille(Coord{2,2});
	g.poseTermite(Coord{1,1},1);
	g.poseTermite(Coord{4,1},2);
	g.poseTermite(Coord{3,1},3);
	Termite t1{1,Coord{1,1},Direction::NE};
	Termite t2{2,Coord{4,1},Direction::N};
	Termite t3{3,Coord{3,1},Direction::SE};
	colonnie c={t1,t2,t3};
	//cout<<g;
	for(int i=1;i<5;i++){
		//if(t1.brindilleEnFace(g))t1.chargeBrindille(g);
		// t1.vieTermite(g,i);
		// t2.vieTermite(g,i);
		// t3.vieTermite(g,i);
		// cout<<g;
		// cout<<endl;
		// cout<<"+++++++++++++++++"<<i;
		// cout<<endl;
		// CHECK(coherence(c,g));
		for (int j=0;j<c.size();j++) {
            c[j].vieTermite(g, i);
        }
        // cout << g;
        // cout << endl;
        // cout << "+++++++++++++++++" << i << endl;
        CHECK(coherence(c, g));
	}

}
TEST_CASE("Colonnie de termite,brindilles et coherence"){
	srand(time(NULL));
	Grille g;
	g.placeBrindille();
	colonnie c=colonnieDeTermites(g);
	//cout<<g;
	CHECK(coherence(c,g));
}
TEST_CASE("Colonnie et deplacement"){
	srand(time(NULL));
	Grille g;
	g.initialiseNid(Coord{9,9},2);
	g.placeBrindille();
	colonnie c=colonnieDeTermites(g);
	
	//cout<<"+++++++++++++++++++++"<<endl;
	//cout<<g;
	CHECK(coherence(c,g));
	
	for(int i=0;i<10000;i++){
		//cout<<"+++++++++++++++++++++"<<i<<endl;
		// Passage par reference pour modifier les termites dans le vecteur.On aurait pu iterer sur les indices
		//cout<<i;
		//cout<<g;
		transmettreBrindille(c);
		for(auto &t:c){
		 	t.vieTermiteNid(g,i,Coord{9,9});
		 }
		 CHECK(coherence(c,g));
		if(i%100==0){
		 	//cout<<g;
		}
	 }
	//cout<<g;
	CHECK(coherence(c,g));
}
Direction affichageDirection(colonnie &c1,Coord c){
	Direction d;
	for(auto t:c1){
		if(t.getPos()==c){
			d=t.getDirection();
			break;
		}
	}
	return d;
}
bool affichageBrindille(colonnie &c1,Coord c){
	bool brindille;
	for(auto t:c1){
		if(t.getPos()==c){
			brindille=t.porteBrindille();
			break;
		}
	}
	return brindille;
}
void affichage(Grille &g,colonnie &c1,colonnie &c2){
	int nid1=c1[0].getNid();
	int nid2=c2[0].getNid();
	for(int i=0;i<tailleGrille;i++){
		for(int j=0;j<tailleGrille;j++){
			Coord courant{i,j};
			if(g.estVide(courant))cout<<"  ";
			else if(g.caseContient(courant)==1000)cout<<"\033[5;33m"<<" #"<<"\033[0m";
			else if(g.contientBrindille(courant)){
				if(g.caseOdeur(courant)==nid1)
				cout<<"\033[31;1m";//"\033[32;1m"<<
				else if(g.caseOdeur(courant)==nid2)
				cout<<"\033[34;1m";
				else{
					cout<<"\033[32;1m";
				}
				cout<<" *"<<"\033[0m";
			}
			else if(g.caseOdeur(courant)==nid1){
				Direction d=affichageDirection(c1,courant);
				bool brindille=affichageBrindille(c1,courant);
				
				
				if(brindille)cout<<"\033[33m";
				else{cout<<"\033[31;1m";}
				if(d==Direction::E or d==Direction::W)cout<<" -";
				if(d==Direction::N or d==Direction::S)cout<<"|*";
				if(d==Direction::NE or d==Direction::SW)cout<<" /";
				if(d==Direction::NW or d==Direction::SE)cout<< "/-";
				cout<<"\033[0m";
			}
			else if(g.caseOdeur(courant)==nid2){
				Direction d=affichageDirection(c2,courant);
				bool brindille=affichageBrindille(c2,courant);
				
			
				if(brindille)cout<<"\033[33m";
				else{cout<<"\033[34;1m";}
				if(d==Direction::E or d==Direction::W)cout<<" -";
				if(d==Direction::N or d==Direction::S)cout<<"|*";
				if(d==Direction::NE or d==Direction::SW)cout<<" /";
				if(d==Direction::NW or d==Direction::SE)cout<<"/-";
				cout<<"\033[0m";
				
			}
		}
		cout<<endl;
	}
}
TEST_CASE("Nid"){
	Grille g;
	g.initialiseNid(Coord{10,0},1);
	//g.initialiseNid(Coord{10,19},2);
	g.placeBrindille();
	colonnie c1=colonnieDeTermitesNid(g,1);
	//colonnie c2=colonnieDeTermitesNid(g,2);
	//colonnie c3=colonnieDeTermitesNid(g,1);
	cout<<"Simulation nid DEBUT"<<endl;
	cout<<g;
	//affichage(g,c1,c2);
	CHECK(coherence(c1,g));
	int gagnant=0;
	for(int i=0;i<1000;i++){
		//cout<<"+++++++++++++++++++++"<<i<<endl;
		// Passage par reference pour modifier les termites dans le vecteur.On aurait pu iterer sur les indices
		//cout<<i<<endl;
		//cout<<g;
		if(gagnant!=0){
			cout<<"Colonnie "<<gagnant<<" ont gagné"<<endl;
			break;
		}
		transmettreBrindille(c1);
		//transmettreBrindille(c2);
		for(auto &t:c1){
		 	t.vieTermiteNid(g,i,Coord{10,0});
			
			if(i>500){
				t.attaqueNid(g,gagnant);
				
			}
			//attaque(g,c2,t);
			//t.vieTermiteNid(g,i,Coord{10,0});
			//t.vieTermiteNid(g,i,Coord{10,0});
		 }
		if(i%2==0){
			 ajouteTermite(Coord{10,0},c1,g);
			 }
		//for(auto &t:c2){
		 //	t.vieTermiteNid(g,i,Coord{10,19});
		//	if(i>500){
		//		t.attaqueNid(g,gagnant);
		//		
		//	}
		//	attaque(g,c1,t);
			
		 //}
		//if(i%2==0){
		//	 ajouteTermite(Coord{10,19},c2,g);
		//	 }
		// for(auto &t:c3){
		//  	t.vieTermiteNid(g,i,Coord{10,0});
		//  }
		 //CHECK(coherence(c1,g));
		if(i%10==0){
			//cout<<"Simulation nid "<<i<<endl;
		 	//cout<<g;
		}
		if(g.caseOdeur(Coord{10,0})!=1){//} or g.caseOdeur(Coord{10,19})!=2){
			cout<<"Simulation "<<i<<endl;
			break;
		}
	 }
	cout<<g;
	//affichage(g,c1,c2);
	//CHECK(coherence(c1,g));
}
TEST_CASE("Avance vers"){
	Grille g;
	g.poseTermite({19,19},1);
	Termite t{1,{19,19}};
	//cout<<g;
	t.marcheAleatoire(g);
	for(int i=0;i<20;i++){
		
		t.avanceVers({0,0},g);
	}
	//cout<<g;
}