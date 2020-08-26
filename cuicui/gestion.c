#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <dirent.h>

#include"infos.h"


//appelée par ouvrirFichier pour remplir le buffer des lignes suivantes si nécessaire
ligne* remplirBuffer(FILE * f, int num, ligne* prev,int vrnum,int vrcol) {
  buffer_courant->taille++;
  ligne* l = malloc(sizeof(ligne));
  l->num = num;
  l->prev = prev;
  l->vrnum = vrnum;
  l->vrcol = vrcol;
  int taille = 200;
  l->txt = malloc(taille);
  l->taille = 200;
  int i = 0;
  char c = fgetc(f);
  char nbc = 0;
  if (c == '\n')
    memset(l->txt,'\0',200);
  while(c!='\n' && c!=EOF) {
    nbc++;
    if (i>=taille) {
      l->txt = realloc(l->txt,taille*2);
      taille*=2;
      l->taille = taille;
    }
    *((l->txt)+i) = c;
    if(lw && nbc>=x) {
      l->next = remplirBuffer(f,num+1,l,vrnum,vrcol+1);
      return l;
    }
    c = fgetc(f);
    i++;
  }
  if (i>=taille) {
    l->txt = realloc(l->txt,taille*2);
    taille*=2;
    l->taille = taille;
  }
  *((l->txt)+i)='\0';
  if(c != EOF && c!= '\0')
    l->next = remplirBuffer(f,num+1,l,vrnum+1,0);
  else {
    l->next = NULL;
    buffer_courant->curs->ligne = num;
    buffer_courant->curs->colonne = nbc;
    if(nbc == 0) {
      buffer_courant->taille--;
      return NULL;
    }
  }
  refresh();
  return l;
}

buffer * nouveauBuffer(char *nom) {
  buffer *b = malloc(sizeof(buffer));
  ligne *l = malloc(sizeof(ligne));
  b->nom = malloc(sizeof(char)*strlen(nom));
  strcpy(b->nom, nom);
  l->prev = NULL;
  l->next = NULL;
  l->txt = malloc(sizeof(char)*100);
  l->taille = 100;
  strcpy(l->txt," \0");
  l->num = 1;
  b->premiere = l;
  b->taille = 1;
  curseur *c = malloc(sizeof(curseur));
  c->ligne = 1;
  c->colonne = -1;
  b->curs = c;
  b->prev = buffer_courant;
  buffer_courant->next = b;
  b->next = b;
  return b;
}


//remplit le buffer des lignes présentes dans le fichier
buffer * ouvrirFichier(char* nom) {
  FILE * f = NULL;
  buffer *b = malloc(sizeof(buffer));
  ligne *l = malloc(sizeof(ligne));
  curseur * c = malloc(sizeof(curseur));

  if(buffer_courant == NULL)
    {
      buffer_courant=b;
      b->prev=b;
      b->next=b;
    }
  else
    {
      b->prev=buffer_courant;
      b->next=buffer_courant->next;
      buffer_courant->next->prev=b;
      buffer_courant->next=b;
      buffer_courant=b;
    }

  c->ligne=0;
  c->colonne=0;
  b->curs = c;
  f = fopen(nom,"r");
  clear();
  refresh();
  if (f == NULL) {
    b = nouveauBuffer(nom);
    buffer_courant=b;
  }
  else {
    int taille = 200;
    b->nom = malloc(sizeof(char)*strlen(nom));
    strcpy(b->nom,nom);
    l->num = 1;
    l->prev = NULL;
    l->vrnum = 1;
    l->vrcol = 0;
    l->txt = malloc(200);
    l->taille = 200;
    b->premiere = l;
    char c = fgetc(f);
    int i = 0;
    int nbc = 0;
    b->taille=1;
    
    while(c!='\n' && c!=EOF) {
      nbc++;
      if (i>=taille) {
	l->txt = realloc(l->txt,taille*2);
	taille*=2;
	l->taille = taille;
      }
      *((l->txt)+i) = c;
      if(lw && nbc>=x) {
	l->next = remplirBuffer(f,2,l,1,1);
	fclose(f);
	return b;
      }
      c = fgetc(f);
      i++;
    }
  
    if (i>=taille) {
      l->txt = realloc(l->txt,taille*2);
      taille*=2;
      l->taille = taille;
    }
    *((l->txt)+i)='\0';
    if (c != EOF)
      l->next = remplirBuffer(f,2,l,2,0);
    else
      l->next = NULL;
    fclose(f);
  }
  return b;
}

buffer * chargerFichier() {
  clear();
  printw("Nom du fichier à ouvrir :\n");
  refresh();
  char *c = malloc(sizeof(char)*75);
  int* ch = malloc(sizeof(char));
  curseur *cur = malloc(sizeof(curseur));
  cur->ligne = 2;
  cur->colonne = 1;
  *c='\0';
  *ch = getch();
  while(*ch!='\n')
    {
      deplacer_curseur2(cur,*ch,c);
      move(cur->ligne-1,cur->colonne-1);
      *ch = getch();
    }
  return ouvrirFichier(c);
}

buffer * fermerBuffer() {
  if(buffer_courant->prev == NULL) {
    if (buffer_courant->next != NULL)
      return buffer_courant->next;
    else
      return chargerFichier();
  }
  else {
    if (buffer_courant->next == NULL)
      return buffer_courant->prev;
    else
      {
	buffer_courant->prev->next = buffer_courant->next;
	buffer_courant->next->prev = buffer_courant->prev;
	return buffer_courant->next;
      }
  }
}

//===========================
//FONCTION DE SAUVEGARDE ET DE GESTION DU FICHIER BACKUP
void enregistrerFichier() {
  FILE *f = NULL;
  f = fopen(buffer_courant->nom,"w+");
  if (f!=NULL) {
    ligne *l = buffer_courant->premiere;
    while(l!=NULL) {
      fputs(l->txt,f);
      fputc('\n',f);
      l=l->next;
    }
  }
  char *d = malloc(sizeof(char)*(strlen(buffer_courant->nom)+1));
  strcpy(d,".\0");
  strcat(d,buffer_courant->nom);
  remove(d);
  fclose(f);
}

void enregistrerFichierLW() {
  FILE *f = NULL;
  f = fopen(buffer_courant->nom,"w+");
  ligne *l = buffer_courant->premiere;
  int k = l->vrnum;
  if (f!=NULL) {
    while(l!=NULL) {
      while(l!=NULL && l->vrnum == k) {
	fputs(l->txt,f);
	l=l->next;
      }
      fputc('\n',f);
      if (l != NULL)
	k = l->vrnum;
    }
  }
  char *d = malloc(sizeof(char)*(strlen(buffer_courant->nom)+1));
  strcpy(d,".\0");
  strcat(d,buffer_courant->nom);
  remove(d);
  fclose(f);
}

void enregistrerFichierBackUp() {
  FILE *f = NULL;
  char *c = malloc(sizeof(char)*(strlen(buffer_courant->nom)+1));
  strcpy(c,".");
  strcat(c,buffer_courant->nom);
  f = fopen(c,"w+");
  if (f!=NULL) {
    ligne *l = buffer_courant->premiere;
    while(l!=NULL) {
      fputs(l->txt,f);
      fputc('\n',f);
      l=l->next;
    }
  }
  free(c);
  fclose(f);
}

void enregistrerFichierBackUpLW() {
  FILE *f = NULL;
  char *c = malloc(sizeof(char)*(strlen(buffer_courant->nom)+1));
  strcpy(c,".");
  strcat(c,buffer_courant->nom);
  f = fopen(c,"w+");
  ligne *l = buffer_courant->premiere;
  int k = l->vrnum;
  if (f!=NULL) {
    while(l!=NULL) {
      while(l!=NULL && l->vrnum == k) {
	fputs(l->txt,f);
	l=l->next;
      }
      fputc('\n',f);
      if (l != NULL)
	k = l->vrnum;
    }
  }
  free(c);
  fclose(f);
}

void enregistrer() {
  if (lw)
    enregistrerFichierBackUpLW();
  else
    enregistrerFichierBackUp();
}


void remplacerBackUp() {
  remove(buffer_courant->nom);
  char *c = malloc(sizeof(char)*(strlen(buffer_courant->nom)+1));
  strcpy(c,".");
  strcat(c,buffer_courant->nom);
  rename(c,buffer_courant->nom);
  free(c);
}

void supprimerBackUp() {
  char *c = malloc(sizeof(char)*(strlen(buffer_courant->nom)+1));
  strcpy(c,".");
  strcat(c,buffer_courant->nom);
  remove(c);
  free(c);
}


void recupBackUp(char *nom) {
  printf("Voulez vous récupérer le fichier %s ?\n (0) oui\n (...) non\n",nom+1);
  refresh();
  int ch = 0;
  scanf("%d",&ch);
  if (ch == '0') {
    char *d = malloc(sizeof(char)*strlen(nom));
    strcpy(d,nom+1);
    remove(d);
    rename(nom,d);
    free(d);
  }
  else
    remove(nom);
    
}

void verifBackUp() {
  DIR* rep = NULL;
  rep = opendir(".");
  char *c = malloc(sizeof(char)*100);
  if (rep != NULL) {
    struct dirent* doss;
    while((doss = readdir(rep)) != NULL) {
      strcpy(c,doss->d_name);
      if(*c == '.' && strlen(c) != 1 && *(c+1) != '.' && strcmp(c,".DS_Store") != 0 && strcmp(c,".config") != 0 && strcmp(c,".clipBoard.txt") != 0)
	recupBackUp(c);
      *c = '\0';
    }
  }
  free(c);
}


void nettoyageBackUp() {
  DIR* rep = NULL;
  rep = opendir(".");
  char *c = malloc(sizeof(char)*100);
  if (rep != NULL) {
    struct dirent* doss;
    while((doss = readdir(rep)) != NULL) {
      strcpy(c,doss->d_name);
      if(*c == '.' && strlen(c) != 1 && *(c+1) != '.' && strcmp(c,".DS_Store") != 0 && strcmp(c,".config") != 0)
	remove(c);
      *c = '\0';
    }
  }
  free(c); 
}


//===========================
//FONCTION DE GESTION DES OPTIONS (cf fichier de config)

int caseOpt(char *c) {
  for(int i = 0 ; i < 5 ; i++)
    if(strcmp(c,OPT[i]) == 0)
      return i;
  return -1;
}

void majOpt(int *p) {
  end = *p;
  save = *(p+1);
  load = *(p+2);
  close = *(p+3);
  select2 = *(p+4);
}

void chargerOpt() {
  FILE *f = NULL;
  int *p = malloc(sizeof(int)*4);
  for(int i = 1 ; i < 5 ; i++)
    *(p+i-1) = i;
  f = fopen(".config","r");
  char *c = malloc(sizeof(char)*100);
  char *key = malloc(sizeof(char)*10);
  int w = 0;
  int opt = 0;
  if (f != NULL) {
    c = fgets(c,100,f);
    while(c!=NULL && *c!=EOF && *c != ' ') {
      w = strcspn(c,":");
      memmove(key,c,w+2);
      opt = caseOpt(key);
      if (opt != -1)
	*(p+opt) = *(c+w+2)-48;
      *key = '\0';
      c = fgets(c,100,f);
    }
    majOpt(p);
    fclose(f);
  }
  free(p);
  free(key);
  free(c);
}

void initOpt() {
  OPT[0] = malloc(sizeof(char)*6);
  strcpy(OPT[0],"end:F\0");
  OPT[1] = malloc(sizeof(char)*7);
  strcpy(OPT[1],"save:F\0");
  OPT[2] = malloc(sizeof(char)*7);
  strcpy(OPT[2],"load:F\0");
  OPT[3] = malloc(sizeof(char)*8);
  strcpy(OPT[3],"close:F\0");
  OPT[4] = malloc(sizeof(char)*9);
  strcpy(OPT[4],"select2:F\0");
    
  end = 1;
  save = 2;
  load = 3;
  close = 4;
  select2 = 5;
  chargerOpt();
}

//===========================
//FONCTION BASIQUE DE L'APPLICATION (Lancement et boucle d'interaction)
void lancerProg(int argc, char *argv[]) {
  buffer_courant = NULL;
  selection = malloc(sizeof(curseur));
  selection->ligne = -1;
  selection->colonne = -1;
  select2 = 0;
  initscr();
  noecho();
  cbreak();
  keypad(stdscr,true);
  getmaxyx(stdscr,y,x);
  yl = y-1;
  plig = 1;
  lw = 0;
  pcol = 0;

  if (argc >= 2 && strcmp(argv[1],"-lw") == 0)
    lw = 1;
  for (int i = lw+1 ; i < argc ; i++) {
    ouvrirFichier(argv[i]);
  }
  if (argc == 1 || (argc == 2 && lw)) {
    chargerFichier();
  }
  recalibrercurseur();
  
}

void boucleProg() {
  unsigned int ch = getch();  
  int tour = 0;
  mousemask(ALL_MOUSE_EVENTS,NULL);
  while(ch != KEY_F(end)) {
    tour++;
    if (ch == KEY_F(save)) {
      if(lw)
	enregistrerFichierLW();
      else
	enregistrerFichier();
    }
    else if (ch == KEY_F(load))
      {
	chargerFichier();
	aff_fichier();
      }
    else if (ch == KEY_F(close)) {
      buffer_courant = fermerBuffer();
      if (buffer_courant == NULL)
	ch = KEY_F(close);
    }
    else if (ch == 545 || ch == 393)
      swapBuffer(-1);
    else if (ch == 560 || ch == 402)
      swapBuffer(1);
    else if (ch == KEY_F(5)) {
      if (select2) {
	select2 = 0;
	selection->ligne = -1;
	selection->colonne = -1;
      }
      else {
	select2 = 1;
	selection->ligne = (buffer_courant->curs->ligne)+plig-1;
	selection->colonne = (buffer_courant->curs->colonne)+pcol;
      }
    }
    else {
      deplacer_curseur(buffer_courant->curs, ch, tour);
    }
    affPos(buffer_courant->curs,y-1,x);
    move((buffer_courant->curs->ligne)-1,(buffer_courant->curs->colonne)-1);
    refresh();
    ch = getch();
  }
}

void recalibrercurseur() {
  buffer_courant->curs->ligne = buffer_courant->taille;
  move(0,0);
  if(buffer_courant->taille>=y) {
    aff_finFichier(prendreLigne(buffer_courant->taille-y+2));
    buffer_courant->curs->ligne = y-1;
    plig = buffer_courant->taille-y+2;
    buffer_courant->curs->colonne =strlen((prendreLigne(plig+buffer_courant->curs->ligne-1)->txt))+1;

  }
  else {
    
    if(buffer_courant->curs->colonne == -1)
      buffer_courant->curs->colonne = 1;
    else
      buffer_courant->curs->colonne = tailleLigne(derniereLigne(buffer_courant->premiere));
    buffer_courant->curs->ligne = buffer_courant->taille;
  }

  if (buffer_courant->curs->colonne > x) {
    buffer_courant->curs->colonne = x;
    pcol = strlen((prendreLigne(plig+buffer_courant->curs->ligne-1)->txt))+1-x;
  }
  aff_fichier();
   
  affPos(buffer_courant->curs,y-1,x);
  move((buffer_courant->curs->ligne)-1,(buffer_courant->curs->colonne)-1);
  refresh();
}
