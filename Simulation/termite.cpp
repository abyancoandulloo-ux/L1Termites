// -*- coding: utf-8 -*-
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
		//}
		tunnel(g);
		tourneAleat();
		avance(g);
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
	if(pos.getLig()!=0 and pos.getCol()!=0 and pos.getLig()!=tailleGrille-1 and pos.getCol()!=tailleGrille-1 and !brindille){
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
					if(!iter.porteBrindille() and distance(iter.getPos(),courant.getPos())<=sqrt(2)){
						courant.setBrindille(false);
						iter.setBrindille(true);
					}
				}
		}
	}

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

