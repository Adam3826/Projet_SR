#ifndef SERVEUR_H
#define SERVEUR_H

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
#include <string.h>

/*prend l'adresse d'un entier p, le nom de l'hôte hostName et le numéro de port noport en argument
crée un socket, récupère les informations du serveur grâce au nom et demande la connection*/
void initialisationConnection(int *descripteur_fichier_client, char *hostName, int Nport);


/*prend le noméro de socket p en argument, attend une chaine de cacactère de l'entrée standard et envoie cette chaine par le socket. 
Attend ensuite la réception d'un entier par le socket*/
void dialogue_serveur(int *descripteur_fichier_client);


#endif
