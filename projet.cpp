// #include <cstdlib>
// #include <ctime>
// #include <iostream>
// #include <stdexcept>
// #include <string>
// #include <vector>
// #include <sstream>
// #include <ostream>
// #include <array>
// using namespace std;
// #include "coord.hpp"
// #include "grille.hpp"
// #include "doctest.h"

// int main(){
// 	srand(time(nullptr));

// 	Grille g;
	
// 	// Placer 5 brindilles aléatoirement
// 	for (int b = 0; b < 5; b++) {
// 	    int lig, col;
// 	    do {
// 	        lig = rand() % tailleGrille;
// 	        col = rand() % tailleGrille;
// 	    } while (!g.estVide(Coord{lig, col}));
// 	    g.poseBrindille(Coord{lig, col});
// 	}
	
// 	// Placer 3 termites aléatoirement
// 	for (int t = 0; t < 3; t++) {
// 	    int lig, col;
// 	    do {
// 	        lig = rand() % tailleGrille;
// 	        col = rand() % tailleGrille;
// 	    } while (!g.estVide(Coord{lig, col}));
// 	    g.poseTermite(Coord{lig, col}, t + 1);
// 	}
	
// 	cout << g;
// 	return 0;
// }
// -*- coding: utf-8 -*-
/**
 * Simulation graphique SFML du comportement des termites.
 * Reproduit la logique du main.cpp original avec deux colonies,
 * attaques après 500 tours, ajout périodique de termites, etc.
 */

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>
#include <SFML/Graphics.hpp>

#include "coord.hpp"
#include "grille.hpp"
#include "termite.hpp"

using namespace std;
using colonnie = vector<Termite>;

// ========== Constantes d'affichage ==========
const int CELL_SIZE = 30;           // pixels par case
const int GRID_SIZE = tailleGrille; // 20
const int WINDOW_SIZE = GRID_SIZE * CELL_SIZE;
const int SIMULATION_DELAY = 500;    // ms entre chaque étape

// Positions fixes des nids
const Coord NID1_POS(10, 0);
const Coord NID2_POS(10, 19);

// ========== Fonctions de dessin ==========

/**
 * Dessine la grille : cases, brindilles, nids.
 */
void drawGrille(sf::RenderWindow& window, const Grille& g) {
    for (int x = 0; x < GRID_SIZE; ++x) {
        for (int y = 0; y < GRID_SIZE; ++y) {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
            cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
            Coord pos(x, y);
            int contenu = g.caseContient(pos);
            int odeur = g.caseOdeur(pos);   // récupération de l'odeur

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
                cell.setFillColor(sf::Color(34, 139, 34)); // Vert pour herbe / vide
            }
            window.draw(cell);
        }
    }
}

/**
 * Dessine les termites (cercle) avec couleur selon leur colonie.
 */
void drawTermites(sf::RenderWindow& window, const colonnie& c1, const colonnie& c2) {
    sf::CircleShape termite(CELL_SIZE / 3);

    // Termites du nid 1 (rouge)
    termite.setFillColor(sf::Color::Red);
    for (const auto& t : c1) {
        Coord pos = t.getPos();
        termite.setPosition(pos.getLig() * CELL_SIZE + CELL_SIZE / 3,
                            pos.getCol() * CELL_SIZE + CELL_SIZE / 3);
        window.draw(termite);
    }

    // Termites du nid 2 (bleu)
    termite.setFillColor(sf::Color::Blue);
    for (const auto& t : c2) {
        Coord pos = t.getPos();
        termite.setPosition(pos.getLig() * CELL_SIZE + CELL_SIZE / 3,
                            pos.getCol() * CELL_SIZE + CELL_SIZE / 3);
        window.draw(termite);
    }
}

/**
 * Affiche les statistiques (étape, effectifs) et les commandes.
 */
void drawStats(sf::RenderWindow& window, int step, const colonnie& c1,
               const colonnie& c2, bool isPaused, int winner) {
    sf::Font font;
    // Tentative de chargement d'une police système
    if (!font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf") &&
        !font.loadFromFile("C:/Windows/Fonts/Arial.ttf")) {
        return; // Pas de police, on n'affiche pas le texte
    }

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::White);

    stringstream ss;
    if (winner == 0) {
        ss << "Step: " << step << "\n"
           << "Nid 1 termites: " << c1.size() << "\n"
           << "Nid 2 termites: " << c2.size() << "\n";
        if (isPaused)
            ss << "\nPAUSE - Appuyez sur ESPACE";
        else
            ss << "\nESPACE: Pause   R: Reset   ESC: Quitter";
    } else {
        ss << "Colonie " << winner << " a gagne !\n"
           << "Appuyez sur R pour rejouer, ESC pour quitter.";
    }

    text.setString(ss.str());
    text.setPosition(10, 10);

    sf::RectangleShape bg(sf::Vector2f(260, 120));
    bg.setFillColor(sf::Color(0, 0, 0, 180));
    bg.setPosition(5, 5);
    window.draw(bg);
    window.draw(text);
}

// ========== Fonction principale ==========

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE),
                            "Simulation de Termites - Deux Colonies");
    window.setFramerateLimit(60);

    // Variables d'état de la simulation
    Grille g;
    colonnie c1, c2;
    int step = 0;
    int winner = 0;          // 0 = aucun, 1 ou 2 = colonie gagnante
    bool isPaused = false;
    bool simulationActive = true;
    sf::Clock clock;

    // Fonction pour réinitialiser complètement la simulation
    auto resetSimulation = [&]() {
        g = Grille();
        g.initialiseNid(NID1_POS, 1);
        g.initialiseNid(NID2_POS, 2);
        g.placeBrindille();
        c1 = colonnieDeTermitesNid(g, 1);
        c2 = colonnieDeTermitesNid(g, 2);
        step = 0;
        winner = 0;
        simulationActive = true;
        isPaused = false;
        cout << "--- Simulation reinitialisee ---" << endl;
    };

    resetSimulation(); // initialisation

    // Boucle principale SFML
    while (window.isOpen() && simulationActive) {
        // Gestion des événements
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    isPaused = !isPaused;
                } else if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                } else if (event.key.code == sf::Keyboard::R) {
                    resetSimulation();
                }
            }
        }

        // Mise à jour de la simulation (si non en pause et pas de vainqueur)
        if (!isPaused && winner == 0){// && step < 10000) {
            if (clock.getElapsedTime().asMilliseconds() >= SIMULATION_DELAY) {
                // --- Logique de simulation identique au main.cpp original ---
                transmettreBrindille(c1);
                transmettreBrindille(c2);

                // Mouvement des termites de la colonie 1
                for (auto& t : c1) {
                    t.vieTermiteNid(g, step, NID1_POS);
                    if (step > 5000) {
                        t.attaqueNid(g, winner);
                    }
                    attaque(g, c2, t); // attaque contre colonie 2
                }
                // Ajout périodique de termites dans le nid 1
                if (step % 2 == 0) {
                    ajouteTermite(NID1_POS, c1, g);
                }

                // Mouvement des termites de la colonie 2
                for (auto& t : c2) {
                    t.vieTermiteNid(g, step, NID2_POS);
                    if (step > 5000) {
                        t.attaqueNid(g, winner);
                    }
                    attaque(g, c1, t); // attaque contre colonie 1
                }
                // Ajout périodique de termites dans le nid 2
                if (step % 2 == 0) {
                    ajouteTermite(NID2_POS, c2, g);
                }

                // Vérification de la destruction d'un nid (odeur perdue)
                if (g.caseOdeur(NID1_POS) != 1 || g.caseOdeur(NID2_POS) != 2) {
                    if (g.caseOdeur(NID1_POS) == 2)
                        winner = 2;
                    else if (g.caseOdeur(NID2_POS) == 1)
                        winner = 1;
                }

                ++step;
                clock.restart();

                // Affichage console périodique
                if (step % 100 == 0 || step == 199 || step == 999) {
                    cout << "Step " << step << endl;
                    cout << g;
                }
            }
        }

        // Rendu graphique
        window.clear(sf::Color::Black);
        drawGrille(window, g);
        drawTermites(window, c1, c2);
        drawStats(window, step, c1, c2, isPaused, winner);
        window.display();

        // Si un vainqueur est déclaré, on désactive la simulation mais on continue d'afficher
        if (winner != 0 && simulationActive) {
            simulationActive = false; // plus de mise à jour, mais affichage statique
            cout << "Colonie " << winner << " a gagne!" << endl;
        }
    }

    cout << "Simulation terminee." << endl;
    return 0;
}