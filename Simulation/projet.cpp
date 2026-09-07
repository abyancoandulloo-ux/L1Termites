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
#include <SFML/Graphics.hpp>
using namespace std;
#include "coord.hpp"
#include "grille.hpp"
#include "termite.hpp"

using colonnie = vector<Termite>;

// Constantes d'affichage
const int CELL_SIZE = 30;          // pixels par case
const int GRID_SIZE = tailleGrille; // 20
const int WINDOW_SIZE = GRID_SIZE * CELL_SIZE;
const int SIMULATION_DELAY = 200;   // ms par étape

// Positions des deux nids (connues du main)
const Coord NID1_POS(10, 0);
const Coord NID2_POS(10, 19);

// Dessine la grille et les positions fixes des nids
void drawGrille(sf::RenderWindow& window, const Grille& g) {
     for (int x = 0; x < GRID_SIZE; ++x) {
        for (int y = 0; y < GRID_SIZE; ++y) {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
            cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
            Coord pos(x, y);
            int contenu = g.caseContient(pos);
            int odeur = g.caseOdeur(pos);

            if (pos.getLig() == NID1_POS.getLig() && pos.getCol() == NID1_POS.getCol()) {
                cell.setFillColor(sf::Color(255, 215, 0)); // Or pour nid 1
            }
            else if (pos.getLig() == NID2_POS.getLig() && pos.getCol() == NID2_POS.getCol()) {
                cell.setFillColor(sf::Color(192, 192, 192)); // Argent pour nid 2
            }
            else if (contenu == -1) { // Brindille
                if (odeur == 1)
                    cell.setFillColor(sf::Color::Red);
                else if (odeur == 2)
                    cell.setFillColor(sf::Color::Blue);
                else
                    cell.setFillColor(sf::Color(160, 82, 45)); // Marron (par défaut)
            }
            else {
                cell.setFillColor(sf::Color(34, 139, 34)); // Vert pour herbe/vide
            }
            window.draw(cell);
        }
    }
}


void drawTermites(sf::RenderWindow& window, const colonnie& c1, const colonnie& c2) {
    sf::CircleShape termite(CELL_SIZE / 3);
    
    // Termites du nid 1 (rouge)
    termite.setFillColor(sf::Color::Red);
    for (const auto& t : c1) {
        Coord pos = t.getPos();
        termite.setPosition(pos.getLig() * CELL_SIZE + CELL_SIZE/3,
                            pos.getCol() * CELL_SIZE + CELL_SIZE/3);
        window.draw(termite);
    }
    
    // Termites du nid 2 (bleu)
    termite.setFillColor(sf::Color::Blue);
    for (const auto& t : c2) {
        Coord pos = t.getPos();
        termite.setPosition(pos.getLig() * CELL_SIZE + CELL_SIZE/3,
                            pos.getCol() * CELL_SIZE + CELL_SIZE/3);
        window.draw(termite);
    }
    
    // Termites du groupe c3 (nid 1 aussi, mais en orange pour les distinguer)
    // termite.setFillColor(sf::Color(255, 165, 0)); // Orange
    // for (const auto& t : c3) {
    //     Coord pos = t.getPos();
    //     termite.setPosition(pos.getLig() * CELL_SIZE + CELL_SIZE/3,
    //                         pos.getCol() * CELL_SIZE + CELL_SIZE/3);
    //     window.draw(termite);
    // }
}

/*Boucle principale (while)
├── Vérifier événements (fermeture)
├── Gérer le timing (clock)
├── Mettre à jour simulation (termites)
├── Effacer fenêtre (clear)
├── Dessiner éléments (draw)
└── Afficher (display)*/

int main() {
    srand(time(NULL));
    
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Simulation de Termites");
    window.setFramerateLimit(60);
    
    cout << "OK" << endl;
    cout << "Simulation SFML - Utilisation des methodes existantes" << endl;
    
    // Initialisation de la grille et des termites
    Grille g;
    g.initialiseNid(NID1_POS, 1);
    g.initialiseNid(NID2_POS, 2);
    g.placeBrindille();
    int gagnant=0;
    colonnie c1 = colonnieDeTermitesNid(g, 1);
    colonnie c2 = colonnieDeTermitesNid(g, 2);
    //colonnie c3 = colonnieDeTermitesNid(g, 1);
    
    cout << "Simulation nid DEBUT" << endl;
    affichage(g,c1,c2);
    cout << g;
    
    int step = 0;
    sf::Clock clock;
    
    while (window.isOpen() && step < 1000) {
        // Gestion des événements (seulement fermeture)
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        // Mise à jour de la simulation
        if (clock.getElapsedTime().asMilliseconds() >= SIMULATION_DELAY) {
			transmettreBrindille(c1);
			transmettreBrindille(c2);
            for (auto& t : c1){
                t.vieTermiteNid(g, step, NID1_POS);
				if(step>500){
				t.attaqueNid(g,gagnant);
				
				}
				attaque(g,c2,t);
			}
            for (auto& t : c2){
                t.vieTermiteNid(g, step, NID2_POS);
				if(step>500){
					t.attaqueNid(g,gagnant);
				
				}
				attaque(g,c1,t);
			}
			if(step%2==0){
				 ajouteTermite(Coord{10,0},c1,g);
				ajouteTermite(Coord{10,19},c2,g);
			 }
            // for (auto& t : c3)
            //     t.vieTermiteNid(g, step, NID1_POS);
             if(g.caseOdeur(Coord{10,0})!=1 or g.caseOdeur(Coord{10,19})!=2){
				cout<<"SIMULATION "<<step<<endl;
				cout<<"FIN DE LA GUERRE DES COLONNIES"<<endl;
				break;
		}
            ++step;
            clock.restart();
            
            if (step % 100 == 0 || step==999) {
                cout << "Simulation nid " << step << endl;
                cout << g;
                cout << "Simulation nid " << step << endl;
                affichage(g,c1,c2);

            }
        }
        
        // Rendu graphique
        window.clear(sf::Color::Black);
        drawGrille(window, g);
        drawTermites(window, c1, c2);
        window.display();
    }
    
    cout << "Simulation terminee!" << endl;
    cout << g;
    affichage(g,c1,c2);
    if (step >= 1000)
        sf::sleep(sf::seconds(3));
    
    return 0;
}