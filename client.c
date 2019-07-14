#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#define SHMSZ 27

main(){
//char c;
int shmid;
key_t key;
char *shm , *s;
key=5678;

//creation du segment

if((shmid=shmget(key,SHMSZ,IPC_CREAT | 0666))<0){
perror("shmget");
exit(1);
}

//attachement du segment à l'espace memoire

if((shm=shmat(shmid,NULL,0))==(char *) -1){

perror("shmat");
exit(1);
}

for(s=shm;*s!=NULL;s++){
putchar(*s);
putchar("\n");
}
*shm='*';
exit(0);
}
