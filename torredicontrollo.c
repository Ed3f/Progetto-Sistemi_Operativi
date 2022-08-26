#include "defined.h"
typedef enum { true, false } pista; 
/*
cuore pulsante del progetto legge i messaggi inviati da hangar ed aerei decide il 
decollo degli aerei 
*/
void torredicontrollo(){
    char si[256];
    time_t timet;
    struct tm *pTm = localtime(&timet);
    pista p1 = true; 
    pista p2 = true; 
    int richieste_aerei[10];
    int aereo_pista_1 ;
    int aereo_pista_2 ; 
    printf("avvio torre\n");
    int  fd, iReadCount = 1;
    struct tdati stdati;
    fd = open(AEREO_TORRE_PATH, O_RDONLY|O_NONBLOCK);  
    sem_t *sem = sem_open("/np1", O_CREAT, S_IRWXU|S_IRGRP|S_IWGRP, 1);  
    stdati.s[0] = '\0';
    int i = 0;
    int j = 0; 
    int u = 0; 
    while (strcmp(stdati.s, "aerei terminati")){ 
        sem_wait(sem);
        if (iReadCount = read(fd, &stdati, sizeof(stdati)) > 0 ){
            if (strcmp(stdati.s, "aereo pronto al decollo") == 0 ){
                if ( i < 10 ){// immagazzina le richieste ricevute
                    time (&timet);
                    struct tm *pTm = localtime(&timet);
                    sprintf(si,"%02d:%02d:%02d", pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
                    printf("%s aereo %d %s\n",si, stdati.nome_aereo, stdati.s);
                    richieste_aerei [i] = stdati.cod;
                    i++;  
                }
                // ciclo nel quale si attende la comunicazione del decollo del aereo per la liberazione della pista che ha occupato 
                    while (p1 == false && p2 == false  ){ 
                        u++;
                        int fs = open(COMUNICAZIONE_DECOLLO, O_RDONLY);
                        read(fs, &stdati,sizeof(stdati));
                        strcmp(stdati.s2,"aereo decollato");
                        time (&timet);
                        pTm = localtime(&timet);
                        sprintf(si,"%02d:%02d:%02d", pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
                        printf("%s",si);
                        printf("\e[0;31m %s %d\e[0m\n",stdati.s2, stdati.n_a);
                            if (aereo_pista_1 ==  stdati.nome_aereo_decollo){
                                p1 = true; 
                                break;
                            }
                            if (aereo_pista_2 == stdati.nome_aereo_decollo ){
                                p2 = true; 
                                break; 
                            }
                    }
                    // in questi 2 controlli si guarda se la pista è libera, in base alla pista libera la torre comunica all' aereo la pista da cui decollare
                    if (p1 == true ){ 
                        p1 = false; 
                        int cod1 = richieste_aerei[j];
                        aereo_pista_1 = cod1;
                        printf("autorizzazione concessa decollare da pista 1\n");
                        j++;
                        kill(cod1 ,SIGUSR1);
                    }
                    else if (p2 == true ){
                        p2 = false; 
                        int cod2 = richieste_aerei[j];
                        aereo_pista_2 = cod2;
                        printf("autorizzazione concessa decollare da pista 2\n");
                        j++;
                        kill(cod2,SIGUSR1); 
                    }
                    while ( u < 10 && u >=8){ // comunicazione del decollo del ultimo aereo 
                        int fs = open(COMUNICAZIONE_DECOLLO, O_RDONLY);
                        read(fs, &stdati, sizeof(stdati));
                        strcmp (stdati.s2,"aereo decollato");
                            time (&timet);
                            pTm = localtime(&timet);
                            sprintf(si,"%02d:%02d:%02d", pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
                            printf("%s",si);
                            printf("\e[0;31m %s %d\e[0m\n",stdati.s2, stdati.n_a);
                             u++;
                    }
            }
        }
        if (strcmp(stdati.s, "aerei terminati") == 0) {//controlla se la stringa arrivata è "aerei terminati" per terminare il programma
            printf("%s\n",stdati.s);
            close(fd);
        }
    }
}