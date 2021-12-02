#include "serveur.h"



void initialisation_connection(int *descripteur_fic, int Nport)
{
  struct sockaddr_in structure_socket;  //pointeur sur la structure qui correspond à l'adresse à laquelle la socket est attachée (serveur) 

  // Creation du socket de type stream
  *descripteur_fic = socket(AF_INET, SOCK_STREAM, 0);

  if (*descripteur_fic == -1)
  {
    perror("socket");
    exit(-1);
  }
  else
  {
    printf("Descripteur créé : %d\n", *descripteur_fic);
  }

  //preparation des champs sin_family, sin_addr et sin_port
  structure_socket.sin_addr.s_addr = htonl(INADDR_ANY);
  structure_socket.sin_family = AF_INET;
  structure_socket.sin_port = htons(Nport);

  //Attachement du socket à l'adresse
  if (bind(*descripteur_fic, (const struct sockaddr *)&structure_socket, sizeof(struct sockaddr_in)) == -1) //
  {
    perror("bind");
    exit(-1);
  }
  else
  {
    printf("Bind succes \n");
  }

  //ouverture du service
  if (listen(*descripteur_fic, 2) == -1)
  {
    perror("Listen");
    exit(-1);
  }
  else
  {
    printf("Listen ok\n");
  }
}

void dialogue_client(int *p)
{
  int socketServeur;
  struct sockaddr_in addresseClient;
  socklen_t taille = sizeof(addresseClient);
  struct sigaction action;
  


  while (1)
  {
    //Acceptation d'une connexion
    socketServeur = accept(*p, (struct sockaddr *)&addresseClient, &taille);
    if (socketServeur == -1)
    {
      perror("accept");
      exit(-1);
    }
    else
    {
      printf("Accept ok\n");
    }

    action.sa_handler = end_child;      //HERE <------------------------------------------------
    action.sa_flags = SA_RESTART;

    sigaction(SIGCHLD, &action, NULL);

    //Création d'un processus fils
    switch (fork())
    {
    case -1:
      perror("fork");
      exit(-1);

    case 0:
      
;	char message_du_client[100];
	char message_pour_le_client[100];
	int entier_du_client;
	int entier_pour_client = 26;
	
	strcpy(message_pour_le_client, "Le serveur vous remercie pour ce message");
	
	//TEST DE RÉCÉPTION D'UN STRING
	read(socketServeur, &message_du_client, sizeof(char[100]));
	printf("message du client : %s\n",message_du_client);
	//test envoi d'un string
	write(socketServeur, &message_pour_le_client, sizeof(char[100]));
	
	//test d'envoi d'un entier
	write(socketServeur, &entier_pour_client, sizeof(int));

	//test de récéption d'un entier
	read(socketServeur, &entier_du_client, sizeof(int));
	printf("entier du client : %d\n",entier_du_client);
      close(socketServeur);
      exit(0);
    default:
      continue;
    }
  }
}




void end_child() //HERE <------------------------------------------------------------------------------------------------------
{
  if (wait(NULL) != -1)
    ;
}

