#include "client.h"

void initialisationConnection(int *descripteur_fichier_client, char *hostName, int Nport)
{
    
    struct sockaddr_in pointeur_socket;
    struct hostent *host_name;
    
    //strcpy(hostName, argv[1]);

    //---------creation du socket--------
    *descripteur_fichier_client = socket(AF_INET, SOCK_STREAM, 0);
	
	switch(*descripteur_fichier_client){
		case -1:
			perror("socket");
			exit(-1);
		default :
			 printf("Descripteur créé : %d\n", *descripteur_fichier_client);
			 break;
	}

    //récupération dans host_name des informations concernant le serveur
    host_name = gethostbyname(hostName);
    memcpy(&pointeur_socket.sin_addr.s_addr, host_name->h_addr, host_name->h_length);

    //preparation des champs sin_family, sin_addr et sin_port
    pointeur_socket.sin_addr = *((struct in_addr *)host_name->h_addr);
    pointeur_socket.sin_family = AF_INET;
    pointeur_socket.sin_port = htons(Nport);

    //Demande de connexion
    if (connect(*descripteur_fichier_client, (const struct sockaddr *)&pointeur_socket, sizeof(struct sockaddr_in)) == -1)
    {
        perror("connect");
        exit(-1);
    }
    else
    {
        printf("connection établie \n");
    }
}

void dialogue_serveur(int *descripteur_fichier_client)
{
    char message[10000];
    int entier;


    //message à envoyer
    //message = malloc(sizeof(char*));
    printf("Veuillez taper un message :\n");
    scanf("%[^\n]", message);         //scanne tout sauf les retours à la ligne.
    printf("Message en attente d'envoi... \n");

    //envoi du message
    
    write(*descripteur_fichier_client, message, sizeof(message));
    printf("Message envoyé\n");

    // Reception de l'entier
    read(*descripteur_fichier_client, &entier, sizeof(int));
    printf("Entier reçu du serveur : %d\n", entier);

    //fermeture de la connection
    close(*descripteur_fichier_client);
}
