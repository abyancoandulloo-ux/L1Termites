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
#include "termite.hpp"

using colonnie = vector<Termite>;
int main(){
	Grille g;
	g.initialiseNid(Coord{10,0},1);
	g.initialiseNid(Coord{10,19},2);
	g.placeBrindille();
	colonnie c1=colonnieDeTermitesNid(g,1);
	colonnie c2=colonnieDeTermitesNid(g,2);
	//colonnie c3=colonnieDeTermitesNid(g,1);
	cout<<"Simulation nid DEBUT"<<endl;
	cout<<g;
	affichage(g,c1,c2);
	//CHECK(coherence(c1,g));
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
		transmettreBrindille(c2);
		for(auto &t:c1){
		 	t.vieTermiteNid(g,i,Coord{10,0});
			
			if(i>500){
				t.attaqueNid(g,gagnant);
				
			}
			attaque(g,c2,t);
			//t.vieTermiteNid(g,i,Coord{10,0});
			//t.vieTermiteNid(g,i,Coord{10,0});
		 }
		if(i%2==0){
			 ajouteTermite(Coord{10,0},c1,g);
			 }
		for(auto &t:c2){
		 	t.vieTermiteNid(g,i,Coord{10,19});
			if(i>500){
				t.attaqueNid(g,gagnant);
				
			}
			attaque(g,c1,t);
			
		 }
		if(i%2==0){
			 ajouteTermite(Coord{10,19},c2,g);
			 }
		// for(auto &t:c3){
		//  	t.vieTermiteNid(g,i,Coord{10,0});
		//  }
		 //CHECK(coherence(c1,g));
		if(i%10==0){
			//cout<<"Simulation nid "<<i<<endl;
		 	//cout<<g;
		}
		if(g.caseOdeur(Coord{10,0})!=1 or g.caseOdeur(Coord{10,19})!=2){
			cout<<"Simulation "<<i<<endl;
			break;
		}
	 }
	cout<<g;
	affichage(g,c1,c2);
	//CHECK(coherence(c1,g));
	return 0;
}