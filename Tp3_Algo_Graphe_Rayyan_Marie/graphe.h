#ifndef FICHIER_H
#define FICHIER_H

typedef struct a *parc_t;

typedef struct s {
    int distance;
    int label; // label du sommet
    parc_t liste_arcs; // arcs sortants du sommet
    struct s *sommet_suivant; // sommet suivant dans le graphe
    int couleur; // couleur du sommet
} sommet_t, *psommet_t;

typedef struct a {
    int poids; // poids de l'arc
    psommet_t dest; // pointeur sommet destinataire
    struct a *arc_suivant; // arc suivant
} arc_t, *parc_t;

typedef psommet_t pgraphe_t;

typedef struct chemin {
    int* sommets; // Tableau de labels des sommets du chemin
    int nombre_sommets; // Nombre de sommets dans le chemin
} chemin_t;

typedef struct lc {
    chemin_t chemin;
    struct lc *suivant;
} *liste_chemins;

// Prototypes de fonction de base du graphe
psommet_t chercher_sommet(pgraphe_t g, int label);
void ajouter_arc(psommet_t o, psommet_t d, int distance);
int nombre_arcs(pgraphe_t g);
int nombre_sommets(pgraphe_t g);
void lire_graphe(char *file_name, pgraphe_t *g);
void ecrire_graphe(psommet_t p);
void ecrire_graphe_colorie(psommet_t p);
int colorier_graphe(pgraphe_t g);
void afficher_graphe_profondeur(pgraphe_t g, int r);
void afficher_graphe_largeur(pgraphe_t g, int r);
void algo_dijkstra(pgraphe_t g, int r);

// Prototypes des nouvelles fonctions
int elementaire(pgraphe_t g, chemin_t c);
int simple(pgraphe_t g, chemin_t c);
int eulerien(pgraphe_t g, chemin_t c);
int hamiltonien(pgraphe_t g, chemin_t c);
int graphe_eulerien(pgraphe_t g);
int graphe_hamiltonien(pgraphe_t g);
int distance(pgraphe_t g, int x, int y);
int excentricite(pgraphe_t g, int n);
int diametre(pgraphe_t g);

#endif // FICHIER_H
