#include "client.h"

int main(int argc, char *argv[])
{

    int descripteur_fichier_client;      //descripteur de fichier
    int Nport; //port
    char *hostName;

    if (argc != 3)
    {
        fprintf(stderr, "erreur : nombre d'arguments \n");
        exit(-1);
    }

    Nport = atoi(argv[2]);
    hostName = argv[1];

    initialisationConnection(&descripteur_fichier_client, hostName, Nport);
    dialogue_serveur(&descripteur_fichier_client);

    return 0;
}
