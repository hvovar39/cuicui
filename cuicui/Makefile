all : cuicui

cuicui : CuiCui.c controleur.o ecriture.o gestion.o interface.o
	gcc CuiCui.c -o cuicui controleur.o ecriture.o gestion.o interface.o -lncurses

controleur.o : controleur.c ecriture.h interface.h
	gcc -c -Wall controleur.c

ecriture.o : ecriture.c
	gcc -c -Wall ecriture.c

gestion.o : gestion.c controleur.h
	gcc -c -Wall gestion.c

interface.o : interface.c
	gcc -c -Wall interface.c

clean :
	rm -rf cuicui *.o
