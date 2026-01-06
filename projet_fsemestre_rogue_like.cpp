// A. MARIE, T. COINCE, Y. WANG - projet_informatique_rogue_like_jeu - 2025.11.20

// g++ -Iextlib/pdcurses/include -Lextlib/pdcurses/lib projet_v2.0.cpp -lpdcurses -o projet.exe


#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include "lib_projet_ncurses.hpp"

using namespace std;

// Enregistrements
// --------------------------------------------------
// Caractéristiques
struct Caracteristiques {
    string nom;
    char presence;
    vector<string> tab_chaine = {"niveau", "pdv", "force", "vision"};
    vector<int> tab_entier;
};
// Position
struct Position {
    int x, y;
};
// Équipements
struct Equipement {
    string nom;
    char presence;
    vector<int> tab_caracteristique_equipement;
    int x;
    int y;
};
// exemple : {"clé"; 'a'; [0, 0, 0, 0]}; {"potion"; 'd'; [0, 10, 0, 0]}; {"arme"; 'c'; [0, 0, 10, 0]}
// Tableau pour les inventaires
struct Tab_inventaire {
    vector<string> equipement = {"cle mysterieuse", "elixir de sante", "epee vorpale"}; // les noms des equipements
    vector<int> nb_equipement;
};
// Joueur
struct Joueur {
    Caracteristiques caras;
    Position pos;
    Tab_inventaire inventaire;
    bool tuer_boss;
};
// Monstre
struct Monstre {
    Caracteristiques caras;
    Position pos;
};
// Porte
struct Porte {
    char presence;
    bool ouverte;
    Position pos;
};
// --------------------------------------------------
// PREMIER SOUS-ALGORITHME
// Affichage de la carte
// Role : procedure qui lit le fichier de la carte du jeu et affiche dans le terminale.
void affichage_map(string nom_fichier_map) {
    // variables
    int x = 0, y = 0;
    char element_map;
    fstream flux(nom_fichier_map);
    // debut
    if (flux.is_open()) {
        while (flux.get(element_map)) {
            if (element_map != '\n') {
                if (x <= CONSOLE_X_SIZE && y <= CONSOLE_Y_SIZE) {
                    ecrire_char(x, y, element_map);
                    x += 1;
                }
            }
            else {
                y += 1;
                x = 0;
            }
        }
    }
    else {
        cerr << "Erreur : impossible d'ouvrir " << nom_fichier_map << endl;
    }
flux.close();
}
// --------------------------------------------------
// DEUXIEME SOUS-ALGORITHME
void fermer_map(string nom_fichier_map) {
    // variables
    int x = 0, y = 0;
    char element_map;
    fstream flux(nom_fichier_map);
    // debut
    if (flux.is_open()) {
        while (flux.get(element_map)) {
            if (element_map != '\n') {
                if (x <= CONSOLE_X_SIZE && y <= CONSOLE_Y_SIZE) {
                    ecrire_char(x, y, ' ');
                    x += 1;
                }
            }
            else {
                y += 1;
                x = 0;
            }
        }
    }
    else {
        cerr << "Erreur : impossible d'ouvrir " << nom_fichier_map << endl;
    }
flux.close();
}
// --------------------------------------------------
// TROISIEME SOUS-ALGORITHME
void message_joueur(int message){
    if (message == 1){
        ecrire_string("vous bougez", 1, 24);
    }
    if (message == 2){
        ecrire_string("Vous reussissez a tuer le monstre et vous gagnez un niveau.", 1, 24);
    }
    if (message == 3){
        ecrire_string("Vous vous faites battre miserablement contre le monstre, vous perdez 10 points de vie.", 1, 24);
    }
    if (message == 4){
        ecrire_string("Vous trouvez une epee vorpale, votre force augmente de 10.", 1, 24);
    }
    if (message == 5){
        ecrire_string("Vous trouvez un elixir de sante, vous regagnez 10 points de vie.", 1, 24);
    }
    if (message == 6){
        ecrire_string("Vous trouvez une cle mysterieuse vous pouvez ouvrir une porte.", 1, 24);
    }
    if (message == 7){
        ecrire_string("                                                                                                ", 1, 24);
    }
    if (message == 8){
        ecrire_string("Spoiler: les murs gagnent toujours!", 1, 24);
    }
    if (message == 9){
        ecrire_string("La porte semble solidement verrouillee. Une cle doit exister quelque part.", 1, 24);
    }
    if (message == 10){
        ecrire_string("La porte s ouvre lentement devant vous.", 1, 24);
    }
}
// --------------------------------------------------
// QUATRIEME SOUS-ALGORITHME
// Role : determiner la position du joueur.
Joueur initialisation_joueur(string nom_fichier_map) {
    // variables
    int x = 0, y = 0;
    char element_map;
    fstream flux(nom_fichier_map);
    // initialisation
    Joueur player;
    player.caras.presence = '@';
    player.caras.tab_entier = {1, 100, 10, 3};
    player.pos.x = -1;
    player.pos.y = -1;

    // debut
    if (flux.is_open()) {
        while (flux.get(element_map)) {
            if (element_map != '\n') {
                if (x < CONSOLE_X_SIZE && y < CONSOLE_Y_SIZE) {
                    switch (element_map) {
                    case '@':
                        player.caras.presence = '@';
                        player.caras.tab_entier = {1, 100, 10, 3};
                        player.pos.x = x;
                        player.pos.y = y;
                        player.inventaire.nb_equipement = {0, 0, 0};
                        player.tuer_boss = false;
                        break;
                    default:
                        break;
                    }
                x += 1;
                }
            }
            else {
                y += 1;
                x = 0;
            }
        }
    }
    else {
        cerr << "Erreur : impossible d'ouvrir " << nom_fichier_map << endl;
    }
    flux.close();
    return player;
}
// --------------------------------------------------
// CINQUIEME SOUS-ALGORITHME
// Role : determiner la position de chaque monstre dans la carte.
vector<Monstre> initialisation_monstre(string nom_fichier_map) {
    // variables
    int x = 0, y = 0;
    char element_map;
    fstream flux(nom_fichier_map);

    vector<Monstre> list_monster;
    Monstre monster;

    // debut
    if (flux.is_open()) {
        while (flux.get(element_map)) {
            if (element_map != '\n') {
                if (x < CONSOLE_X_SIZE && y < CONSOLE_Y_SIZE) {
                    switch (element_map) {
                    // Monstre de niveau 1 
                    case 'I': case 'J': case 'X': case 'Y':
                        monster.caras.nom = "Goblin";
                        monster.caras.presence = element_map;
                        monster.caras.tab_entier = {1, 0, 5, 2}; // {"niveau", "pdv", "force", "vision"}
                        monster.pos.x = x;
                        monster.pos.y = y;
                        list_monster.push_back(monster);
                        break;
                    // Monstre de niveau 2
                    case 'B': case 'H': case 'K': case 'L': case 'M':
                    case 'N': case 'O': case 'P': case 'Z':
                        monster.caras.nom = "Harpy";
                        monster.caras.presence = element_map;
                        monster.caras.tab_entier = {2, 0, 20, 4}; // {"niveau", "pdv", "force", "vision"}
                        monster.pos.x = x;
                        monster.pos.y = y;
                        list_monster.push_back(monster);
                        break;
                    // Monstre de niveau 3
                    case 'A': case 'C': case 'D': case 'E': case 'W':
                    case 'F': case 'G': case 'Q': case 'V':
                        monster.caras.nom = "Dragon rouge";
                        monster.caras.presence = element_map;
                        monster.caras.tab_entier = {3, 0, 50, 6}; // {"niveau", "pdv", "force", "vision"}
                        monster.pos.x = x;
                        monster.pos.y = y;
                        list_monster.push_back(monster);
                        break;
                    // Boss final
                    case 'R':
                        monster.caras.nom = "Tarrasque";
                        monster.caras.presence = 'R';
                        monster.caras.tab_entier = {4, 0, 70, 8}; // {"niveau", "pdv", "force", "vision"}
                        monster.pos.x = x;
                        monster.pos.y = y;
                        list_monster.push_back(monster);
                        break;
                    default:
                        break;
                    }
                x += 1;    
                }
            }
            else {
                y += 1;
                x = 0;
            }
        }
    }
    else {
        cerr << "Erreur : impossible d'ouvrir " << nom_fichier_map << endl;
    }
    flux.close();
    return list_monster;
}
// --------------------------------------------------
// SIXIEME SOUS-ALGORITHME
bool cle(Joueur Joueur){
    if (Joueur.inventaire.nb_equipement[0] >= 1){
        return true;
    }
    return false;
}
// --------------------------------------------------
// SEPTIEME SOUS-ALGORITHME
// Role : determiner les caracteristiques de chaque objet dans la carte.
vector<Equipement> initialisation_equipement(string nom_fichier_map) {
    // variables
    int x = 0;
    int y = 0;
    char element_map;
    fstream flux(nom_fichier_map);

    vector<Equipement> list_equipement = {};
    Equipement equipement;

    // debut
    if (flux.is_open()) {
        while (flux.get(element_map)) {
            if (element_map != '\n') {
                if (x < CONSOLE_X_SIZE && y < CONSOLE_Y_SIZE) {
                    switch (element_map) {
                    case 'a': case 'b': case 'u': case 'w': 
                        equipement.nom = "cle mysterieuse";
                        equipement.presence = element_map;
                        equipement.x = x;
                        equipement.y = y;
                        equipement.tab_caracteristique_equipement = {1, 0, 0, 0}; // {"niveau", "pdv", "force", "vision"}
                        list_equipement.push_back(equipement);
                        break;
                    case 'd': case 'e': case 'g': 
                    case 'n': case 'o': case 'x':
                        equipement.nom = "elixir de sante";
                        equipement.presence = element_map;
                        equipement.x = x;
                        equipement.y = y;
                        equipement.tab_caracteristique_equipement = {0, 10, 0, 0}; // {"niveau", "pdv", "force", "vision"}
                        list_equipement.push_back(equipement);
                        break;
                    case 'c': case 'k': case 'm': 
                    case 'r': case 's': case 'v': case 'y':
                        equipement.nom = "epee vorpale";
                        equipement.presence = element_map;
                        equipement.x = x;
                        equipement.y = y;
                        equipement.tab_caracteristique_equipement = {0, 0, 10, 0}; // {"niveau", "pdv", "force", "vision"}
                        list_equipement.push_back(equipement);
                        break;
                    default:
                        break;
                    }
                x += 1;    
                }
            }
            else {
                y += 1;
                x = 0;
            }
        }
    }
    else {
        cerr << "Erreur : impossible d'ouvrir " << nom_fichier_map << endl;
    }
    flux.close();
    return list_equipement;
}
// --------------------------------------------------
// HUITIEME SOUS-ALGORITHME
// Role : initialiser les portes.
vector<Porte> initialisation_porte(string nom_fichier_map) {
    // variables
    int x = 0, y = 0;
    char element_map;
    fstream flux(nom_fichier_map);

    vector<Porte> list_door = {};
    Porte door;
    // debut
    if (flux.is_open()) {
        while (flux.get(element_map)) {
            if (element_map != '\n') {
                if (x < CONSOLE_X_SIZE && y < CONSOLE_Y_SIZE) {
                    switch (element_map) {
                    case '1': case '2': case '3':
                    case '4': case '5': case '6':
                    case '7': case '8': case '9':
                        door.presence = element_map;
                        door.ouverte = false;
                        door.pos.x = x;
                        door.pos.y = y;
                        list_door.push_back(door);
                        break;
                    default:
                        break;
                    }
                x += 1;
                }
            }
            else {
                y += 1;
                x = 0;
            }
        }
    }
    else {
        cerr << "Erreur : impossible d'ouvrir " << nom_fichier_map << endl;
    }
    flux.close();
    return list_door;
}
// --------------------------------------------------
// NEUFIEME SOUS-ALGORITHME
// Role : determiner s'il n'y a que '.' autour d'un monstre. C'est le procedure a executer avant le deplacement d'un monstre.
bool est_sol(int x, int y, string nom_fichier_map) {
    // variables
    int current_x = 0, current_y = 0;
    char element_map;
    ifstream flux(nom_fichier_map.c_str());

    // debut
    if (!flux.is_open()) {
        return false;
    }
    else {
        while (flux.get(element_map)) {
            if (element_map != '\n') {
                if (current_x == x && current_y == y) {
                    flux.close();
                    return (element_map == '.');
                }
                current_x += 1;
            }   
                else {
                    current_y += 1;
                    current_x = 0;
                }
            }
            
        }
    flux.close();
    return false;
}
// --------------------------------------------------
// DIXIEME SOUS-ALGORITHME
// Role : fonction determine si une case est une porte et retoune une booleen.
bool est_porte(int x, int y, string nom_fichier_map) {
    // variables
    int current_x = 0, current_y = 0;
    char element_map;
    ifstream flux(nom_fichier_map.c_str());

    // debut
    if (!flux.is_open()) {
        return false;
    }
    else {
        while (flux.get(element_map)) {
            if (element_map != '\n') {
                if (current_x == x && current_y == y) {
                    flux.close();
                    switch (element_map) {
                    case '1': case '2': case '3': case '4': case '5': 
                    case '6': case '7': case '8': case '9':
                        return true;
                        break;
                    default:
                        break;
                    }
                }
                current_x += 1;
            }   
                else {
                    current_y += 1;
                    current_x = 0;
                }
            }
            
        }
    flux.close();
    
    return false;
}
//----------------------------------------------------
// ONZIEME SOUS-ALGORITHME
// Role : determiner si une case est occupee par le mur.
bool est_mur(int x, int y, string nom_fichier_map) {
    // variables
    int current_x = 0, current_y = 0;
    char element_map;
    ifstream flux(nom_fichier_map.c_str());

    // debut
    if (!flux.is_open()) {
        return true;
    }
    else {
        while (flux.get(element_map)) {
            if (element_map != '\n') {
                if (current_x == x && current_y == y) {
                    flux.close();
                    switch (element_map) {
                    case '#':
                        return true;
                        break;
                    default:
                        break;
                    }
                }
                current_x += 1;
            }   
                else {
                    current_y += 1;
                    current_x = 0;
                }
            }
            
        }
    flux.close();
    return false;
}
// --------------------------------------------------
// DOUZIEME SOUS-ALGORITHME
// Role : derterminer si un monstre et le joueur peuvent se voir. C'est la fonction a executer avant la determination de l'attaque.
bool monstre_voit_joueur(string nom_fichier_map, const Monstre& monster, const Joueur& player) {
    // variables
    int i;
    int dx, dy, distance;
    int vision = monster.caras.tab_entier[3];
    ifstream flux(nom_fichier_map.c_str());

    // debut
    if (!flux.is_open()) {
        return false;
    }

    dx = abs(player.pos.x - monster.pos.x);
    dy = abs(player.pos.y - monster.pos.y);
    distance = dx + dy - 1;
    if (distance > vision) {
        return false;
    }

    if (dx == 0) { // le joueur et un monstre dans la meme ligne, x_joueur = x_monstre.
        int y_debut = min(player.pos.y, monster.pos.y);
        int y_fin = max(player.pos.y, monster.pos.y);

        for (i = y_debut + 1; i < y_fin; i++) {
            if (est_porte(monster.pos.x, i, nom_fichier_map) or est_mur(monster.pos.x, i, nom_fichier_map)) {
                flux.close();
                return false;
            }
        }
        flux.close();
        return true;
    }
    if (dy == 0) { // le joueur et un monstre dans la meme colonne, y_joueur = y_monstre.
        int x_debut = min(player.pos.x, monster.pos.x);
        int x_fin = max(player.pos.x, monster.pos.x);

        for (i = x_debut + 1; i < x_fin; i++) {
            if (est_porte(i, monster.pos.y, nom_fichier_map) or est_mur(monster.pos.y, i, nom_fichier_map)) {
                flux.close();
                return false;
            }
        }
        flux.close();
        return true;
    }
    if (est_porte(monster.pos.x, player.pos.y, nom_fichier_map) or est_mur(monster.pos.x, player.pos.y, nom_fichier_map) || est_porte(player.pos.x, monster.pos.y, nom_fichier_map) or est_mur(player.pos.x, monster.pos.y, nom_fichier_map)) {
        return false;
    }
    flux.close();
    return 0;
}
// --------------------------------------------------
// TREIZIEME SOUS-ALGORITHME
// Role : changer le symbol d'un monstre et le sol quand il bouge, n'importe comment il bouge.
void monstre_bouge(Monstre& monstre, int new_x, int new_y) {
    ecrire_char(monstre.pos.x, monstre.pos.y, '.');
    monstre.pos.x = new_x;
    monstre.pos.y = new_y;
    ecrire_char(monstre.pos.x, monstre.pos.y, monstre.caras.presence);
}
// --------------------------------------------------
// QUATORZIEME SOUS-ALGORITHME
// Role : determiner le deplacement d'un monstre vers la direction du joueur.
void deplacement_monstre_vers_joueur(string nom_fichier_map, Monstre& monstre, const Joueur& player) {    
    // variables
    int dx, dy;
    int new_x, new_y;
    // debut
    dx = monstre.pos.x - player.pos.x;
    dy = monstre.pos.y - player.pos.y;

    if (abs(dx) > abs(dy)) { // horizontallement
        if (dx > 0) {
            new_x = monstre.pos.x - 1;
            if (est_porte(new_x, monstre.pos.y, nom_fichier_map) == true && 
                est_mur(new_x, monstre.pos.y, nom_fichier_map)== true &&
                !(new_x == player.pos.x && monstre.pos.y == player.pos.y)) { // monstre a droite du joueur ---> bouger vers gauche.
                    monstre_bouge(monstre, new_x, monstre.pos.y);
                }
        }
        else if (dx < 0) {
            new_x = monstre.pos.x + 1;
            if (est_porte(new_x, monstre.pos.y, nom_fichier_map) == true && 
                est_mur(new_x, monstre.pos.y, nom_fichier_map) == true &&
                !(new_x == player.pos.x && monstre.pos.y == player.pos.y)) { // monstre a gauche du joueur ---> bouger vers droite.
                    monstre_bouge(monstre, new_x, monstre.pos.y);
                }
        }
    }
    else { // verticallement
        if (dy > 0) {
            new_y = monstre.pos.y - 1;
            if (est_porte(monstre.pos.x, monstre.pos.y - 1, nom_fichier_map) == true && 
                est_mur(monstre.pos.x, monstre.pos.y - 1, nom_fichier_map) == true &&
                !(monstre.pos.x == player.pos.x && new_y == player.pos.y)) { // monstre en dessous du joueur
                    monstre_bouge(monstre, monstre.pos.x, new_y);
                }
        }
        else if (dy < 0) {
            new_y = monstre.pos.y + 1;
            if (est_porte(monstre.pos.x, monstre.pos.y + 1, nom_fichier_map) == true && 
                est_mur(monstre.pos.x, monstre.pos.y + 1, nom_fichier_map) == true &&
                !(monstre.pos.x == player.pos.x && new_y == player.pos.y)) { // monstre au dessus du joueur
                    monstre_bouge(monstre, monstre.pos.x, new_y);
                }
        }
    }
}
// --------------------------------------------------
// QUINZIENE SOUS-ALGORITHME
// Role : derterminer le deplacement aleatoire de chaque monstre pour chaque tour (apres le deplacement du joueur).
void deplacement_monstre_aleatoire(string nom_fichier_map, const Joueur player, Monstre& monstre) {
    // variables
    int x = monstre.pos.x;
    int y = monstre.pos.y;
    vector<Position> directions_possibles = {};

    // debut
    if (est_sol(x - 1, y, nom_fichier_map) && !(x - 1 == player.pos.x && y == player.pos.y)) { // vers gauche;
        directions_possibles.push_back({-1, 0});
    }
    if (est_sol(x + 1, y, nom_fichier_map) && !(x + 1 == player.pos.x && y == player.pos.y)) {
        directions_possibles.push_back({1, 0});
    }
    if (est_sol(x, y - 1, nom_fichier_map) && !(x == player.pos.x && y - 1 == player.pos.y)) { // verifier haut.
        directions_possibles.push_back({0, -1});
    }
    if (est_sol(x, y + 1, nom_fichier_map) && !(x == player.pos.x && y + 1 == player.pos.y)) { // verifier bas.
        directions_possibles.push_back({0, 1});
    }
    if (!directions_possibles.empty()) {
        int move_index = rand() % int(directions_possibles.size()); 
        int new_x = x + directions_possibles[move_index].x;
        int new_y = y + directions_possibles[move_index].y;
        monstre_bouge(monstre, new_x, new_y);
    }
}
// --------------------------------------------------
// SEIZIEME SOUS-ALGORITHME
// Role : le fonction qui petmet a un monstre de jeter le joueur quand il le defeat.
void monstre_jette_joueur(string nom_fichier_map, Joueur& player, Monstre& monster) {
    // variables
    vector<Position> directions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}}; // vers haut, vers bas, vers gauche, vers droite. 
    int i_direc = rand() % int(directions.size());
    Position direction_jete = directions[i_direc];

    int jette_cmb_case = rand() % 2 + 3; // 3 ou 4

    int chemin;
    int real_case = 0;
    bool chemin_disponible;
    // debut
    for (chemin = 1; chemin <= jette_cmb_case; chemin ++) {
        int parcour_x = player.pos.x + (direction_jete.x * chemin);
        int parcour_y = player.pos.y + (direction_jete.y * chemin);

        if (est_porte(parcour_x, parcour_y, nom_fichier_map) || est_mur(parcour_x, parcour_y, nom_fichier_map)) {
            chemin_disponible = false;
            break;
        }
        else {
            chemin_disponible = true;
            real_case = chemin;
        }
    }
    if (real_case >= 1) {
        int target_position_x = player.pos.x + (direction_jete.x * real_case);
        int target_position_y = player.pos.y + (direction_jete.y * real_case);
        
        if (chemin_disponible && est_sol(target_position_x, target_position_y, nom_fichier_map)) {
            ecrire_char(player.pos.x, player.pos.y, monster.caras.presence);
            player.pos.x += direction_jete.x * real_case;
            player.pos.y += direction_jete.y * real_case;
            ecrire_char(player.pos.x, player.pos.y, '@');
        }
    }
}
// --------------------------------------------------
// DIX-SEPTIEME SOUS-ALGORITHME
int combat_monstre_joueur(string nom_fichier_map, Monstre& monster, Joueur& player, int& nb_monstre_defeat) {
    // variables
    int force_joueur = player.caras.tab_entier[2];
    int force_monstre = monster.caras.tab_entier[2];

    // debut
    if (force_joueur > force_monstre) { // victoire!
        // cout << "VICTOIRE !" << endl;
        int monster_pos_x = monster.pos.x;
        int monster_pos_y = monster.pos.y;

        player.caras.tab_entier[0] += 1; // niveau +1;
        player.caras.tab_entier[2] += 5; // force +5;
        player.caras.tab_entier[3] += 1; // vision +1;

        monster.caras.tab_entier[1] = -1; // pdv = -1 pour indiquer qu'il est mort.

        ecrire_char(monster.pos.x, monster.pos.y, '.');
        ecrire_char(player.pos.x, player.pos.y, '@');

        nb_monstre_defeat += 1;
        message_joueur(2);
    }
    else {
        // cout << monster.caras.nom << " vous a vaincu ! " << endl;
        int old_pos_x = player.pos.x;
        int old_pos_y = player.pos.y;

        player.caras.tab_entier[1] -= 10; // pdv - 10;
        monstre_jette_joueur(nom_fichier_map, player, monster);
        if (player.pos.x == old_pos_x && player.pos.y == old_pos_y) {
            ecrire_char(old_pos_x, old_pos_y, monster.caras.presence);
        }
        ecrire_char(old_pos_x, old_pos_y, '.');
        ecrire_char(player.pos.x, player.pos.y, '@');
        message_joueur(3);
    }
    ecrire_char(player.pos.x, player.pos.y, '@');
    return nb_monstre_defeat;
}
// --------------------------------------------------
// DIX-HUITIEME SOUS-ALGORITHME
// Role : le procedure qui determine le deplacement des monstres selon les cas differents.
void deplacement_monstre(string nom_fichier_map, vector<Monstre>& liste_monstre, Joueur& player, int& nb_monstre_defeat) {
    // variables
    int i;

    // debut
    for (i = 0; i < int(liste_monstre.size()); i++) {
        if (liste_monstre[i].caras.tab_entier[1] == 0) {
            int memo_x = liste_monstre[i].pos.x;
            int memo_y = liste_monstre[i].pos.y;

            if (monstre_voit_joueur(nom_fichier_map, liste_monstre[i], player)) {
                if (rand() % 2 == 0) { // 50% possibilite que le monstre veut attaquer le joueur quand il le voit.
                    deplacement_monstre_vers_joueur(nom_fichier_map, liste_monstre[i], player);
                }
                else { // le monstre n'attaque pas le joueur quand il le voit.
                    deplacement_monstre_aleatoire(nom_fichier_map, player, liste_monstre[i]);
                }
            }
            else {
                deplacement_monstre_aleatoire(nom_fichier_map, player, liste_monstre[i]);
            }
            
            int new_x = liste_monstre[i].pos.x;
            int new_y = liste_monstre[i].pos.y;

            if ((memo_x != new_x || memo_y != new_y) && 
                liste_monstre[i].caras.tab_entier[1] == 0 &&
                new_x == player.pos.x && 
                new_y == player.pos.y) {

                combat_monstre_joueur(nom_fichier_map, liste_monstre[i], player, nb_monstre_defeat);
                if (liste_monstre[i].caras.tab_entier[1] == -1){
                    liste_monstre.erase(liste_monstre.begin() + i);
                    i -= 1;
                }
            }
        }
    }
}
// ----------------------------------------------------
// DIX-NEUFIEME SOUS-ALGORITHME
int trouver_porte(int x, int y, const vector<Porte>& porte){
    int i;
    for (i=0; i<(int)porte.size(); i++){
        if (porte[i].pos.x == x && porte[i].pos.y ==y ){
            return i;
        }
    }
    return -1;
}
// ----------------------------------------------------
// VINGTIEME SOUS-ALGORITHME
void ouverture_porte( Joueur& Joueur, string nom_fichier){
    Joueur.inventaire.nb_equipement[0]= Joueur.inventaire.nb_equipement[0] -1;   
}
// ----------------------------------------------------
// VINGT-ET-UNIEME SOUS-ALGORITHME
bool est_porte_ouverte(Porte porte){
    if (porte.ouverte == true){
        return true;
    }
    return false;
}
// ----------------------------------------------------
// VINGT-DEUXIEME SOUS-ALGORITHME
// Role : la fonction gerer le déplacement du joueur en fonction d'une entree directionnelle, en verifiant les collisions avec les murs et les portes, et en mettant a jour l'affichage.
void deplacement_joueur(string nom_fichier_map, int input, Joueur& Joueur, vector<Porte>& porte){
    int nx = Joueur.pos.x;
    int ny = Joueur.pos.y;

    if (input == 'z') ny--;
    else if (input == 's') ny++;
    else if (input == 'q') nx--;
    else if (input == 'd') nx++;
    else return;
    int idx = trouver_porte(nx, ny, porte);
    if (est_mur(nx, ny, nom_fichier_map)) {
        message_joueur(7);
        message_joueur(8);
        return;
    }

    // il y a une porte
    if (idx != -1) {
        if (porte[idx].ouverte) {
            ecrire_char(Joueur.pos.x, Joueur.pos.y, '.');
            Joueur.pos.x = nx;
            Joueur.pos.y = ny;
        }
        else if (cle(Joueur)) {
            porte[idx].ouverte = true;
            ouverture_porte(Joueur, nom_fichier_map);
            message_joueur(10);
            saisie_bloquante();
            ecrire_char(Joueur.pos.x, Joueur.pos.y, '.');
            Joueur.pos.x = nx;
            Joueur.pos.y = ny;
            
        }
        else {
            message_joueur(9);
            return; // porte fermée sans clé
        }
    }
    else {
        // sol normal
        ecrire_char(Joueur.pos.x, Joueur.pos.y, '.');
        Joueur.pos.x = nx;
        Joueur.pos.y = ny;
    }
    message_joueur(1);
    message_joueur(7);
}
// ----------------------------------------------------
// VINGT-TROISIEME SOUS-ALGORITHME
void afficher_personnage(Joueur Joueur) {
    ecrire_string("@", Joueur.pos.x, Joueur.pos.y);
}
// ----------------------------------------------------
// VINGT-QUATRIEME SOUS-ALGORITHME
// Role : le procedure affiche les stateristiques du joueur.  
void ecrire_stat_joueur(int x, int y, Joueur j, int nb_monstre_defeat) {
    ecrire_string("Nom:", 1, 23);
    ecrire_string(j.caras.nom, 6, 23);
    ecrire_string("Niveau:", 16, 23);
    ecrire_string(std::to_string(j.caras.tab_entier[0]), 24, 23);
    ecrire_string("Pdv:", 29, 23);
    ecrire_string("    ", 34, 23);
    ecrire_string(std::to_string(j.caras.tab_entier[1]), 34, 23);
    ecrire_string("Force:", 42, 23);
    ecrire_string(std::to_string(j.caras.tab_entier[2]), 49, 23);
    ecrire_string("Vision:", 56, 23);
    ecrire_string(std::to_string(j.caras.tab_entier[3]), 64, 23);
  
//droite
    ecrire_string("--------------------------", 82, 0);
    ecrire_string("        INVENTAIRE ", 82, 1);
    ecrire_string("--------------------------", 82, 2);
    ecrire_string("Cles:", 82, 4);
    ecrire_string("    ", 88, 4);
    ecrire_string(std::to_string(j.inventaire.nb_equipement[0]), 88, 4);
    ecrire_string("Potions:", 82, 6);
    ecrire_string(std::to_string(j.inventaire.nb_equipement[1]), 91, 6);
    ecrire_string("Armes:", 82, 8);
    ecrire_string(std::to_string(j.inventaire.nb_equipement[2]), 89, 8);

    
    ecrire_string("--------------------------", 82, 10);
    ecrire_string("       STATISTIQUES", 82, 11);
    ecrire_string("--------------------------", 82, 12);
    ecrire_string("Monstres vaincus:", 82, 14);
    ecrire_string(std::to_string(nb_monstre_defeat), 100, 14);
    ecrire_string("--------------------------", 82, 16);
    ecrire_string("  CONDITIONS DE VICTOIRE", 82, 17);
    ecrire_string("--------------------------", 82, 18);
    ecrire_string("Boss vaincu:", 82, 20);
    ecrire_string("0 / 1", 95, 20);
    if (j.tuer_boss == true ){
        ecrire_string("1 / 1", 95, 20);
    }
    ecrire_string("Porte sortie ouverte:", 82, 22);
    ecrire_string("0 / 1", 104, 22);
    if (j.pos.x == 71 && j.pos.y == 6){
        ecrire_string("1 / 1", 104, 22);
    }
    
}
// --------------------------------------------------
// VINGT-CINQIEME SOUS-ALGORITHME
void affichage_controls(int x, int y){
    //affichage controls
    ecrire_string("CONTROLS:", 1, 27);
    ecrire_string("P = Remet a zero la partie", 1, 28);
    ecrire_string("A = Ferme la console", 1, 29);
    ecrire_string("Z = Deplacement vers le haut", 1, 30);
    ecrire_string("Q = Deplacement vers la gauche", 1, 31);
    ecrire_string("S = Deplacement vers le bas", 1, 32);
    ecrire_string("D = Deplacement vers la droite", 1, 33);

    //affichage elements jeu
    ecrire_string("ELEMENTS DU JEU:", 37, 27);
    ecrire_string("Cles: a, b, u, w", 37, 28);
    ecrire_string("Potions: d, e, g, n, o, x", 37, 29);
    ecrire_string("Armes: c, k, m, r, s, v, y", 37, 30);

    //affichage niveau monstres
    ecrire_string("NIVEAU MONSTRES:", 70, 27);
    ecrire_string("Niveau 1: I, J, X, Y (Force: 5)", 70, 28);
    ecrire_string("Niveau 2: B, H, K, L, M, N, O, P, Z (Force: 20)", 70, 29);
    ecrire_string("Niveau 3: A, C, D, E, F, G, Q, V (Force: 50)", 70, 30);
    ecrire_string("BOSS: R (Force: 70)", 70, 31);

}
// --------------------------------------------------
// VINGT-SIXIEME SOUS-ALGORITHME
bool joueur_cote_equipement(const Joueur& j, const std::vector<Equipement>& liste) {
    for (const auto& e : liste) {
        int dx = abs(j.pos.x - e.x);
        int dy = abs(j.pos.y - e.y);
        if ((dx + dy) == 1) {
            return true;
        }
    }
    return false;
}
// ------------------------------------------------------
// VINGT-SEPTIEME SOUS-ALGORITHME
bool echec(Joueur& Joueur) {
    if (Joueur.caras.tab_entier[1] == 0) {
        return true;
    }
    return false;
}
// --------------------------------------------------
// VINGT-HUITIEME SOUS-ALGORITHME
bool victoire(Joueur& joueur, string nom_fichier_map) {
    if (joueur.pos.x == 71 && joueur.pos.y == 6 && joueur.tuer_boss == true){
        return true;
    }
    return false;
}
// --------------------------------------------------

// Programme principal
int main() {
    // Variables
    string nom_fichier;
    string fichier_scenar;
    string fichier_controls;
    string fichier_victoire;
    string fichier_defaite;
    int input = ',';

    Joueur player;

    int nb_monstre_defeat = 0;
    
    vector<Monstre> list_monster;
    vector<Equipement> list_equip;
    vector<Porte> list_door;
    
    
    // Debut

    // Affichage utilisateur avant demarrage jeu
    cout << "Nom du fichier de la carte du jeu a ouvrir (Pensez a bien agrandir le terminal en pleine ecran afin d'afficher completement le jeu): ";
    cin >> nom_fichier;

    // test du procedure initialisation_joueur().
    player = initialisation_joueur(nom_fichier);
    // cout << "le joueur est presente comme " << player.caras.presence << " dans la carte." << endl;
    // cout << player.caras.tab_chaine[0] << " : " << player.caras.tab_entier[0] << endl;
    // cout << player.caras.tab_chaine[1] << " : " << player.caras.tab_entier[1] << endl;
    // cout << player.caras.tab_chaine[2] << " : " << player.caras.tab_entier[2] << endl;
    // cout << player.caras.tab_chaine[3] << " : " << player.caras.tab_entier[3] << endl;
    
    // cout << "la position du joueur est : (" << player.pos.x << ',' << player.pos.y << ')' << endl;
    // test du procedure initialisation_monstre().
    cout << "Nommez votre personnage ( Maximum 7 caracteres ) : "; 
    cin >> player.caras.nom;
    assert(player.caras.nom.size() <= 7 && "Le nom depasse 7 caracteres !");

    initialiser_console(0, 0);

    fichier_scenar = "scenario.txt";
    fichier_controls = "controls.txt";
    fichier_victoire = "fichier_victoire.txt";
    fichier_defaite = "fichier_defaite.txt";

    affichage_map(fichier_controls);

    saisie_bloquante();

    fermer_map(fichier_controls);

    affichage_map(fichier_scenar);

    saisie_bloquante();

    fermer_map(fichier_scenar);
    
    ecrire_stat_joueur(1, 23, player, nb_monstre_defeat);
    while (input != 'a' &&  input != KEY_LEFT &&  input != KEY_RIGHT &&  input != KEY_UP  &&  input != KEY_DOWN) {
        
        // Attend le prochain caractère (-> touche appuyée)
        input = saisie_bloquante();

        //Demarrage du jeu en appuyant sur 'p'
        if (input == 'p') {

            affichage_map(nom_fichier); 

            player = initialisation_joueur(nom_fichier);
            
            nb_monstre_defeat = 0;
            list_monster = initialisation_monstre(nom_fichier);
            
            list_equip = initialisation_equipement(nom_fichier);
            list_door = initialisation_porte(nom_fichier);
        }
        // Effacer la console avant de réafficher le contenu
        if (input == 'z' || input == 'q' || input == 's' || input == 'd') {
            int new_x = player.pos.x;
            int new_y = player.pos.y;
            int ind_monstre = -1;
            int ind_equip = -1;
            int int_porte = trouver_porte(new_x, new_y, list_door);
            if (input == 'z') {
                new_y -= 1;
            }
            else if (input == 's') {
                new_y += 1;
            }
            else if (input == 'q') {
                new_x -= 1;
            }
            else if (input == 'd') {
                new_x += 1;
            }         
            for (int i = 0; i < int(list_monster.size()); i++) {
                if (list_monster[i].caras.tab_entier[1] == 0 && list_monster[i].pos.x == new_x && list_monster[i].pos.y == new_y) {
                    ind_monstre = i;
                    break;
                }
            }
            if (ind_monstre != -1) {
                combat_monstre_joueur(nom_fichier, list_monster[ind_monstre], player, nb_monstre_defeat);
                if (list_monster[ind_monstre].caras.tab_entier[1] == -1) {
                    ecrire_char(new_x, new_y, '.');
                    if (list_monster[ind_monstre].caras.nom == "Tarrasque"){
                        player.tuer_boss = true;
                    }
                    list_monster.erase(list_monster.begin() + ind_monstre);
                    ecrire_stat_joueur(1, 23, player,nb_monstre_defeat);
                }
            }
            else if (!est_porte(new_x, new_y, nom_fichier) or !est_mur(new_x, new_y, nom_fichier)) {
                if (int_porte != -1){
                    deplacement_joueur(nom_fichier, input, player, list_door);
                }
                else if (int_porte == -1){
                    deplacement_joueur(nom_fichier, input, player, list_door);
                }    
                afficher_personnage(player);
                ecrire_stat_joueur(1, 23, player,nb_monstre_defeat);
            }
        

            for (int i = 0; i < (int)list_equip.size(); i++) {
                if (list_equip[i].tab_caracteristique_equipement[0] == 1 &&
                    list_equip[i].x == new_x &&
                    list_equip[i].y == new_y) {
                    ind_equip = i;
                    break;
                }
                else if (list_equip[i].tab_caracteristique_equipement[1] == 10 &&
                    list_equip[i].x == new_x &&
                    list_equip[i].y == new_y) {
                    ind_equip = i;
                    break;
                }
                else if (list_equip[i].tab_caracteristique_equipement[2] == 10 &&
                    list_equip[i].x == new_x &&
                    list_equip[i].y == new_y) {
                    ind_equip = i;
                    break;
                }
            }

            if (ind_equip != -1) {
                string nom = list_equip[ind_equip].nom;
                list_equip.erase(list_equip.begin() + ind_equip);

                if (nom == "cle mysterieuse") {
                    player.inventaire.nb_equipement[0] = player.inventaire.nb_equipement[0] + 1;
                    message_joueur(6);
                }
                else if (nom == "elixir de sante") {
                    player.inventaire.nb_equipement[1]++;
                    player.caras.tab_entier[1] = player.caras.tab_entier[1] + 10;
                    message_joueur(5);
                }
                else {
                    player.inventaire.nb_equipement[2]++;
                    player.caras.tab_entier[2] = player.caras.tab_entier[2] + 10;
                    message_joueur(4);
                }
                ecrire_stat_joueur(1, 23, player, nb_monstre_defeat);
            }
            ecrire_stat_joueur(1, 23, player, nb_monstre_defeat);
            affichage_controls(0, 0);
            deplacement_monstre(nom_fichier, list_monster, player, nb_monstre_defeat);

            if (victoire(player, nom_fichier) == true){
                affichage_map(fichier_victoire);
                saisie_bloquante();
                fermer_map(fichier_victoire);
                effacer_console();
                initialiser_console(0, 0);
                ecrire_string("Appuyer sur P pour commencer une nouvelle partie.", 10, 10);
            }

            if (echec(player)== true){
                effacer_console();
                affichage_map(fichier_defaite);
                saisie_bloquante();
                fermer_map(fichier_defaite);
                effacer_console();
                initialiser_console(0, 0);
                ecrire_string("Appuyer sur P pour commencer une nouvelle partie.", 10, 10);
            }
        } 
    }
    fermer_console();
}
