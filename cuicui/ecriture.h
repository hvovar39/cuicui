// crée une ligne vide
ligne * ligneVide(int num, ligne *prev, ligne *next);
// crée une ligne remplie
ligne * ligneRemplie(int num, char *txt, ligne *prev, ligne *next);
// met à jour le numéro des lignes
void Mise_A_Jour();
// met à jour de manière récursive le numéro des lignes
void MAJ_secondaire(ligne *l, int k);
// met à jour la variable vrcol d'un buffer utilisant le linewrapping
void MAJ_secondaire_vrcol(ligne *l);
// écrit un caractère dans un buffer
void ecriture(char let, ligne *lig, int col);
// fait un saut de ligne dans le buffer
void saut_de_ligne(ligne *l, int col);
// supprime une ligne du buffer
void suppression_de_ligne (ligne *l);
// supprime un caractère ou appelle la suppression de ligne si besoin
void suppression(ligne *lig, int col);
// supprime une région entière du buffer
void suppr_region(ligne *l1, int col1, ligne *l2, int col2);
//supprime la sélection actuelle
void suppr_selection();
// écrit une région entière de texte dans le buffer
void ecriture_chaine(ligne *l, int col, char *chaine);
// remplace les caractères sélectionnés par un autre caractère
void ecriture_region (char let,ligne *l1, int col1, ligne *l2, int col2);
// remplace les caractères sélectionnés par une autre chaîne de caractère
void ecriture_region_chaine (char *chaine, ligne *l1, int col1, ligne *l2, int col2);
