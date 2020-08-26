//affiche une ligne
void aff_ligne(ligne * l);
// affiche un fichier/buffer
void aff_fichier();
// affiche la fin d'un fichier
void aff_finFichier(ligne *l);
// affiche la fin d'une ligne
void aff_finLigne(ligne *l, int col);
// gère le curseur et les appels aux fonctions d'écriture grâce aux touches du clavier
void deplacer_curseur(curseur *c,int ch, int tour);
// change de buffer
void swapBuffer(int i);
// affiche la position courante sur la dernière ligne
void affPos(curseur *c, int p, int t);
// gère le curseur et le clavier sur le chargement de fichier
void deplacer_curseur2(curseur *c,int ch, char *d);
// retourne le tableau de char représentant la partie sélectionner
char * getSelection();
// gère la KEY_RIGHT
void flec_droite(curseur *c);
// gère la KEY_LEFT
void flec_gauche(curseur *c);
// gère la KEY_UP
void flec_haut(curseur *c);
// gère la KEY_DOWN
void flec_bas(curseur *c);
// gère la touche entrée
void touche_entre(curseur *c);
// gère la souris
void souris(curseur *c);
// gère la touche DELETE
void touch_del(curseur *c);
// gère l'appel aux fonctions d'écriture
void ecrire(curseur *c, int ch, int tour);
