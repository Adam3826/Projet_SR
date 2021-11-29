
#include "serveur.h"

int main(int argc, char *argv[])
{

    int p;      //descripteur de fichier
    int noport; //port

    if (argc != 2)
    {
        fprintf(stderr, "erreur : nombre d'arguments \n");
        exit(-1);
    }
    noport = atoi(argv[1]);

    initialisation(&p, noport);
    dialogueClient(&p);

    return 0;
}