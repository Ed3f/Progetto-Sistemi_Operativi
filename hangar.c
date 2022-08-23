#include "defined.h"
// funzione che crea gli aerei e comunica alla torre che gli aerei sono tutti decollati
void  hangar(){
    char s[256];
    time_t timet;
    sem_t *sem = sem_open("/np1", O_CREAT, S_IRWXU|S_IRGRP|S_IWGRP, 1); 
    struct tm *pTm= localtime(&timet);
    printf("avvio hangar \n ");
    printf("creazione aerei\n");
    pid_t pid[10];
    //creazione dei processi aereo 
    for (int i = 0; i < 10; i++){
        pid[i] = fork();
        if ( pid[i] < 0){
            fprintf(stderr, "failed creation to aereo ");
        }
        if (pid[i] == 0){
            time (&timet);
            pTm = localtime(&timet);
            sprintf(s,"%02d:%02d:%02d", pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
            printf("%s", s);
             printf("\e[0;31m aereo %d codice %d\e[0m\n",i, getpid());
            aereo(i);
            exit(0);
        }
        sleep(2);
    }
    
    // attesa della terminazione di tutti i processi 
   for (int i = 0; i < 10; i++){
        waitpid (pid[i],0,0);
       time (&timet);
       pTm = localtime(&timet);
       sprintf(s, "%02d:%02d:%02d", pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
       printf("%s",s);
       printf("terminazione aereo %d\n ",i );
    }
    // fase terminale di hangar ed invio comunicazione di fine processo
    struct tdati stdati;
    int fd = open ( AEREO_TORRE_PATH,O_WRONLY);
    strcpy(stdati.s,"aerei terminati");
    if (write(fd, &stdati, sizeof(stdati)) == -1){
        printf("errore nell' invio\n");
    }
    else{
        printf("messaggio inviato correttamente\n");
    }
    sem_post(sem);
      close(fd);
}

