#include "serveur.h"
#include <dirent.h>
#include <stdlib.h>
#include <string.h>


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
  int socketClient;DIR *dirp;
  struct sockaddr_in addresseClient;
  socklen_t taille = sizeof(addresseClient);
  struct sigaction action;
  char chaine_carac[10000];
  int test = 10; //test d'envoi d'entier au client
  char ligne[100];
  FILE *f;

  while (1)
  {
    //Acceptation d'une connexion
    socketClient = accept(*p, (struct sockaddr *)&addresseClient, &taille);
    if (socketClient == -1)
    {
      perror("accept");
      exit(-1);
    }  
    else
    {
      printf("Accept ok\n");
    }

    action.sa_handler = end_child; // le champ sa_handler reçoit le nom du gestionnaire de signal
    action.sa_flags = SA_RESTART;//il permet d'éviter l'interruption de "accept" par la reception de SIGCHILD

    sigaction(SIGCHLD, &action, NULL);// action est associé au signal SIGCHILD

    //Création d'un processus fils
    switch (fork())
    {
    case -1:
      perror("fork");
      exit(-1);

    case 0:
      printf("en attente du Client... \n");
      read(socketClient, &chaine_carac, sizeof(char[10000]));

      if (chaine_carac != NULL)
      {
        printf("message \"%s\" bien reçu \n", chaine_carac);
        write(socketClient, &test, sizeof(int));
      }
	
      read(socketClient, &test, sizeof(int));
      printf("la valeur de test est %d\n",test);

      if(test == 1){ 
	 printf("le serveur a bien recu votre command\n");
	}
	
	
	f = fopen("fic","w");				   //  on écrit les noms fichiers du dossier images dans un fichier  fic  
	DIR *dirp;					  //	
	struct dirent *dp;				  // la focntion opendir permet de la lecture dans un dossier 
	dirp = opendir("images");			 
	dp = readdir(dirp);				
	int i=0;		
	while((dp = readdir(dirp)) !=NULL){		//  lecture de chaque nom de fichier contenue dans le dossier images et l'écrire dans fic
			fputs(dp->d_name,f);
			fputs("\n",f);
			i++;		
	   }
	fclose(f);
	
	
	f = fopen("fic","r");				// lecture du fichier fic par le processus fils  du serveur 		
	int j = 0;
						
        write(socketClient, &i, sizeof(int));		// cette boucle   
        while( j<i  && fgets(ligne,10, f) != NULL){
            fgets(ligne, 10, f);
            write(socketClient, ligne, sizeof(ligne));
            j++;
	} 	
	fclose(f);
	
	

      close(socketClient);
      exit(0);
    default:
      continue;
    }
  }
}

void end_child() //HERE <------------------------------------------------------------------------------------------------------
{
  if (wait(NULL) != -1);
}
