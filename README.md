# Projet Rogue-Like en Terminal
Université de Nantes - Licence 1 Informatique (XLGIIE010) - 2025/2026

===== Équipe =====

T. COINCE - A. MARIE - Y. WANG (E238394Z)

===== Description du Projet =====

Il s'agit d'un jeu de type Rogue s'exécutant dans le terminal. Le joueur contrôle un personnage (@) qui doit se déplacer sur une grille, lue depuis un fichier texte, depuis un point de départ jusqu'à un point d'arrivée ($). Le jeu comprend des éléments tels que des murs(#), des équipements(présenté comme lettre minuscule), des monstres(présenté en lettre majuscule) et des portes(présenté comme les chiffres). L'objectif principal du projet est de mettre en pratique la programmation en C++, la conception modulaire et le travail d'équipe.

===== Fonctionnalités Principales =====
1. Rendu complet dans le terminal : affichage dynamique de la carte, des personnages, de l'interface et de l'état du jeu.
2. Initialisation complexe des entités : lecture de fichiers de configuration pour générer le joueur, les monstres, les équipements et les portes sur la carte.
3. Système d'IA pour les monstres : implémente une IA avec déplacement aléatoire et poursuite agressive basée sur une détection de ligne de vue.
4. Combat au tour par tour : affrontez des monstres dans des combats où la force détermine l'issue, avec un effet visuel de "projection".
5. Système d'inventaire et de progression : collectez des clés, potions et armes pour ouvrir des portes et renforcer le personnage.
6. Portes interactives : trouvez et utilisez des clés pour déverrouiller les portes qui barrent le chemin.

===== Guide de Compilation et d'Exécution =====

===== Prérequis =====

1. Un terminal sous Unix/Linux, macOS, ou Windows.
2. Un compilateur compatible C++(comme g++).
3. Un répertoire qui contient : - controls.txt
                                - scenario.txt
                                - carte.txt
                                - fichier_victoire.txt
                                - fichier_defaite.txt
                                - lib_projet_ncurses(.cpp et .hpp) 
                                - projet_fsemestre_rogue_like.cpp

===== Étapes de Compilation=====

1. Assurez-vous que ce répertoire contient les fichiers de données nécessaires.
2. Ouvrez un terminal et naviguez jusqu'à ce répertoire.
3. Compilez avec la commande suivante : g++ -Iextlib/pdcurses/include -Lextlib/pdcurses/lib projet_fsemestre_rogue_like.cpp -lpdcurses -o projet.exe

===== Lancer le Jeu ===== 

1. Une fois compilé, exécutez le binaire généré : ./projet.exe
2. À l'invite, entrez le nom de votre fichier carte (par exemple, carte_exemple.txt).
3. Entrez le nom de votre personnage (7 caractères maximum).
4. Suivez les instructions à l'écran. Appuyez sur 'P' pour démarrer le jeu.
5. Utilisez Z (Haut), Q (Gauche), S (Bas), D (Droite) pour vous déplacer.
6. Trouvez les clés (a, b, u, w) pour ouvrir les portes numérotées (1-9). Collectez potions et armes pour devenir plus fort !

===== Contributions Principales & Implémentation =====

Ce projet fut un travail d'équipe. Ma contribution principale porte sur la conception de la logique cœur (parsing, instanciation, IA, règles de combat), avec un souci de robustesse et de lisibilité :
1. Monteur/Conception et écriture de la logique pour lire les fichiers carte et instancier correctement toutes les entités du jeu (joueur, monstres, objets, portes) avec leurs caractéristiques.
   Codes inclus : initialisation_joueur(), initialisation_monstre(), initialisation_equipement(), initialisation_porte()	

3. L'IA pour les monstres, incluant la vérification de la vision, la prise de décision du mouvement aléatoire et du movement ciblé quand un monstre voit le joueur, et la coordination des actions pour tous les monstres.
   Codes inclus : deplacement_monstre_aleatoire(), deplacement_monstre_vers_joueur(), monstre_voit_joueur(), deplacement_monstre()

4. Implémentation de la résolution du combat au tour par tour, des modifications de stats et de la mécanique visuelle unique de "projection" pour le joueur vaincu.
   combat_monstre_joueur(), monstre_jette_joueur()	

===== Limites =====

1. Pour des raisons de simplicité en phase de prototype, les fonctions de vérification (est_mur, est_porte) lisent le fichier à chaque appel. Une optimisation évidente serait de stocker la carte en mémoire.
   
2. Correction des conditions de collision (portes/murs) + tests unitaires simples.
