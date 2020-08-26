#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <dirent.h>

#include "infos.h"
#include "gestion.h"


int main(int argc, char *argv[]) {
  //initialise les options
  initOpt();
  //charge les fichiers de type .*
  verifBackUp();
  
  lancerProg(argc,argv);

  boucleProg();

  nettoyageBackUp();
  endwin();
}
