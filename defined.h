#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <semaphore.h>
#define AEREO_TORRE_PATH "/tmp/AEREO_TORRE_PATH"
#define COMUNICAZIONE_DECOLLO "/tmp/COMUNICAZIONE_DECOLLO"
struct tdati{ 
    int nome_aereo;
    int nome_aereo_decollo;
    int n_a; 
    int cod;  
    char s[64];
    char s2[64];
    };
void hangar();
void aereo(int nome_aereo);
void torredicontrollo();

