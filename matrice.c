#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h> 
#include <sys/shm.h>  
#include <sys/sem.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>           
#include <sys/stat.h>        
#include <semaphore.h>


union semun {
   int  val;    /* Value for SETVAL */
   struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
   unsigned short  *array;  /* Array for GETALL, SETALL */
   struct seminfo  *__buf;  /* Buffer for IPC_INF
                              (Linux-specific) */
};

		
int main(char *arg[]){
	
		int i,j,i1,j1;
//		struct sembuf sb = {0, 0, 0}; 
		union semun sem_arg; // paramètre pour configurer le semaphore
    // Argument du premier processus

    // clé pour le semaphore 
    key_t sem_key = ftok("semfile",74);
    // on demande au system de nous créer le semaphore
    int semid = semget(sem_key, 1, 0666|IPC_CREAT);

    // la valeur du semaphore est initialisée à 1
    sem_arg.val = 1;
    if(semctl(semid, 0, SETVAL, sem_arg)==-1){
       perror("semctl");
       exit(1);
    }
    //memoire partagee
		pid_t pid;
		 // ftok to generate unique key 
    key_t key1 = ftok("shmfile",61); 
    key_t key2 = ftok("shmfile",62); 
    key_t key3 = ftok("shmfile",63);
    int M1[5][5],M2[5][5],M3[25][3]; 
//    int a = 0; // shared data
  for(i=0;i<5;i++){
  for(j=0;j<5;j++)
  {
	  M1[i][j]=4+i;
	  M2[i][j]=0;
	  }
  }
  
  //printf("%d",M1[0][2]);
 for(i=0;i<25;i++){
  for(j=0;j<3;j++)
  {
	  M3[i][j]=-1;
	  }
  }
    // shmget returns an identifier in shmid 
    int shmid1 = shmget(key1,1024,0666|IPC_CREAT); 
        int shmid2 = shmget(key2,1024,0666|IPC_CREAT); 
    int shmid3 = shmget(key3,1024,0666|IPC_CREAT); 
    // shmat to attach to shared memory 
     char **str1 = (char**) shmat(shmid1,(void**)0,0);
     sprintf(*str1,"%d",M1); 
     char **str2 = (char**) shmat(shmid2,(void**)0,0);
     sprintf(*str2,"%d",M2); 
     char **str3 = (char**) shmat(shmid3,(void**)0,0);
     sprintf(*str3,"%d",M3); 
//boucles pour 25 processus
  for(i=0;i<5;i++){
  for(j=0;j<5;j++)
  { 
	  
	  pid=fork();
	  if(pid < 0){
          perror("Erreur de création du processus\n");
          exit(EXIT_FAILURE);
    }

    if(pid == 0){
		struct sembuf sb = {0, 0, 0};
        //.................................................
        // tentative de verrou du semaphore
     sb.sem_op = -1;
     if(semop(semid, &sb,1) == -1){
       perror("sempo");
       exit(1);
     }
  
     //printf("Proc w %d %d\n", getpid(), atoi(*str1+1));
     // 
     for(i1=0;i1<5;i1++)
     {	      
	      for(j1=0;j1<5;j1++)
	      {
	         if(M2[i1][j1]!=-1)
	            {
		          int k,t,termi=0;
		           
		           for(k=0;k<5;k++)
		             { 
						 termi +=M1[i1][k]*M1[k][j1];
			         }
		 
		            for(t=0;t<25;t++)
		            {
			           if(M3[t][0]==-1)
			           {
				          M3[t][0]=i1;
				          M3[t][1]=j1;
				          M3[t][2]=termi;
	                      sprintf(*str3,"%d\n",M3); 	
	                      printf("%d :: %d  %d %d\n",getpid(),i1,j1,termi);	
	                      break;	 
				       }else{break;}
				 	 break;				 
		            }
		        M2[i1][j1]=-1;
		        sprintf(*str2,"%d\n",M2);
		       }else{break;}
		       break;
	     }
	}
     
     // déverrouillage du semaphore
     sb.sem_op = 1;
     if(semop(semid, &sb,1) == -1){
       perror("sempo");
       exit(1);
     } 
        
        //.................................................
    }
	  
	  }
  }     
		//str=Matrice_creation(atoi(arg[1]),atoi(arg[2]));
		
	//detach from shared memory 
  shmdt(*str1);
  shmdt(*str2);
  shmdt(*str3); 
  return 0;	
}
