#include "defined.h"
int attesa (int max, int min);
void scrittura_su_pipe(int n);
void comunicazione_decollo( int n );
/*
Funzione aereo la quale attende un periodo tra gli 8-3 sec per la preparazioe 
comunica alla torre la volontà di prendere il volo ed infine attende dai 5-15 sec 
per il decollo effettivo  
*/
void  aereo(int nome_aereo){
    char s[256];
    time_t timet;
    time (&timet);
    struct tm *pTm;
    sem_t *sem = sem_open("/np1", O_CREAT, S_IRWXU|S_IRGRP|S_IWGRP, 1); 
    sigset_t sig1; 
    int sign1;
    sigemptyset(&sig1);
    sigaddset(&sig1,SIGUSR1);
    sigprocmask(SIG_BLOCK, &sig1, NULL);
    int secondi_attesa;
    int secondi_attesa2;
    secondi_attesa = attesa (8, 3);
    printf("attesa %d\n ",secondi_attesa);
    sleep(secondi_attesa);
    scrittura_su_pipe(nome_aereo);  
    sigwait(&sig1,&sign1); // autorizzazione  concessa dalla torre all'aereo
    int secondi_attesa1 = attesa(15,5);
    printf("aereo %d secondi attesa %d per la preparazione al decollo\n",nome_aereo,secondi_attesa1);
    sleep(secondi_attesa1);
    comunicazione_decollo(nome_aereo);
}
// funzione che randomicamente mi determina il tempo di attesa
int attesa (int max, int min){
    time_t timet;
    time (&timet);
    struct tm *pTm = localtime(&timet);
    srand(pTm->tm_sec);
    int valore = (rand () % max);
    return ((valore < min ? min+1 : valore));
}

// funzione nella quale l'aereo invia la notifica di esser pronto al decollo dopo il periodo di attesa
void scrittura_su_pipe(int n ){
    struct tdati stdati;
    char s[256];
    time_t timet;
    struct tm *pTm;
    sem_t *sem = sem_open("/np1", O_CREAT, S_IRWXU|S_IRGRP|S_IWGRP, 1);
    int fd = open ( AEREO_TORRE_PATH,O_WRONLY|O_NONBLOCK);
    stdati.nome_aereo = n;
    stdati.cod= getpid();
    strcpy(stdati.s,"aereo pronto al decollo");
    if(write(fd, &stdati, sizeof(stdati)) == -1) {
        perror("Child: Errore in write");
    } 
    sem_post(sem);
    close(fd);
}
// comunicazione per il decollo del aereo 
void comunicazione_decollo(int n){
    struct tdati stdati;
    int fs = open (COMUNICAZIONE_DECOLLO,O_WRONLY);
    stdati.n_a = n;
    stdati.nome_aereo_decollo = getpid();
    strcpy(stdati.s2,"aereo decollato");
    if(write(fs, &stdati, sizeof(stdati)) == -1) {
        perror("Child: Errore in write");
    } 
    close (fs);
}