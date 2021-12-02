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
  char mimefic[80];
  int test = 10; //test d'envoi d'entier au client
  char ligne[100];
  FILE *f;
  FILE *fichier_1;
  FILE *fichier_2;


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
      }
	
      read(socketClient, &test, sizeof(int));    //reception de la command
      printf("la valeur de test est %d\n",test);

 //
      if(test == 1){ 
		printf("le serveur a bien recu votre command\n");
		f = fopen("fic","w");				   //  on écrit les noms fichiers du dossier images dans un fichier  fic  
		DIR *dirp;					  //	
		struct dirent *dp;				  // la focntion opendir permet de la lecture dans un dossier 
		dirp = opendir("images");			 
		dp = readdir(dirp);				
		int i=0;		
		while((dp = readdir(dirp)) !=NULL){		//  lecture de chaque nom de fichier contenue dans le dossier images et l'écrire dans f
			fputs(dp->d_name,f);            //  ecriture les noms de fichiers  du dossier images dans le fichier f
			fputs("\n",f);
			i++;		
	   	   }
		fclose(f);
	
	
		f = fopen("fic","r");				// lecture du fichier fic par le processus fils  du serveur 		
		int j = 0;
						
        	write(socketClient, &i, sizeof(int));		// l'envoie de la taille du dossier images 
        
        	while( j<i  && fgets(ligne,20, f) != NULL){      // cette boucle   sert  a envoyer le contenus du fichier dont on n'a stocker les noms des fihiers image du dossier images 
          	fgets(ligne, 20, f);		         // lecture a partir du fichier 
            	write(socketClient, ligne, sizeof(ligne));   // envoie  du contenue du fichier
            	j++;
		} 	
		fclose(f); 
	}
	
      if (test ==2){
      		int i=0;	
      		read(socketClient, &i, sizeof(int));
      		int j =0;
      		while (j<i){
      		    read(socketClient, &chaine_carac, sizeof(chaine_carac));	
      		    printf("%s\n",chaine_carac);
      		    
      		    switch(fork()){
      		    	case -1:
      		    		 perror("fork");
     				 exit(-1);
			case 0:      		    
      		    		fichier_1 = freopen("mime_fichier.txt","r+",stdout); 		            // rediriger la sortie de la focntion execlp vers le fichier "mime_fichier.txt"     
      		    		if(execlp("file","file","-b","--mime-type",chaine_carac,(char*) 0) == -1){  //récuperer un le mime-type  que le client a envoyer 
      		    			perror("execlp");
      		    			exit(-1);
      		   		 }
      	
      		   	default:
      		    		fichier_1 = fopen("mime_fichier.txt","r");
      		    		fichier_2 = fopen("mime", "r");
      		    		
      		    		fgets(mimefic,30,fichier_1);
      		    		int vrai = 0;
      				while(fgets(ligne,50,fichier_2) != NULL){  // On lit 50 caractères du fichier_2, on stocke le tout dans "ligne"
      					if (strcmp(mimefic,ligne) ==0){
      						printf("le fichier est admissible\n");
      						vrai =1;
      					}		
      				
      				}			    		
      				if (vrai != 1){
      			 		printf("le fichier n'est pas admissible\n");
      			 		
      				} 
      				
      		    		fclose(fichier_1);
      		    		fclose(fichier_2);
      		    		
      		    		
      		    }     		    
		j++;
      		}
     	 }	




	
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
