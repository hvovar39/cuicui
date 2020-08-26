#include"infos.h"

ligne * ligneVide(int num, ligne *prev, ligne *next) {
  ligne *l = malloc(sizeof(ligne));
  l->num = num;
  l->prev = prev;
  l->next = next;
  l->taille = 5;
  l->txt = malloc(sizeof(char)*5);
  memset(l->txt,'\0',5);
  return l;
}

ligne * ligneRemplie(int num, char *txt, ligne *prev, ligne *next) {
  ligne *l = malloc(sizeof(ligne));
  l->num = num;
  l->prev = prev;
  l->next = next;
  l->taille = strlen(txt);
  l->txt = malloc(sizeof(char)*(l->taille));
  strcpy(l->txt,txt);
  return l;
}

void Mise_A_Jour(){
  ligne *tmp = buffer_courant->premiere;
  int n = 1;
  while (tmp != NULL){
    if (lw == 1) {
      if (tmp->vrcol > 0){ 
	tmp->vrnum = tmp->prev->vrnum;
      }
      else {
	if (tmp->prev == NULL)
	  tmp->vrnum =1;
	else
	  tmp->vrnum = tmp->prev->vrnum+1;
      }
    }
    tmp = tmp->next;
    n++;
  }
}

void MAJ_secondaire(ligne *l, int k){
  ligne *tmp = l;
  int n = k;
  while (tmp->next != NULL){
    tmp->num = n;
    if (lw == 1) {
      if (tmp->vrcol > 0){
	tmp->vrnum = tmp->prev->vrnum;
      }
      else {
	if (tmp->prev == NULL)
	  tmp->vrnum = 1;
	else
	  tmp->vrnum = tmp->prev->vrnum+1;
      }
    }
    tmp = tmp->next;
    n++;
  }
}

void MAJ_secondaire_vrcol(ligne *l){
  ligne *tmp = l;
  do
    {
      tmp->vrcol = tmp->prev->vrcol+1;
      tmp = tmp->next;
    }while (tmp->vrcol != 0);
}

void ecriture(char let, ligne *lig, int col){
  int tl = strlen(lig->txt);
  if(strlen(lig->txt) == 0) {
    lig->txt = malloc(sizeof(char)*5);
    lig->taille = 5;
    *((lig->txt)+0) = let;
    *((lig->txt)+1) = '\0';
  } 
  else if (strlen(lig->txt) >= lig->taille) {
    lig->txt = realloc (lig->txt, (lig->taille)*2);
    lig->taille = (lig->taille)*2;
    ecriture(let, lig, col);
  }
  else {
    char tmp = lig->txt[col];
    if (lw) {
      if (col == x-1) {
	if (lig->next != NULL && lig->next->vrnum == lig->vrnum){
	  *((lig->txt)+col) = let;
	  ecriture(tmp,lig->next,0);
	}
	else {
	  *((lig->txt)+col) = let;
	  *((lig->txt)+tl+1) = '\0';
	  ligne *new = malloc(sizeof(ligne));
	  new->txt = malloc(sizeof(char)*5);
	  new->taille = 5;
	  new->num = lig->num+1;
	  new->prev = lig;
	  if (lig->next != NULL) {
	    new->next = lig->next;
	    lig->next->prev = new;
	  }
	  else
	    new->next = NULL;
	  lig->next = new;
	  new->vrnum = lig->vrnum;
	  new->vrcol = lig->vrcol+1;
	  memset(new->txt,'\0',5);
	  MAJ_secondaire(lig,lig->num);
	  buffer_courant->taille++;
	  ecriture(tmp,new,0);
	}
      }
      else {
	*((lig->txt)+tl) = '\0';
	*((lig->txt)+col) = let;
	*((lig->txt)+tl+1) = '\0';
	if (tmp != '\0') {
	  ecriture(tmp,lig,(col+1));
	}
      }
    }
    else {
      *((lig->txt)+tl) = '\0';
      *((lig->txt)+col) = let;
      *((lig->txt)+tl+1) = '\0';
      if (tmp != '\0') {
	ecriture(tmp,lig,(col+1));
      }
    }
  }
}

void saut_de_ligne(ligne *l, int col) {
  char *c = malloc(sizeof(char)*strlen(l->txt));
  if(col == 0 && l->prev == NULL) {
    buffer_courant->premiere = ligneVide(1,NULL,l);
    l->prev = buffer_courant->premiere;
    Mise_A_Jour();
  }
  else {
    if (col == 0) {
      l->prev->next = ligneVide(l->num,l->prev,l);
      l->prev = l->prev->next;
      if(l->prev != NULL)
	MAJ_secondaire(l->prev,l->num);
    }
    else {
      if(col<strlen(l->txt)) {
	strcpy(c,l->txt+col);
	strcat(c,"\0");
      }
      ligne *new = malloc(sizeof(ligne));
      new->taille = strlen(c)+1;
      new->txt = malloc(sizeof(char)*(new->taille));
      new->num = l->num+1;
      new->prev = l;
      new->next = l->next;
      if (lw) {
	new->vrnum = l->vrnum;
	new->vrcol = 0;
      }
      memset(new->txt,'\0',new->taille);
      if (l->next != NULL)
	l->next->prev = new;
      if (strlen(l->txt)>col) {
	*((l->txt)+col) = '\0';
	strcpy(new->txt,c);
	if (lw && new->next != NULL && new->next->vrnum == new->vrnum) {
	  suppression_de_ligne(new->next);
	}
      }
      else {
	memset(new->txt,'\0',new->taille);
      }
      l->next = new;
      MAJ_secondaire(l,l->num);
    }
  }
  Mise_A_Jour();
  buffer_courant->taille++;
}

void suppression_de_ligne (ligne *l) {
  if(lw) {
    if (l->prev != NULL) {
      if (strlen(l->txt) == 0) {
	if (l->next == NULL)
	  l->prev->next = NULL;
	else {
	  l->prev->next = l->next;
	  l->next->prev = l->prev;
	}
      }
      else {
	int n  = x-strlen(l->prev->txt);
	if (l->prev->taille < strlen(l->prev->txt)+strlen(l->txt)) {
	  l->prev->txt = realloc(l->prev->txt,sizeof(char)*( strlen(l->prev->txt)+strlen(l->txt)+10));
	  l->prev->taille = (strlen(l->prev->txt)+strlen(l->txt)+10);
	}
	if (strlen(l->txt) != 0) 
	  strncat(l->prev->txt,l->txt,n);
	if (strlen(l->txt) > n) {
	  l->vrcol = l->prev->vrcol+1;
	  char *tmp = ((l->txt)+n);
	  l->txt = malloc(sizeof(char)*200);
	  l->taille = 200;
	  strcpy(l->txt,tmp);
	  MAJ_secondaire(l->prev,l->prev->num);
	}
	else {
	  if (l->next == NULL)
	    l->prev->next = NULL;
	  else {
	    l->prev->next = l->next;
	    l->next->prev = l->prev;
	  }
	}
	if (l->next->vrnum == l->vrnum){
	  suppression_de_ligne(l->next);
	}
      }
      buffer_courant->taille--;
    }
  }
  else {
    if (l->prev != NULL) {
      if (l->prev->taille <= strlen(l->prev->txt)+strlen(l->txt)) {
	if(strlen(l->prev->txt) == 0)
	  l->prev->txt = malloc(sizeof(char)*(strlen(l->prev->txt)+strlen(l->txt)+10));
	else
	  l->prev->txt = realloc(l->prev->txt,sizeof(char)*(strlen(l->prev->txt)+strlen(l->txt)+10));
	l->prev->taille = (strlen(l->prev->txt)+strlen(l->txt)+10);
      }
      if (strlen(l->txt) != 0) 
	strcat(l->prev->txt,l->txt);
      if(l->next != NULL) {
	l->next->prev = l->prev;
	l->prev->next = l->next;
	MAJ_secondaire(l->prev,l->prev->num);
      }
      buffer_courant->taille--;
    }
  }
}

void suppression(ligne *lig, int col){
  if (col == 0){
    if (lw && lig->vrcol > 0) {
      suppression(lig->prev,(x-1));
    }
    else {
      suppression_de_ligne(lig);
    }
  }
  else{
    if (lw == 1 && lig->next != NULL && lig->next->vrnum == lig->vrnum){
      if (col-1 ==  x-1) {
	*((lig->txt)+col-1) = lig->next->txt[0];
	suppression(lig->next,1);
      }
      else {
	*((lig->txt)+col-1) = *((lig->txt)+col);
	suppression(lig,(col+1));
      }
    }
    else {
      char tmp = lig->txt[col-1];
      if (tmp == '\0') {
	*((lig->txt)+col-1) = *((lig->txt)+col);
	lig->txt = realloc (lig->txt, (lig->taille)-1);
      }
      else {
	*((lig->txt)+col-1) = *((lig->txt)+col);
	suppression(lig,(col+1));

      }
    }
  }
}

void suppr_region(ligne *l1, int col1, ligne *l2, int col2){
  int tmp1,tmp2;
  int b = 0;
  int tai = col1+(strlen(l2->txt)-col2);
  if (l2->next == NULL)
    b++;
  else if (lw) {
    if (l2->next->vrnum != l2->vrnum) 
      b++;
  }
  else {
    if (l2->next->num != l2->num)
      b++;
  }
  if (l1->num == l2->num) {
    tmp1 = 0;
    tmp2 = col2-col1;
    while(tmp1 <= tmp2) {
      suppression(l1,col1);
      tmp1++;
    }
    if (l1->prev != NULL)
      MAJ_secondaire(l1->prev,l1->prev->num);
    else
      MAJ_secondaire(l1,1);
  }
  else {
    if (l2->next != NULL)
      l1->next = l2->next;
    else
      l1->next = NULL;
    tmp1 = col1;
    while (tmp1 < strlen(l1->txt)){
      suppression(l1,col1);
      tmp1++;
    }
    tmp1 = col1;
    tmp2 = col2;
    while (tmp2 < strlen(l2->txt)){
      ecriture(l2->txt[tmp2],l1,tmp1);
      tmp1++;
      tmp2++;
    }
    if (b)
      *((l1->txt)+tai) = '\0';
    if (l1->prev != NULL)
      MAJ_secondaire(l1->prev,l1->prev->num);
    else
      MAJ_secondaire(l1,1);    
  }
}

void suppr_selection() {
  if(selection->ligne<buffer_courant->curs->ligne || (selection->ligne == buffer_courant->curs->ligne && selection->colonne<buffer_courant->curs->colonne))
    {
      suppr_region (prendreLigne(selection->ligne), selection->colonne, prendreLigne(buffer_courant->curs->ligne), buffer_courant->curs->colonne);
      aff_finFichier(prendreLigne(selection->ligne));
    }
  else
    {
      suppr_region (prendreLigne(buffer_courant->curs->ligne), buffer_courant->curs->colonne, prendreLigne(selection->ligne), selection->colonne);
      aff_finFichier(prendreLigne(buffer_courant->curs->ligne));
    }
  refresh();
}

void ecriture_chaine(ligne *l, int col, char *chaine){
  ligne *tmp_l = l;
  int tmp_col = col;
  int tmp_cha = 0;
  while (tmp_cha < strlen(chaine)) {
    if (chaine[tmp_cha]=='\n'){
      saut_de_ligne(tmp_l, tmp_col);
      tmp_l = tmp_l->next;
      tmp_col = 0;
      tmp_cha++;
    }
    else {
      ecriture(chaine[tmp_cha], tmp_l, tmp_col);
      tmp_col++;
      tmp_cha++;
    }
  }
  MAJ_secondaire(l,l->num);
}

void ecriture_region (char let,ligne *l1, int col1, ligne *l2, int col2) {
  suppr_region(l1,col1,l2,col2);
  ecriture(let,l1,col1);
}

void ecriture_region_chaine (char *chaine, ligne *l1, int col1, ligne *l2, int col2) {
  suppr_region(l1,col1,l2,col2);
  ecriture_chaine(l1,col1,chaine);
}
