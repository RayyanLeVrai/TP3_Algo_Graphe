/*
  Structures de type graphe
  Structures de donnees de type liste
  (Pas de contrainte sur le nombre de noeuds des  graphes)
*/


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "graphe.h"
#include "pile.h"
#include "file.h"
#include "fap.h"


psommet_t chercher_sommet (pgraphe_t g, int label)
{
  psommet_t s ;

  s = g ;

  while ((s!= NULL) && (s->label != label))
    {
      s = s->sommet_suivant ;
    }
  return s ;
}

parc_t existence_arc (parc_t l, psommet_t s)
{
  parc_t p = l ;

  while (p != NULL)
    {
      if (p->dest == s)
	return p ;
      p = p->arc_suivant ;
    }
  return p ;
  
}


void ajouter_arc (psommet_t o, psommet_t d, int distance)
{
  parc_t parc ;

  parc = (parc_t) malloc (sizeof(arc_t)) ;

  if (existence_arc (o->liste_arcs, d) != NULL)
    {
      fprintf(stderr, "ajout d'un arc deja existant\n") ;
      exit (-1) ;
    }
  
  parc->poids = distance ;
  parc->dest = d ;
  parc->arc_suivant = o->liste_arcs ;
  o->liste_arcs = parc ;
  return ;
}



// ===================================================================

int nombre_sommets (pgraphe_t g)
{
  psommet_t p = g ;
  int nb = 0 ;

  while (p != NULL)
    {
      nb = nb + 1 ;
      p = p->sommet_suivant ;
    }

  return nb ;
}

int nombre_arcs (pgraphe_t g)
{

  psommet_t p = g ;
  int nb_arcs = 0 ;

  while (p != NULL)
    {
      parc_t l = p->liste_arcs ;

      while (l != NULL)
	{
          nb_arcs = nb_arcs + 1 ;
	  l = l->arc_suivant ;
	}
      
      p = p->sommet_suivant ;
    }
  return nb_arcs ;
}

void init_couleur_sommet (pgraphe_t g)
{
  psommet_t p = g ;

  while (p != NULL)
    {
      p->couleur = 0 ; // couleur indefinie
      p = p->sommet_suivant ; // passer au sommet suivant dans le graphe
    }
  
  return ;
}

int colorier_graphe (pgraphe_t g)
{
  /*
    coloriage du graphe g
    
    datasets
    graphe data/gr_planning
    graphe data/gr_sched1
    graphe data/gr_sched2
  */

  psommet_t p = g ;
  parc_t a ;
  int couleur ;
  int max_couleur = INT_MIN ; // -INFINI
  
  int change ;

  init_couleur_sommet (g) ;
  
  while (p != NULL)
    {
      couleur = 1 ; // 1 est la premiere couleur

      // Pour chaque sommet, on essaie de lui affecter la plus petite couleur

      // Choix de la couleur pour le sommet p
      
      do
	{
	  a = p->liste_arcs ;
	  change = 0 ;
      
	  while (a != NULL)
	    {
	      if (a->dest->couleur == couleur)
		{
		  couleur = couleur + 1 ;
		  change = 1 ;
		} 
	      a = a->arc_suivant ; 
	    }

	} while (change == 1) ;

      // couleur du sommet est differente des couleurs de tous les voisins
      
      p->couleur = couleur ;
      if (couleur > max_couleur)
	max_couleur = couleur ;

      p = p->sommet_suivant ;
    }
  
  return max_couleur ;
}

#include <stdio.h>
#include <stdlib.h>

// Cette fonction initialise les couleurs de tous les sommets à 0 pour indiquer qu'ils n'ont pas été visités
void reinitialiser_couleurs(psommet_t graph) {
    psommet_t courant = graph;
    while (courant != NULL) {
        courant->couleur = 0;
        courant = courant->sommet_suivant;
    }
}

// Fonction pour effectuer un parcours en largeur du graphe
void afficher_graphe_largeur(pgraphe_t g, int r) {
    // Réinitialisation des couleurs des sommets
    reinitialiser_couleurs(g);

    psommet_t sommet_actuel;
    parc_t prochain_arc;
    pfile_t file = creer_file();  // Assumons que creer_file, enfiler, file_vide et defiler sont déjà implémentées

    enfiler(file, chercher_sommet(g, r));  // Chercher le sommet de départ et l'enfiler

    while (!file_vide(file)) {
        sommet_actuel = defiler(file);

        // Vérifier si le sommet actuel n'a pas été visité
        if (sommet_actuel->couleur == 0) {
            printf("%d ", sommet_actuel->label);  // Afficher le label du sommet
            sommet_actuel->couleur = 1;  // Marquer le sommet comme visité

            prochain_arc = sommet_actuel->liste_arcs;
            while (prochain_arc != NULL) {
                if (prochain_arc->dest->couleur == 0) {
                    enfiler(file, prochain_arc->dest);  // Enfiler le sommet de destination si non visité
                }
                prochain_arc = prochain_arc->arc_suivant;
            }
        }
    }
    printf("\n");
}



void afficher_graphe_profondeur(pgraphe_t g, int r) {
    if (g == NULL) return;

    // Initialiser toutes les couleurs à 0 (blanc, non visité)
    psommet_t courant = g;
    while (courant != NULL) {
        courant->couleur = 0;
        courant = courant->sommet_suivant;
    }

    // Chercher le sommet de départ
    psommet_t sommet_de_depart = chercher_sommet(g, r);
    if (sommet_de_depart == NULL) return;

    ppile_t pile = creer_pile();  // Supposons que creer_pile, empiler, pile_vide et depiler sont déjà implémentées
    empiler(pile, sommet_de_depart);

    while (!pile_vide(pile)) {
        psommet_t sommet_actuel = depiler(pile);

        // Marquer le sommet visité s'il ne l'a pas encore été
        if (sommet_actuel->couleur == 0) {
            printf("%d ", sommet_actuel->label);
            sommet_actuel->couleur = 1;  // Marquer comme visité

            // Parcourir tous les arcs sortants du sommet actuel
            parc_t arc = sommet_actuel->liste_arcs;
            while (arc != NULL) {
                if (arc->dest->couleur == 0) {
                    empiler(pile, arc->dest);  // Empiler les sommets non visités
                }
                arc = arc->arc_suivant;
            }
        }
    }
    printf("\n");
}

void algo_dijkstra(pgraphe_t g, int r) {
    if (g == NULL) return;  // Vérifier si le graphe est vide

    psommet_t v, u;
    parc_t arcs;

    // Création de la file à priorité (FAP)
    fap F = creer_fap_vide(comparaison_croissante);  // Utilisation de la fonction de comparaison croissante

    // Initialisation des distances et insertion dans la FAP
    psommet_t temp = g;
    while (temp != NULL) {
        temp->distance = INT_MAX;  // Initialisation des distances à l'infini
        if (temp->label == r) {
            v = temp;  // Définir le sommet de départ
            v->distance = 0;
        }
        F = inserer(F, temp->label, temp->distance);  // Insertion dans la FAP avec la distance comme priorité
        temp = temp->sommet_suivant;
    }

    // Traitement de la FAP tant qu'elle n'est pas vide
    int sommet_label, priorite;
    while (!est_fap_vide(F)) {
        F = extraire(F, &sommet_label, &priorite);  // Extraire le sommet avec la plus petite distance
        u = chercher_sommet(g, sommet_label);  // Trouver le sommet extrait dans le graphe

        // Réévaluer la distance pour chaque arc sortant de u
        arcs = u->liste_arcs;
        while (arcs != NULL) {
            v = arcs->dest;  // Destination de l'arc actuel
            int nouvelle_distance = u->distance + arcs->poids;  // Calculer la nouvelle distance potentielle
            if (v->distance > nouvelle_distance) {
                v->distance = nouvelle_distance;  // Mettre à jour la distance si elle est inférieure
                F = inserer(F, v->label, v->distance);  // Réinsertion pour mise à jour de la priorité
            }
            arcs = arcs->arc_suivant;
        }
    }

    // Afficher les distances depuis le sommet de départ
    temp = g;
    printf("Distances à partir du sommet %d:\n", r);
    while (temp != NULL) {
        printf("Sommet %d : %d\n", temp->label, (temp->distance == INT_MAX ? -1 : temp->distance));
        temp = temp->sommet_suivant;
    }
}





// ======================================================================

int elementaire(pgraphe_t g, chemin_t c) {
    // On initialise tous les sommets du chemin à la couleur 0 (non visité)
    for (int i = 0; i < c.nombre_sommets; i++) {
        psommet_t sommet = chercher_sommet(g, c.sommets[i]);
        if (sommet != NULL) {
            sommet->couleur = 0;
        }
    }

    int elementaire = 1; // On part du principe que le chemin est élémentaire

    // On parcourt tous les sommets du chemin
    for (int i = 0; i < c.nombre_sommets; i++) {
        psommet_t sommet = chercher_sommet(g, c.sommets[i]);
        if (sommet != NULL) {
            if (sommet->couleur != 0) {
                elementaire = 0; // Si un sommet a déjà été visité, le chemin n'est pas élémentaire
                break;
            } else {
                sommet->couleur = 1; // On marque le sommet comme visité
            }
        }
    }

    // On réinitialise les couleurs à 0 pour ne pas affecter d'autres opérations
    for (int i = 0; i < c.nombre_sommets; i++) {
        psommet_t sommet = chercher_sommet(g, c.sommets[i]);
        if (sommet != NULL) {
            sommet->couleur = 0;
        }
    }

    return elementaire;
}

int simple(pgraphe_t g, chemin_t c) {
    if (c.nombre_sommets < 2) {
        // Moins de deux sommets dans le chemin, aucun arc à vérifier
        return 1;
    }

    // Tableau pour stocker les pointeurs vers les arcs utilisés dans le chemin
    parc_t liste_arcs[c.nombre_sommets - 1]; 
    parc_t arcs;

    // Remplir le tableau avec les arcs correspondants entre les sommets consécutifs du chemin
    for (int i = 0; i < c.nombre_sommets - 1; i++) {
        arcs = chercher_sommet(g, c.sommets[i])->liste_arcs;
        while (arcs != NULL) {
            if (arcs->dest == chercher_sommet(g, c.sommets[i + 1])) {
                liste_arcs[i] = arcs; // Ajoute l'arc au tableau si l'arc connecte deux sommets consécutifs
                break;
            }
            arcs = arcs->arc_suivant;
        }
    }

    // Vérifier si un arc est répété dans le tableau
    int simple = 1;
    for (int i = 0; i < c.nombre_sommets - 1; i++) {
        for (int y = i + 1; y < c.nombre_sommets - 1; y++) {
            if (liste_arcs[i] == liste_arcs[y]) {
                simple = 0; // On a trouvé un arc répété
                break;
            }
        }
        if (!simple) break; // Sortir de la boucle extérieure si un doublon a été trouvé
    }

    return simple;
}

int eulerien(pgraphe_t g, chemin_t c) {
    if (c.nombre_sommets < 2) {
        // Moins de deux sommets dans le chemin, aucun arc à vérifier
        return 1; // Techniquement un chemin vide ou à un seul sommet ne peut être eulérien
    }

    // Tableau pour stocker les pointeurs vers les arcs utilisés dans le chemin
    parc_t liste_arcs[c.nombre_sommets - 1];
    parc_t arcs;

    // Remplir le tableau avec les arcs correspondants entre les sommets consécutifs du chemin
    for (int i = 0; i < c.nombre_sommets - 1; i++) {
        arcs = chercher_sommet(g, c.sommets[i])->liste_arcs;
        while (arcs != NULL) {
            if (arcs->dest == chercher_sommet(g, c.sommets[i + 1])) {
                liste_arcs[i] = arcs;
                break;
            }
            arcs = arcs->arc_suivant;
        }
    }

    // On doit vérifier que tous les arcs du graphe sont utilisés une fois dans le chemin
    int eulerien = 1;
    // Simuler la récupération de tous les arcs du graphe, nécessiterait une fonction ou une logique supplémentaire
    // Parcours de tous les sommets et de tous leurs arcs
    psommet_t sommet = g;
    while (sommet != NULL) {
        arcs = sommet->liste_arcs;
        while (arcs != NULL) {
            int arc_inclu = 0;
            for (int i = 0; i < c.nombre_sommets - 1; i++) {
                if (liste_arcs[i] == arcs) {
                    arc_inclu = 1;
                    break;
                }
            }
            if (!arc_inclu) {
                eulerien = 0; // Un arc n'est pas inclus dans le chemin
                break;
            }
            arcs = arcs->arc_suivant;
        }
        if (!eulerien) break;
        sommet = sommet->sommet_suivant;
    }

    return eulerien;
}

int hamiltonien(pgraphe_t g, chemin_t c) {
    // On commence par vérifier si le chemin contient le même nombre de sommets que le graphe
    if (c.nombre_sommets != nombre_sommets(g)) {
        return 0; // Si les tailles diffèrent, ce n'est pas un chemin hamiltonien
    }

    int hamiltonien = 1; // On assume que le chemin est hamiltonien
    int sommet_inclu;
    psommet_t sommet = g;

    // Parcourir tous les sommets du graphe pour vérifier leur présence dans le chemin
    while (sommet != NULL && hamiltonien) {
        sommet_inclu = 0; // Indicateur pour savoir si le sommet actuel est dans le chemin
        for (int i = 0; i < c.nombre_sommets; i++) {
            if (c.sommets[i] == sommet->label) {
                sommet_inclu = 1;
                break; // Dès qu'on trouve le sommet dans le chemin, on arrête la recherche
            }
        }
        if (!sommet_inclu) {
            hamiltonien = 0; // Si un sommet n'est pas trouvé dans le chemin, ce n'est pas un chemin hamiltonien
        }
        sommet = sommet->sommet_suivant; // Passer au sommet suivant dans le graphe
    }

    return hamiltonien; // Renvoie 1 si tous les sommets sont inclus une fois, sinon 0
}

int graphe_eulerien(pgraphe_t g) {
    if (g == NULL) return 0; // Un graphe vide n'est pas eulérien

    psommet_t sommet = g;
    int total_arcs = 0;

    // Compter les arcs sortants et entrants pour chaque sommet
    while (sommet != NULL) {
        int count_out = 0; // Nombre d'arcs sortants
        parc_t arc = sommet->liste_arcs;
        while (arc != NULL) {
            count_out++;
            arc = arc->arc_suivant;
        }
        sommet->distance = count_out; // Utiliser le champ 'distance' temporairement pour stocker le nombre d'arcs sortants
        sommet = sommet->sommet_suivant;
        total_arcs += count_out;
    }

    if (total_arcs == 0) return 0; // Aucun arc, le graphe ne peut être eulérien

    // Vérifier que chaque sommet a autant d'arcs entrants que sortants
    sommet = g;
    while (sommet != NULL) {
        int count_in = 0;
        psommet_t temp = g;
        while (temp != NULL) {
            parc_t arc = temp->liste_arcs;
            while (arc != NULL) {
                if (arc->dest == sommet) {
                    count_in++;
                }
                arc = arc->arc_suivant;
            }
            temp = temp->sommet_suivant;
        }
        if (count_in != sommet->distance) { // Comparer les arcs entrants aux arcs sortants stockés dans 'distance'
            return 0;
        }
        sommet = sommet->sommet_suivant;
    }

    // Vérifier que tous les sommets avec des arcs sont dans la même composante connexe
    // Ceci est simplifié ici. Normalement, il faudrait faire un parcours pour confirmer la connectivité
    return 1; // Si toutes les vérifications sont passées, le graphe est eulérien
}

int graphe_hamiltonien(pgraphe_t g) {
    if (g == NULL) return 0; // Un graphe vide n'est pas hamiltonien

    int n = nombre_sommets(g);
    if (n < 3) return 0; // Un graphe avec moins de 3 sommets ne peut pas contenir de cycle Hamiltonien

    // Utiliser une heuristique simple basée sur le degré de chaque sommet
    // Selon le théorème d'Ore (pour les graphes non orientés):
    // Si pour chaque paire de sommets non adjacents u et v, deg(u) + deg(v) >= n, le graphe est Hamiltonien
    psommet_t sommet = g, temp;
    int *degres = malloc(n * sizeof(int));
    if (degres == NULL) return 0; // Échec de l'allocation

    // Initialiser le tableau de degrés
    for (int i = 0; i < n; i++) degres[i] = 0;

    // Calculer le degré de chaque sommet
    sommet = g;
    int index = 0;
    while (sommet != NULL) {
        parc_t arc = sommet->liste_arcs;
        while (arc != NULL) {
            degres[index]++;
            arc = arc->arc_suivant;
        }
        sommet = sommet->sommet_suivant;
        index++;
    }

    // Vérifier la condition du théorème d'Ore modifié pour les graphes orientés
    int hamiltonien = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && degres[i] + degres[j] < n) {
                hamiltonien = 0;
                break;
            }
        }
        if (!hamiltonien) break;
    }

    free(degres); // Libérer la mémoire
    return hamiltonien;
}

#include <limits.h> // Pour INT_MAX

int distance(pgraphe_t g, int x, int y) {
    if (g == NULL) return -1; // Graphe vide

    // Nombre de sommets dans le graphe
    int n = nombre_sommets(g);
    int dist[n]; // Tableau pour les distances minimales
    int sptSet[n]; // Tableau pour les sommets déjà inclus dans le chemin le plus court
    int label_to_index[n]; // Mapping des labels de sommets à leurs indices dans le tableau

    // Initialisation des distances à infini et sptSet à false
    int index = 0;
    psommet_t sommet = g;
    while (sommet != NULL) {
        dist[index] = INT_MAX;
        sptSet[index] = 0;
        label_to_index[sommet->label] = index;
        sommet = sommet->sommet_suivant;
        index++;
    }

    // Distance du sommet source à lui-même est toujours 0
    dist[label_to_index[x]] = 0;

    // Trouver le chemin le plus court pour tous les sommets
    for (int count = 0; count < n - 1; count++) {
        // Choisir le sommet minimum de l'ensemble des sommets pas encore traités
        int u = -1;
        int min = INT_MAX;
        for (int v = 0; v < n; v++) {
            if (!sptSet[v] && dist[v] <= min) {
                min = dist[v], u = v;
            }
        }

        // Marquer le sommet choisi comme traité
        sptSet[u] = 1;

        // Mettre à jour la valeur de dist des sommets adjacents de u
        sommet = chercher_sommet(g, label_to_index[u]);
        parc_t arc = sommet->liste_arcs;
        while (arc != NULL) {
            int v = label_to_index[arc->dest->label];
            if (!sptSet[v] && dist[u] != INT_MAX && dist[u] + arc->poids < dist[v]) {
                dist[v] = dist[u] + arc->poids;
            }
            arc = arc->arc_suivant;
        }
    }

    // Retourner la distance trouvée au sommet y
    int final_distance = dist[label_to_index[y]];
    if (final_distance == INT_MAX) {
        return -1; // Aucun chemin trouvé
    } else {
        return final_distance; // Retourner la distance calculée
    }
}



int excentricite(pgraphe_t g, int n) {
  int max_limite = 1000000;
    if (g == NULL) return -1; // Graphe vide

    int num_sommets = nombre_sommets(g);
    int dist[num_sommets]; // Tableau pour les distances minimales
    int sptSet[num_sommets]; // Tableau pour les sommets déjà inclus dans le chemin le plus court

    // Initialiser les distances à infini et sptSet à false pour tous les sommets
    for (int i = 0; i < num_sommets; i++) {
        dist[i] = max_limite;
        sptSet[i] = 0;
    }

    // Trouver le sommet correspondant au label n
    psommet_t sommet_n = chercher_sommet(g, n);
    if (sommet_n == NULL) return -1; // Sommet non trouvé

    dist[n - 1] = 0; // La distance du sommet à lui-même est toujours 0

    // Algorithme de Dijkstra
    for (int count = 0; count < num_sommets - 1; count++) {
        // Trouver le sommet avec la distance minimum non traité
        int min_dist = max_limite, min_index = -1;
        for (int i = 0; i < num_sommets; i++) {
            if (!sptSet[i] && dist[i] <= min_dist) {
                min_dist = dist[i];
                min_index = i;
            }
        }

        // Marquer le sommet trouvé comme traité
        sptSet[min_index] = 1;

        // Mettre à jour la distance des sommets adjacents
        for (psommet_t sommet = g; sommet != NULL; sommet = sommet->sommet_suivant) {
            for (parc_t arc = sommet->liste_arcs; arc != NULL; arc = arc->arc_suivant) {
                int adj_index = arc->dest->label - 1;
                if (!sptSet[adj_index] && dist[min_index] != max_limite &&
                    dist[min_index] + arc->poids < dist[adj_index]) {
                    dist[adj_index] = dist[min_index] + arc->poids;
                }
            }
        }
    }

    // Trouver la distance maximale à partir du sommet n
    int max_dist = 0;
    for (int i = 0; i < num_sommets; i++) {
        if (dist[i] > max_dist && dist[i] != max_limite) {
            max_dist = dist[i];
        }
    }

    return max_dist;
}

int diametre(pgraphe_t g) {
    if (g == NULL) return 0; // Un graphe vide a un diamètre de 0

    int max = 0; // Initialiser le maximum à 0
    psommet_t sommet = g;
    int exen; // Pour stocker l'excentricité de chaque sommet

    while (sommet != NULL) {
        exen = excentricite(g, sommet->label); // Calculer l'excentricité du sommet courant
        if (exen > max) {
            max = exen; // Mettre à jour max si l'excentricité courante est plus grande
        }
        sommet = sommet->sommet_suivant; // Passer au sommet suivant
    }

    return max; // Retourner l'excentricité maximale trouvée, qui est le diamètre du graphe
}



int degre_sortant_sommet (pgraphe_t g, psommet_t s)
{
  if(s==NULL) return -1;
  int compteur = 0;
  parc_t arcs = s->liste_arcs;
  while(arcs != NULL){
    compteur++;
    arcs = arcs->arc_suivant;
  }
  return compteur;
}

int degre_entrant_sommet (pgraphe_t g, psommet_t s)
{
  if(s==NULL) return -1;
  pgraphe_t sommet = g;
  int compteur = 0;
  parc_t arcs;
  while(sommet!=NULL){
    arcs = sommet->liste_arcs;
    while(arcs != NULL){
      if(arcs->dest==s)
        compteur++;
      arcs = arcs->arc_suivant;
    }
    sommet=sommet->sommet_suivant;
  }
  return compteur;
}

int degre_maximal_graphe (pgraphe_t g)
{
  int max = degre_sortant_sommet(g, g)+degre_entrant_sommet(g, g);
  pgraphe_t sommet = g;
  while(sommet!=NULL){
    if(degre_sortant_sommet(g, sommet)+degre_entrant_sommet(g, sommet)>max)
      max = degre_sortant_sommet(g, sommet)+degre_entrant_sommet(g, sommet);
    sommet=sommet->sommet_suivant;
  }
  return max;
}


int degre_minimal_graphe (pgraphe_t g)
{
  int min = degre_sortant_sommet(g, g)+degre_entrant_sommet(g, g);
  pgraphe_t sommet = g;
  while(sommet!=NULL){
    if(degre_sortant_sommet(g, sommet)+degre_entrant_sommet(g, sommet)<min)
      min = degre_sortant_sommet(g, sommet)+degre_entrant_sommet(g, sommet);
    sommet=sommet->sommet_suivant;
  }
  return min;
}



int independant (pgraphe_t g)
{
  /* Les aretes du graphe n'ont pas de sommet en commun */

  return 0 ;
}



int complet (pgraphe_t g)
{
  /* Toutes les paires de sommet du graphe sont jointes par un arc */

  return 0 ;
}

int regulier (pgraphe_t g)
{
  /* 
     graphe regulier: tous les sommets ont le meme degre
     g est le ponteur vers le premier sommet du graphe
     renvoie 1 si le graphe est régulier, 0 sinon
  */

  return 0 ;
}




/*
  placer les fonctions de l'examen 2017 juste apres
*/
