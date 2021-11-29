#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include "client.h"

int main(int argc, char *argv[])
{

    int p;      //descripteur de fichier
    int noport; //port
    char *hostName;

    if (argc != 3)
    {
        fprintf(stderr, "erreur : nombre d'arguments \n");
        exit(-1);
    }

    noport = atoi(argv[2]);
    hostName = argv[1];

    initialisation(&p, hostName, noport);
    dialogue(&p);

    return 0;
}