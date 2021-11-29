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


void initialisation(int *p, int noport);
void dialogueClient(int *p);
void end_child();

#endif