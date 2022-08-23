#include "defined.h"
int main() {
    char s[256];
    time_t timet;
    struct tm *pTm;
    mkfifo(AEREO_TORRE_PATH, O_CREAT|S_IRWXU);
    mkfifo(COMUNICAZIONE_DECOLLO, O_CREAT|S_IRWXU);
    sem_t *sem = sem_open("/np1", O_CREAT, S_IRWXU|S_IRGRP|S_IWGRP, 1);
    pid_t pid;
    pid = fork();
    if (pid < 0 ){
        fprintf(stderr,"Errore nel fork\n");
        return 1;
    }//avvio processo hangar
    if (pid == 0 ){
        hangar ();
        time (&timet);
        pTm = localtime(&timet);
        sprintf(s, "%02d:%02d:%02d", pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
        printf("%s",s);
        printf("terminazione hangar\n");
        return 0;
    }
    else {//avvio processo torre
        torredicontrollo();
    }
    // attesa della terminazione dei processi hangar e torre 
     if (pid > 0){
        waitpid(pid, 0, 0);
        time (&timet);
        pTm = localtime(&timet);
        sprintf(s, "%02d:%02d:%02d", pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
        printf("%s",s);
        printf("terminazione torre\n ");
    }
    unlink(AEREO_TORRE_PATH);
    unlink(COMUNICAZIONE_DECOLLO);
    return 2;
}
