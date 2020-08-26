#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <dirent.h>

typedef struct curseur{
  int ligne;
  int colonne;
} curseur;

typedef struct ligne{
  size_t taille;
  int num;
  int vrnum;
  int vrcol;
  struct ligne * prev;
  struct ligne * next;
  char* txt;
} ligne;

typedef struct buffer{
  curseur* curs;
  char* nom;
  ligne * premiere;
  int taille;

  struct buffer * prev;
  struct buffer * next;
} buffer;

buffer* buffer_courant;
curseur* selection;
int x,y,plig,end,save,load,close,selec,lw,pcol,yl;
char * OPT[5];
int select2;

#include "ecriture.h"
#include "interface.h"
#include "controleur.h"
#include "gestion.h"
