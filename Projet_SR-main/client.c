#include "client.h"
#include <dirent.h>

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
    memcpy(&pointeur_socket.sin_addr.s_addr, host_name->h_addr, host_name->h_length);//

    //preparation des champs sin_family, sin_addr et sin_port
    pointeur_socket.sin_addr = *((struct in_addr *)host_name->h_addr);//
    pointeur_socket.sin_family = AF_INET;
    pointeur_socket.sin_port = htons(Nport);

    //Demande de connexion
    if (connect(*descripteur_fichier_client, (const struct sockaddr *)&pointeur_socket, sizeof(struct sockaddr_in)) == -1)// sizeof: la taille de      l'objet pointé par pointeur_socket.
    {
        perror("connect");
        exit(-1);
    }

    else
    {
        printf("connection établie \n");
    }
}


void menu(int *d){

	
	int command  =0;
	while(command !=3){
	 
	printf("-----------------------Menu---------------------------\n");	
	printf("            1. consulter les fichier dans le serveur\n"); 
	printf("            2. deposer un fichier\n");
	printf("            3. Quitter\n"); 	
	
		
	printf("veuillez entrer une requet\n");
	scanf("%d",&command);


		switch(command){
			case 1:
				write(*d,&command, sizeof(int));
				break;
			case 2:
				printf("deposer un fichier\n");
				char message[1000];
				printf("veuillez entrer le fichier a deposer\n");
				scanf("%s",message);
				write(*d,message, sizeof(message));
				break;
			case 3:
				printf("Au revoir\n");
				break;
				
		}
	

		
  	}
}


void dialogue_serveur(int *descripteur_fichier_client){
    char message[10000];
    int entier;

    char tabImages[20][30];
   
    //message à envoyer
    //message = malloc(sizeof(char*));
    printf("Veuillez taper un message :\n");
    scanf("%[^\n]", message);         //scanne tout sauf mm les espaces les retours à la ligne.
    printf("Message en attente d'envoi... \n");

    //envoi du message
    
     write(*descripteur_fichier_client, message, sizeof(message));
    //printf("Message envoyé\n");

    // Reception de l'entier
     

      menu(descripteur_fichier_client);
      read(*descripteur_fichier_client, &entier,sizeof(int));
      printf("Entier reçu du serveur : %d\n", entier);
    
    
      read(*descripteur_fichier_client, tabImages, sizeof(char[20][30]));
      printf("%s\n",tabImages[0]);
    //  printf("%s\n",tabImages[1]);
     // printf("%s\n",tabImages[2]);

 


    //fermeture de la connection
    close(*descripteur_fichier_client);
}
