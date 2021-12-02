#include "serveur.h"
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/stat.h>

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
  char buffer[1000];	
  int taille_ligne=8;
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
                printf("taile du dossier %d",i);
        	while( j<i && fgets(ligne,32, f)!= NULL){
        	                            // cette boucle   sert  a envoyer le contenus du fichier dont on n'a stocker les noms des fihiers image du dossier images 
          		//fgets(ligne,32, f);
          		taille_ligne = strlen(ligne) -1;     
          		printf("taille de la ligne %d\n",taille_ligne);		
          		write(socketClient, &taille_ligne, sizeof(int)); 		    	   	        // lecture a partir du fichier 
            		write(socketClient, ligne, sizeof(ligne));   // envoie  du contenue du fichier
            		j++;
		} 
		

	}
	
      if (test ==2){
      		int i=0;	
      		read(socketClient, &i, sizeof(int));
      		int j =0;
      		while (j<i){
      		    read(socketClient, &chaine_carac, sizeof(chaine_carac));	 // récupérer le fichier que le client nous a envoyer 
      		    printf("%s\n",chaine_carac);				 // affichier le nom du fichier a analyser pour savoir si, il est admissible 
      		    
      		    switch(fork()){
      		    	case -1:
      		    		 perror("fork");
     				 exit(-1);
			case 0:      		    
      		    		fichier_1 = freopen("mime_fichier.txt","r+",stdout); 		            // rediriger la sortie de la focntion execlp vers le fichier "mime_fichier.txt"     
      		    		if(execlp("file","file","-b","--mime-type",chaine_carac,(char*) 0) == -1){   //récuperer  le mime-type  du fichier  que le client a envoyer et le rediriger vert le fichier mime_fichier.txt 
      		    			perror("execlp");
      		    			exit(-1);
      		   		 }
      	
      		   	default:
      		    		fichier_1 = fopen("mime_fichier.txt","r");
      		    		fichier_2 = fopen("mime", "r");
      		    		
      		    		fgets(mimefic,30,fichier_1);                // mettre le contenue de mime_fichier.txt dans un tabeau de caracteres  "mimefic"
      		    		int vrai = 0;
      				while(fgets(ligne,50,fichier_2) != NULL){      // On lit 50 caractères du fichier_2, on stocke le tout dans "ligne"
      					if (strcmp(mimefic,ligne) ==0){        // comparer le mime du fichier envoyer par le client  avec les mimes admissibles
      						printf("le fichier est admissible\n");
      						vrai =1;                       // mettre la varaible vrai egale a 1 si  le fichier est admissible
      					}		
      				
      				}
      							    		
      				if (vrai != 1){
      			 		printf("le fichier n'est pas admissible\n");
      			 		/* envoyer un message au client pour l informer que le fichier n est pas admissible*/
      				}
      				else{ 
      					int fic;
	 				char dossier[20] = "images/";        //vratiable pour stocker le fichier dans le dossier image  
	 				strcat(dossier,chaine_carac);        // concatener le dossier avec le nom du fichier afin de crée le fichier dans le dossier images
	 				printf("je suis dans le serveur le fichier que le client m'a envoyer est %s\n",dossier);
	 
	 
	 				fic = open(dossier,O_CREAT,0644);  // creation de fichier dans le dossier images 
	 				close(fic);                        // ferme le fichier   
	 	
	 				
	 				
	 				// lire le contenus du fichier envoyer depuis le client est le recopier dans le fichier crée dans images	
			 		 f = fopen(dossier,"w");	          
			 		 while(read(socketClient, &buffer, sizeof(buffer))){  // On lit 50 caractères du fichier_2, on stocke le tout dans "ligne"		
      		      			      fputs(buffer,f);  
      					 }
					fclose(f);
				}
				
      		    		fclose(fichier_1);
      		    		fclose(fichier_2);
      		    		
      		    		
      		    }     		    
		j++;
      		}
     	 }	


	if (test == 3){
	 	int fic;
	 	char dossier[20] = "images/";
	 	read(socketClient, &chaine_carac, sizeof(chaine_carac));
	 	strcat(dossier,chaine_carac);
	 	printf("je suis dans le serveur le fichier que le client m'a envoyer est %s\n",dossier);
	 	fic = open(dossier,O_CREAT,0644);
	 	close(fic);
	 	
	 	
	 	
	 	f = fopen(dossier,"w");	
	 	printf("hhhhhhhhhhhhhhh\n");
	 	while(read(socketClient, &buffer, sizeof(buffer))){  // On lit 50 caractères du fichier_2, on stocke le tout dans "ligne"
      						
      		      printf("buffer : %s",buffer);		
      		      fputs(buffer,f);  
      			          				 				
      		}
		fclose(f);
	
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
