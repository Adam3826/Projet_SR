#include "serveur.h"

int main(int argc, char *argv[])
{
    int descripteur_fic;      //descripteur de fichier
    int Nport; //port
    
    if (argc != 2)
    {
        fprintf(stderr, "erreur : nombre d'arguments \n");
        exit(-1);
    }
    Nport = atoi(argv[1]);

    initialisation_connection(&descripteur_fic, Nport);
    dialogue_client(&descripteur_fic);
    

    return 0;
}
