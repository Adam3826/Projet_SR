#define MAX_SIZE_NAME 255 //taille max du nom d'un fichier sur linux

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>

#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <dirent.h>




/*prend en argument l'adresse d'un entier p et l'entier représentant le numéro de port noport. 
Crée un socket d'écoute puis attache la socket d'écoute p*/
void initialisation_connection(int *p, int noport);


/*Prend en argument l'adresse d'un socket, crée un processus fils à chaque connection de client,
chaque processus créé lance une boucle infinie pour répondre aux requetes des clients jusqu'à 
la deconnection du client. */
void dialogue_client(int *p);

/*prend en argument le socket de service et lance le protocole de consultation du
serveur (envoi de tous les fichiers et upload possible de l'un d'entre eux)*/
void consulter(int* socketServeur);


/*prend en argument le nom d'un fichier et le socket de service, vérifie que le fichier 
demandé par le client existe bien dans le serveur et en informe le client. renvoie 1 si 
le fichier existe, 0 sinon*/
int verif_existe(int* SocketServeur, char* nom_fic);

/*prend en argument le nom d'un fichier et le socket de service, vérifie que le fichier 
demandé par le client a un type valide et en informe le client. renvoie 1 si 
le fichier existe, 0 sinon*/
int verif_type(int* SocketServeur, char* nom_fic);

/*prend en argument le socket de service et lance le protocole de dépot sur 
le serveur (envoi de la taille, vérification de la validité du type de fichier,
stockage du fichier, accusé de récéption.)*/
void upload(int* socketServeur);

/*prend en argument le descripteur de fichier pour dialoguer, prend connaissance 
de la commande du client et appelle la fonction du protocole correspondant (consulter ou upload)*/
void repondre_commande(int *socketServeur);

//prend en argument la socket de service et envoie au client un fichier contenant la liste des fichiers disponibles
void envoi_liste(int* socketServeur);


//handler
void end_child();




