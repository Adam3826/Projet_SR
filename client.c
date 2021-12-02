#include "client.h"


void initialisationConnection(int *descripteur_fichier_client, char *hostName, int Nport){
   
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


	char message_a_envoyer[100];
	char message_a_recevoir[100];
	int entier_a_envoyer = 42;
	int entier_a_recevoir;


	printf("Veuillez entrer un message :\n");
	scanf("%[^\n]", message_a_envoyer);         //scanne tout sauf les retours à la ligne.



	// test envoi d'un string
	write(*descripteur_fichier_client, &message_a_envoyer, sizeof(char[100]));
	printf("Message envoyé\n");


	// test Reception d'un string
	read(*descripteur_fichier_client, &message_a_recevoir, sizeof(char[100]));
	printf("le serveur a répondu \"%s\"\n", message_a_recevoir);

	// test Reception d'un entier
	read(*descripteur_fichier_client, &entier_a_recevoir, sizeof(int));
	printf("Entier reçu du serveur : %d\n", entier_a_recevoir);

	// test envoi d'un entier
	write(*descripteur_fichier_client, &entier_a_envoyer, sizeof(int));
	//fermeture de la connection
	close(*descripteur_fichier_client);
}















