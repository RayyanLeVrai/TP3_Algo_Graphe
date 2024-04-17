#include <stdlib.h>
#include <stdio.h>
#include "file.h"

pfile_t creer_file ()
{
  pfile_t file = malloc(sizeof(file_t));
  file->tete=0;
  file->queue=0;
  return file;
}

int detruire_file (pfile_t f)
{
  free(f);
  return 0;
}

int file_vide (pfile_t f)
{
  if(f==NULL)printf("error\n");
  return (f->tete==f->queue);
}

int file_pleine (pfile_t f)
{
  return (f->tete!=f->queue && f->tete%MAX_FILE_SIZE==f->queue);
}

psommet_t defiler (pfile_t f)
{
  if(f==NULL)return NULL;
  psommet_t var = f->Tab[f->queue];
  f->queue = (f->queue+1)%MAX_FILE_SIZE;
  if(f->queue==0)f->tete=f->tete%MAX_FILE_SIZE;
  return var;
}

int enfiler (pfile_t f, psommet_t p)
{
  f->Tab[f->tete%MAX_FILE_SIZE]=p;
  f->tete++;
  return 0;
}
