#include "fap.h"
#include <unistd.h>
#include <stdlib.h>

fap creer_fap_vide(int (*p_cmp)(int, int))
{
  fap resultat;

  resultat.tete = NULL;
  resultat.comparaison = p_cmp;
  return resultat;
}

fap inserer(fap f, int element, int priorite)
{
  struct maillon *nouveau, *courant, *precedent;

  nouveau = (struct maillon *) malloc(sizeof(struct maillon));
  nouveau->element = element;
  nouveau->priorite = priorite;
  if ((f.tete == NULL) || (f.comparaison(priorite, f.tete->priorite)==-1))
    {
      nouveau->prochain = f.tete;
      f.tete = nouveau;
    }
  else
    {
      precedent = f.tete;
      courant = precedent->prochain;
      while ((courant != NULL) && (f.comparaison(priorite, f.tete->priorite)==1))
        {
          precedent = courant;
          courant = courant->prochain;
        }
      precedent->prochain = nouveau;
      nouveau->prochain = courant;
    }
  return f;
}
  
fap extraire(fap f, int *element, int *priorite)
{
  struct maillon *courant;

  if (f.tete != NULL)
    {
      courant = f.tete;
      *element = courant->element;
      *priorite = courant->priorite;
      f.tete = courant->prochain;
      free(courant);
    }
  return f;
}

int est_fap_vide(fap f)
{
  return f.tete == NULL;
}

void
detruire_fap(fap f)
{
  int element, priorite;

  while (!est_fap_vide(f))
      f = extraire(f,&element,&priorite);
}

int comparaison_croissante(int a, int b) {
  if (a<b) {
    return -1;
  } else if (a==b) {
    return 0;
  } else if (a>b) {
    return 1;
  } else {
    return -1;
  }
}

int comparaison_decroissante(int a, int b) {
  if (a>b) {
    return -1;
  } else if (a==b) {
    return 0;
  } else if (a<b) {
    return 1;
  } else {
    return -1;
  }
}