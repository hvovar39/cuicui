// fonction appelée par ouvrirFichier pour remplir le buffer des lignes suivantes si nécessaire
ligne* remplirBuffer(FILE * f, int num, ligne* prev,int vrnum,int vrcol);
// crée un nouveau buffer vide
buffer * nouveauBuffer(char *nom);
// remplit le buffer des lignes présentes dans le fichier
buffer * ouvrirFichier(char* nom);
// affiche une mini-interface pour ouvrir/charger un nouveau fichier
buffer * chargerFichier();
// ferme un buffer
buffer * fermerBuffer();
// enregistre un buffer (dans le fichier lié)
void enregistrerFichier();
// enregistre un buffer (dans le fichier lié) en prenant en compte le linewrapping
void enregistrerFichierLW();
// enregistre un fichier de sauvegarde automatique
void enregistrerFichierBackUp();
// enregistre un fichier de sauvegarde automatique en prenant en compte le linewrapping
void enregistrerFichierBackUpLW();
// gère l'enregistrement des fichiers de sauvegarde
void enregistrer();
// remplace le fichier par le fichier de sauvegarde automatique
void remplacerBackUp();
// supprime le fichier de sauvegarde automatique
void supprimerBackUp();
// affiche une mini-interface pour que l'utilisateur décide s'il souhaite récupérerer des fichiers de sauvegarde automatique
void recupBackUp(char *nom);
// vérifie s'il y a des fichiers de sauvegarde automatique dans le dossier
void verifBackUp();
// supprime les fichiers de sauvegarde automatique du dossier courant
void nettoyageBackUp();
// recherche dans les options disponibles pour les mettres à jour
int caseOpt(char *c);
// met à jour les options/commanes
void majOpt(int *p);
// charge le fichier .config pour charger les options
void chargerOpt();
// initialise les options
void initOpt();
// lance les fonctions pour initialiser le programme
void lancerProg(int argc, char *argv[]);
// gère la boucle du programme
void boucleProg();
// recalibre le curseur en fonction du buffer ouvert
void recalibrercurseur();
