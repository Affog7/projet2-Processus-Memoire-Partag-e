#include <stdio.h>
int main() {
int i, j;
/* boucle pour créer les 3 processus fils */
for (i=0; i < 3; i++) {
switch (fork()) {
case -1: fprintf(stderr, "Erreur dans %d\n", getpid());
perror("fork");
exit(1);
case 0:
/* On est dans un fils */

printf("Processus fils %d pid %d -> Proc pere pid %d\n",i+1,getpid(),getppid());
fflush(stdout);

/* Ne pas oublier de sortir sinon on cree fact(10) processus */
exit(0);

default: /* On est dans le pere; ne rien faire */ ;
}
}

/* 2e boucle, attendre les 3 fils */
for (i=0; i < 3; i++) {
wait(NULL);
}
exit(0);
}
