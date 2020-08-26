#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <dirent.h>

#include"infos.h"

//affiche le contenu texte d'une ligne
void aff_ligne(ligne * l) {
  char *s = malloc(sizeof(char)*(x+1));
  if (pcol < strlen(l->txt)) {
    memcpy(s,(l->txt+pcol),x);
    strcat(s,"\0");
    if(strlen(s) != 0)
      printw(s);
    if(strlen(s)<x)
      printw("\n");
    yl--;
    free(s);
  }
  else
    printw("\n");
  refresh();
}

//affiche le contenu d'un fichier chargé dans un buffer
void aff_fichier() {
  ligne * l = buffer_courant->premiere;
  do {
    aff_ligne(l);
    l = (l->next);
  } while(l != NULL && yl > 0);
  yl = y -1;
}

void aff_finFichier(ligne *l) {
  do {
    aff_ligne(l);
    l = (l->next);
  } while(l != NULL && yl > 0);
  yl = y-1;
}

void aff_finLigne(ligne *l, int col) {
  printw(l->txt+col-1);
  printw("\n");
  refresh();
}

//retourne le tableau de char représentant la partie sélectionnée
char * getSelection(){
  int tailleSelection = 0;
  char * temp = malloc(sizeof(char)*1000);
  temp[1000]='\0';
  char * s = malloc(1);
  
  if(selection->ligne<buffer_courant->curs->ligne+plig-1 || (selection->ligne == buffer_courant->curs->ligne+plig-1 && selection->colonne<buffer_courant->curs->colonne+pcol))
    {
      if(selection->ligne == buffer_courant->curs->ligne+plig-1)
	tailleSelection = buffer_courant->curs->colonne+pcol - selection->colonne;
      else{
	tailleSelection = strlen(prendreLigne(selection->ligne)->txt) - selection->colonne;
	for (int i = selection->ligne+1; i<buffer_courant->curs->ligne+plig-1; i++)
	  tailleSelection += strlen(prendreLigne(i)->txt);
	tailleSelection += buffer_courant->curs->colonne+pcol;
      }
      
      s = realloc(s, sizeof(char)*tailleSelection+1);
      s[tailleSelection] = '\0';
      strcpy(temp, ((prendreLigne (selection->ligne))->txt) + (selection->colonne-1));
      if(selection->ligne == buffer_courant->curs->ligne+plig-1)
	temp[buffer_courant->curs->colonne - 4+pcol]='\0';
      strcpy(s, temp);
      if(selection->ligne != buffer_courant->curs->ligne+plig-1){
	strcat(s, "\n");
	for (int j = selection->ligne+1; j<buffer_courant->curs->ligne+plig-1; j++)
	  strcat (strcat (s, (prendreLigne (j))->txt), "\n");
	temp = (prendreLigne (buffer_courant->curs->ligne+plig-1)->txt);
	temp [buffer_courant->curs->colonne+pcol] = '\0';
	strcat(s, temp);
      }
      s[tailleSelection] = '\0';
    }
  
  else
    {
      if(selection->ligne == buffer_courant->curs->ligne+plig-1)
	tailleSelection =  selection->colonne - buffer_courant->curs->colonne+pcol;
      else{
	tailleSelection = selection->colonne;
	for (int i = selection->ligne-1; i>buffer_courant->curs->ligne+plig-1; i--)
	  tailleSelection += strlen(prendreLigne(i)->txt);
	tailleSelection += strlen(prendreLigne(buffer_courant->curs->ligne+plig-1)->txt) - buffer_courant->curs->colonne+pcol;
      }
      
      s = realloc(s, sizeof(char)*tailleSelection+1);
      s[tailleSelection] = '\0';
      strcpy( temp, (prendreLigne (buffer_courant->curs->ligne+plig-1)->txt) + buffer_courant->curs->colonne-1+pcol);
      if(selection->ligne == buffer_courant->curs->ligne+plig-1){
	temp [selection->colonne - buffer_courant->curs->colonne +1+pcol] = '\0';
	strcpy(s, temp);
      }
      else{
	strcpy(s, temp);
	strcat(s, "\n");
	for (int j = buffer_courant->curs->ligne+plig; j < selection->ligne; j++)
	  strcat (strcat (s, prendreLigne(j)->txt), "\n");
	temp = prendreLigne(selection->ligne)->txt;
	temp[selection->colonne] = '\0';
	strcat(s, temp);
      }
      s[tailleSelection] = '\0';
    }
  /*printf("!!!!%s!!!!!\n",s);
    refresh();*/
  return s;
}

//change la valeur du curseur en fonction des arrow keys du clavier, touche entrée, etc
void deplacer_curseur(curseur *c,int ch, int tour) {
  MEVENT event;
  char *s;
  FILE *f;
  switch(ch) {
  case KEY_RIGHT:
    flec_droite(c);
    break;
  case KEY_LEFT:
    flec_gauche(c);
    break;
  case KEY_UP:
    flec_haut(c);
    break;
  case KEY_DOWN:
    flec_bas(c);
    break;
  case '\n':
    touche_entre(c);
    break;
  case KEY_MOUSE:
    if(getmouse(&event) == OK) {
      if (event.bstate & BUTTON1_CLICKED) {
	c->ligne = event.y+1;
	c->colonne = event.x+1;
	if (buffer_courant->taille<(c->ligne)+plig-1)
	  c->ligne = derniereLigne(buffer_courant->premiere)->num-plig+1;
	else if (tailleLigne(prendreLigne(c->ligne+plig-1))<c->colonne)
	  c->colonne = tailleLigne(prendreLigne(c->ligne+plig-1));
      }
    }
    break;
  
     
  case 2 : //ctl+b pour copier
    s = getSelection();
    f = NULL;
    f = fopen(".clipBoard.txt", "w+");
    if (f!=NULL){
      fputs( s, f);
      fclose(f);
      popen ("pbcopy < .clipBoard.txt", "r");
    }
    break;
    
  case 14 : //ctrl+n pour couper
    s = getSelection();
    f = NULL;
    f = fopen(".clipBoard.txt", "w+");
    if (f!=NULL){
      fputs( s, f);
      fclose(f);
      popen ("pbcopy < .clipBoard.txt", "r");
    }
  case 127:
    touch_del(c);
    break;
  case 263:  //Pour linux
    touch_del(c);
    break;

    case 22:
    f = NULL;
    f = popen("xclip -o", "r");
    s = malloc(sizeof(char) * 9999);
    s[0] = '\0';
    char * tampon = malloc (sizeof(char) * 1000);
    if (f!=NULL) {
      while (fgets(s, sizeof(s), f) != NULL)
	strcat (s, tampon);
      fclose(f);
    }
    ecriture_chaine(prendreLigne(buffer_courant->curs->ligne), buffer_courant->curs->colonne-1, s);
    move(0,0);
    clear();
    aff_finFichier(prendreLigne(plig));
    free(tampon);
    free(s);
    break;
    
    
  default:
    ecrire(c,ch, tour);
    break;
  }
}

void swapBuffer(int i){
  if(i<0)
    {
      buffer_courant=buffer_courant->prev;
      clear();
      recalibrercurseur();
    }
  else
    {
      buffer_courant=buffer_courant->next;
      clear();
      recalibrercurseur();
    }
}

void affPos(curseur *c, int p, int t) {
  char *d = malloc(sizeof(char)*(t-1));
  if (lw) {
    sprintf(d,"ligne : %d ; colonne : %d",
    	    prendreLigne(c->ligne+plig-1)->vrnum,
	    c->colonne+(x*(prendreLigne(c->ligne+plig-1)->vrcol)));
  } else {
    sprintf(d,"ligne : %d ; colonne : %d",
	    c->ligne+plig-1,
	    c->colonne+pcol);
  }
  move(p,0);
  clrtoeol();
  printw(d);
  refresh();
}


void deplacer_curseur2(curseur *c,int ch, char *d) {
  if (ch == KEY_F(end)) {
    endwin();
    exit(0);
  }
  int taille = strlen(d);
  switch(ch) {
  case KEY_LEFT:
    if(c->colonne>1)
      c->colonne--;
    break;
  case KEY_RIGHT:
    if(c->colonne<=taille)
      c->colonne++;
    break;
  case 127:
    if(c->colonne>1) {
      char *mem = malloc(sizeof(char)*taille);
      if(c->colonne< taille)
	strcpy(mem,d+c->colonne-1);
      *(d+(c->colonne-2)) = '\0';
      if (c->colonne<taille)
	strcat(d,mem);
      move(c->ligne-1,0);
      clrtoeol();
      refresh();
      printw(d);
      refresh();
      free(mem);
    }
    c->colonne--;
    break;
    // pour LINUX
  case 263:
    if(c->colonne>1) {
      char *mem = malloc(sizeof(char)*taille);
      if(c->colonne< taille)
	strcpy(mem,d+c->colonne-1);
      *(d+(c->colonne-2)) = '\0';
      if (c->colonne<taille)
	strcat(d,mem);
      move(c->ligne-1,0);
      clrtoeol();
      refresh();
      printw(d);
      refresh();
      free(mem);
    }
    c->colonne--;
    break;    
  default:
    if(c->colonne == taille+1 && taille != 0) {
      strcat(d,(char*)&ch);
      move(c->ligne-1,0);
      printw(d);
      refresh();
      c->colonne++;
    }
    else if (taille == 0) {
      strcat(d,(char*)&ch);
      strcat(d,"\0");
      move(c->ligne-1,0);
      printw(d);
      refresh();
      c->colonne++;
    }
    else {
      char *mem = malloc(sizeof(char)*taille);
      strcpy(mem,(d+c->colonne-1));
      strcat(mem,"\0");
      *(d+c->colonne-1) = '\0';
      strcat(d,(char*)&ch);
      strcat(d,mem);
      move(c->ligne-1,0);
      printw(d);
      refresh();
      free(mem);
      c->colonne++;
    }
    break;		    
  }
   
}

void flec_droite(curseur *c) {
  if ((c->colonne)+pcol >= tailleLigne(prendreLigne(c->ligne+plig-1))) {
      if (hasNext(prendreLigne(c->ligne+plig))) {
	c->colonne = 1;
	pcol = 0;
	(c->ligne)++;
	move(0,0);
	clear();
	aff_finFichier(prendreLigne(plig));
      }
    } else {
      if (c->colonne == x) {
	if ((c->colonne)+pcol < tailleLigne(prendreLigne(c->ligne+plig-1)) && !lw) {
	  move(0,0);
	  clear();
	  aff_finFichier(prendreLigne(plig));
	  pcol++;
	}
	else
	  if (hasNext(prendreLigne(c->ligne+plig))) {
	    c->colonne = 1;
	    pcol = 0;
	    (c->ligne)++;
	    move(0,0);
	    clear();
	    aff_finFichier(prendreLigne(plig));
	  }
      } else
	(c->colonne)++;
    }
}

void flec_gauche(curseur *c) {
  if (c->colonne == 1) {
      if(pcol != 0) {
	pcol--;
	move(0,0);
	clear();
	aff_finFichier(prendreLigne(plig));
      }
      else if (hasPrev(prendreLigne(c->ligne+plig-1))) {
	c->ligne--;
	(c->colonne) = tailleLigne(prendreLigne(c->ligne+plig-1));
	if (c->colonne > x && (!lw)) {
	  pcol = tailleLigne(prendreLigne(c->ligne+plig-1)) - x;
	  move(0,0);
	  clear();
	  aff_finFichier(prendreLigne(plig));
	  c->colonne = x;
	} else if (c->colonne >x)
	  c->colonne = x;
	
      }
    } else {
      (c->colonne)--;
    }
}

void flec_haut(curseur *c) {
  if (hasPrev(prendreLigne(c->ligne+plig-1))) {
      if (c->ligne == 1) {
	move(0,0);
	clear();
	plig--;
	aff_finFichier(prendreLigne(plig));
      }
      else if (tailleLigne(prendreLigne((c->ligne+plig-2))) >= (c->colonne+pcol))
	(c->ligne)--;
      else {
	(c->ligne)--;
	(c->colonne) = tailleLigne(prendreLigne(c->ligne+plig-1));
	if (c->colonne > x) {
	  pcol = tailleLigne(prendreLigne(c->ligne+plig-1)) - x;
	  move(0,0);
	  clear();
	  aff_finFichier(prendreLigne(plig));
	  c->colonne = x;
	} else {
	  pcol = 0;
	  move(0,0);
	  clear();
	  aff_finFichier(prendreLigne(plig));
	}
      }
  }
  else {
    pcol = 0;
    (c->colonne) = 1;
    move(0,0);
    clear();
    aff_finFichier(prendreLigne(plig));
  }
}

void flec_bas(curseur *c) {
  if (hasNext(prendreLigne(c->ligne+plig-1))) {
      if (c->ligne == y-1) {
	move(0,0);
	clear();
	plig++;
	aff_finFichier(prendreLigne(plig));
      }
      else if (tailleLigne(prendreLigne((c->ligne+1)+plig-1)) >= (c->colonne)+pcol)
	(c->ligne)++;
      else {
	(c->ligne)++;
	(c->colonne) = tailleLigne(prendreLigne(c->ligne+plig-1));
	if (c->colonne > x) {
	  pcol = tailleLigne(prendreLigne(c->ligne+plig-1)) - x;
	  move(0,0);
	  clear();
	  aff_finFichier(prendreLigne(plig));
	  c->colonne = x;
	}
      }
    }
    else
      (c->colonne) = tailleLigne(prendreLigne(c->ligne+plig-1))+1;
    if (c->colonne > x) {
      pcol = tailleLigne(prendreLigne(c->ligne+plig-1)) - x;
      move(0,0);
      clear();
      aff_finFichier(prendreLigne(plig));
      c->colonne = x;
    }
}

void touche_entre(curseur *c) {
  if (select2) {
      if (c->ligne+plig-1 == selection->ligne) {
	if (c->colonne+pcol > selection->colonne) {
	  suppr_region(prendreLigne(selection->ligne), selection->colonne, prendreLigne(c->ligne+plig-1), c->colonne+pcol);
	  saut_de_ligne(prendreLigne(selection->ligne),selection->colonne);	  
	}
	else {
	  suppr_region(prendreLigne(c->ligne+plig-1), c->colonne+pcol, prendreLigne(selection->ligne), selection->colonne);
	  saut_de_ligne(prendreLigne(c->ligne+plig-1),c->colonne-1);
	}
      }
      else if (c->ligne+plig-1 > selection->ligne) {
	suppr_region(prendreLigne(selection->ligne), selection->colonne, prendreLigne(c->ligne+plig-1), c->colonne+pcol);
	saut_de_ligne(prendreLigne(selection->ligne),selection->colonne);
	if (selection->ligne < plig) {
	  plig = selection->ligne;
	  c->ligne = 1;
	}
	else {
	  c->ligne = selection->ligne-plig+1;
	}	
      }
      else {
	suppr_region(prendreLigne(c->ligne+plig-1), c->colonne+pcol, prendreLigne(selection->ligne), selection->colonne);
	saut_de_ligne(prendreLigne(c->ligne+plig-1),c->colonne-1);
      }
      select2 = 0;
      selection->ligne = -1;
      selection->colonne = -1;
    }
    else {
      saut_de_ligne(prendreLigne(c->ligne+plig-1),c->colonne-1+pcol);
    }
    pcol = 0;
    move(0,0);
    clear();
    if (c->ligne == y-1) {
      plig++;
      c->ligne--;
    }
    pcol = 0;
    aff_finFichier(prendreLigne(plig));
    c->colonne = 1;
    c->ligne++;
}


void touch_del(curseur *c) {
  int coltmp = 0;
  if (prendreLigne((c->ligne-1)+plig-1) != NULL)
    coltmp = strlen((prendreLigne((c->ligne-1)+plig-1))->txt)+1;
  if (select2) {
    if (c->ligne+plig-1 == selection->ligne) {
      if (c->colonne+pcol > selection->colonne) {
	suppr_region(prendreLigne(selection->ligne), selection->colonne, prendreLigne(c->ligne+plig-1), c->colonne+pcol);
	if (selection->colonne < pcol){
	  pcol = selection->colonne;
	  c->colonne = 0;
	}
	else {
	  c->colonne = selection->colonne-pcol;
	}
      }
      else {
	suppr_region(prendreLigne(c->ligne+plig-1), c->colonne+pcol, prendreLigne(selection->ligne), selection->colonne);
      }
    }
    else if (c->ligne+plig-1 > selection->ligne) {
      suppr_region(prendreLigne(selection->ligne), selection->colonne, prendreLigne(c->ligne+plig-1), c->colonne+pcol);
      if (selection->ligne < plig) {
	plig = selection->ligne;
	c->ligne = 1;
      }
      else {
	c->ligne = selection->ligne-plig+1;
      }
      if(selection->colonne < pcol || selection->colonne > pcol+x) {
	pcol = selection->colonne;
	c->colonne = 1;
      }
      else {
	c->colonne = selection->colonne-pcol;
      }
    }
    else {
      suppr_region(prendreLigne(c->ligne+plig-1), c->colonne+pcol, prendreLigne(selection->ligne), selection->colonne);
    }
    selection->ligne = -1;
    selection->colonne = -1;
  }
  else {
    suppression(prendreLigne(c->ligne+plig-1),c->colonne-1+pcol);
  }
  if (c->colonne == 1) {
    if (hasPrev(prendreLigne(c->ligne+plig-1))) {
      if(c->ligne == 1)
	plig--;
      else
	c->ligne--;
      c->colonne = coltmp;
      if (c->colonne > x) {
	if(!lw && x < coltmp)
	  pcol = coltmp;
	move(0,0);
	clear(); 
	aff_finFichier(prendreLigne(plig));
	c->colonne = 1;
      }
    }
  }
  else if(!select2)
    c->colonne--;
  if(c->colonne+1 <= x) {
    move(0,0);
    clear();
    aff_finFichier(prendreLigne(plig));
    refresh();
    select2 = 0;
  }

}

void ecrire(curseur *c, int ch, int tour) {
  if (c->colonne == 1 || *((prendreLigne(c->ligne+plig-1))->txt+(c->colonne-2))!= '\0') {
      if (select2) {
	if (c->ligne+plig-1 == selection->ligne) {
	  if (c->colonne+pcol > selection->colonne) {
	    ecriture_region(ch, prendreLigne(selection->ligne), selection->colonne, prendreLigne(c->ligne+plig-1), c->colonne+pcol);
	    if (selection->colonne < pcol){
	      pcol = selection->colonne;
	      c->colonne = 1;
	    }
	    else {
	      c->colonne = selection->colonne-pcol+1;
	    }
	  }
	  else {
	    ecriture_region(ch, prendreLigne(c->ligne+plig-1), c->colonne+pcol, prendreLigne(selection->ligne), selection->colonne);
	  }
	}
	else if (c->ligne+plig-1 > selection->ligne) {
	  ecriture_region(ch, prendreLigne(selection->ligne), selection->colonne, prendreLigne(c->ligne+plig-1), c->colonne+pcol);
	  if (selection->ligne < plig) {
	    plig = selection->ligne;
	    c->ligne = 1;
	  }
	  else {
	    c->ligne = selection->ligne-plig+1;
	  }
	  if(selection->colonne < pcol || selection->colonne > pcol+x) {
	    pcol = selection->colonne;
	    c->colonne = 1;
	  }
	  else {
	    c->colonne = selection->colonne-pcol;
	  }
	}
	else {
	  ecriture_region(ch, prendreLigne(c->ligne+plig-1), c->colonne+pcol, prendreLigne(selection->ligne), selection->colonne);
	}
	select2 = 0;
	selection->ligne = -1;
	selection->colonne = -1;
	aff_finFichier(prendreLigne(plig));
      }
      else {
	ecriture(ch,prendreLigne(c->ligne+plig-1),c->colonne-1+pcol);
      }
      if (lw && c->colonne == x) {
	if (c->ligne == y-1) {
	  plig++;
	}
	else
	  c->ligne++;
	c->colonne = 1;
      }
      else
	c->colonne++;
      if (c->colonne > x && !lw) {
	pcol++;
	c->colonne = x;
      }
      move(c->colonne,c->ligne);
      clear();
      aff_finFichier(prendreLigne(plig)); 
    }
  if(tour%10 == 0)
    enregistrer();
}
