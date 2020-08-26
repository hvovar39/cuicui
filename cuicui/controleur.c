#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <dirent.h>

#include "infos.h"

int hasNext(ligne * l) {
  return (l->next != NULL);
}

int hasPrev(ligne * l) {
  return (l->prev != NULL);
}

ligne * prendreLigne(int n) {
  ligne * l = buffer_courant->premiere;
  while (hasNext(l) && n != 1) {
    l = l->next;
    n--;
  }
  return l;
}

int tailleLigne(ligne * l) {
  return strlen(l->txt)+1;
}

ligne * derniereLigne(ligne *l) {
  if (hasNext(l))
    return derniereLigne(l->next);
  return l;
}




