#include <stdlib.h>
#include <stdio.h>
#include "pile.h"

ppile_t creer_pile()
{
  ppile_t new = malloc(sizeof(pile_t));
  new->sommet = 0;
  return new;
}

int detruire_pile(ppile_t p)
{
  free(p->Tab);
  return 0;
}

//return 1 si la pile est vide, 0 sinon
int pile_vide(ppile_t p)
{
  return p->sommet == 0;
}

//return 1 si la pile est pleine 0 sinon
int pile_pleine(ppile_t p)
{
  return p->sommet == MAX_PILE_SIZE;
}

//on test si la pile est vide
psommet_t depiler(ppile_t p)
{
  if (!pile_vide(p))
  {
    psommet_t res=p->Tab[p->sommet-1];
    p->Tab[p->sommet-1]=NULL;
    p->sommet--;
    return res;
  }
  else
  {
    printf("La pile est vide!\n");
    return NULL;
  }
}

//on test si la pile est pleine
int empiler(ppile_t p, psommet_t pn)
{
  if (!pile_pleine(p))
  {
    p->Tab[p->sommet] = pn; //bug
    p->sommet++;
    return 1;
  }
  else
  {
    printf("La pile est pleine!\n");
    return 0;
  }
}

void afficher_pile(ppile_t p)
{
  for (int i = 0; i < MAX_PILE_SIZE; i++)
  {
    if (i>=p->sommet){
      printf("NULL ");
    }
    else{
    printf("%d ", p->Tab[i]->label);
    }
  }
  printf("\n\n");
}

