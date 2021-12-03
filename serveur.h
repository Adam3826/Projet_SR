#ifndef SERVEUR_H
#define SERVEUR_H

#define MAX_SIZE_NAME 255

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


/*prend en argument l'adresse d'un entier p et l'entier représentant le numéro de port noport. 
Crée un socket d'écoute puis attache la socket d'écoute p*/
void initialisation_connection(int *p, int noport);


/*Prend l'adresse d'un socket au format int* en argument, envoie l'entier 2 dans le socket à un client si le server reçoit un message de ce client, puis ferme le socket avec ce client.*/
void dialogue_client(int *p);


void end_child();




#endif
