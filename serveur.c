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
  char nom_de_fichier[MAX_SIZE_NAME];
  char mimefic[80];
  int requete; 
  char ligne[100];
  FILE *f;
  FILE *fichier_1;
  FILE *fichier_2;
  char ligne_a_envoyer[MAX_SIZE_NAME];	
  int taille_ligne=50;

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
      printf("Client accepté\n");
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
	
      read(socketClient, &requete, sizeof(int));    //reception de la command
      printf("requête n°%d\n",requete);

    int i = 0;
      if(requete == 1){ 
      
		printf("le serveur a bien recu votre requête\n");
		
		f = fopen("fic","w");	
		DIR *pointeur_repertoire;				 
		struct dirent *repertoire;			 // la focntion opendir permet de la lecture dans un dossier 
		pointeur_repertoire = opendir("images");			 
		repertoire = readdir(pointeur_repertoire);		
			
		while((repertoire = readdir(pointeur_repertoire)) !=NULL){     //lecture de chaque nom de fichier contenue dans le dossier images et l'écrire dans buffer      	
			fputs(repertoire->d_name,f);
			i++; 
	   	      }
	   	 fclose(f);     
	   	      
		write(socketClient,&i,sizeof(int));		
		
		DIR *pointeur_repertoire2;
		struct dirent *repertoire2;
		pointeur_repertoire2 = opendir("images");
		repertoire2 = readdir(pointeur_repertoire2);			 
		while((repertoire2 = readdir(pointeur_repertoire2)) !=NULL){             //lecture de chaque nom de fichier contenue dans le dossier images et l'écrire dans buffer
			          	
			strcpy(ligne_a_envoyer,repertoire2->d_name);            //  ecriture les noms de fichiers  du dossier images dans le fichier buffer
			printf("ligne_a_envoyer = %s\n",ligne_a_envoyer);
			write(socketClient, ligne_a_envoyer,sizeof(char[MAX_SIZE_NAME]));  
			
	   	      }
	

		
	}
	
	
      if (requete ==2){
      		int i=0;	
      		read(socketClient, &i, sizeof(int));
      		int j =0;
      		while (j<i){
      		   read(socketClient, &nom_de_fichier, sizeof(nom_de_fichier));	 // récupérer le fichier que le client nous a envoyer 
      		   printf("%s\n",nom_de_fichier);	// affichier le nom du fichier a analyser pour savoir si, il est admissible 
      		    
      		   switch(fork()){
      		    case -1:
      		    		 perror("fork");
     				 exit(-1);
		    case 0:      		    
      		    		fichier_1 = freopen("mime_fichier.txt","r+",stdout); // rediriger la sortie de la focntion execlp vers le fichier "mime_fichier.txt"     
      		    		if(execlp("file","file","-b","--mime-type",nom_de_fichier,(char*) 0) == -1){   //récuperer  le mime-type  du fichier  que le client a envoyer et le rediriger vert le fichier mime_fichier.txt 
      		    			perror("execlp");
      		    			exit(-1);
      		   		 }
      	
      		   default:
      		    	fichier_1 = fopen("mime_fichier.txt","r");
      		    	fichier_2 = fopen("mime", "r");
      		    		
      		    	fgets(mimefic,30,fichier_1);            		// mettre le contenue de mime_fichier.txt dans un tabeau de caracteres  "mimefic"
      		    	int vrai = 0;
      			while(fgets(ligne,50,fichier_2) != NULL){      		// On lit 50 caractères du fichier_2, on stocke le tout dans "ligne"
      			   if (strcmp(mimefic,ligne) ==0){     			  // comparer le mime du fichier envoyer par le client  avec les mimes admissibles
      					printf("le fichier est admissible\n");
      					vrai =1;                       			// mettre la varaible vrai egale a 1 si  le fichier est admissible
      					}		
      				
      				}
      							    		
      			if (vrai != 1){
      			 	printf("le fichier n'est pas admissible\n");
      			       /* envoyer un message au client pour l informer que le fichier n est pas admissible*/
      			       	unlink(nom_de_fichier);
      			}
      			else{ 
      			  	int fic;
	 			char dossier[20] = "images/";
	 			read(socketClient, &nom_de_fichier, sizeof(nom_de_fichier));
	 			strcat(dossier,nom_de_fichier);
	 			printf("je suis dans le serveur le fichier que le client m'a envoyer est %s\n",dossier);
	 			fic = open(dossier,O_CREAT,0644);
	 			close(fic);
	 			
	 			f = fopen(dossier,"w");	
	 			while(read(socketClient, &ligne_a_envoyer, sizeof(ligne_a_envoyer))){  // On lit 50 caractères du fichier_2, on stocke le tout dans "ligne"
      						
      		 		     	printf("ligne_a_envoyer : %s",ligne_a_envoyer);		
      		      			fputs(ligne_a_envoyer,f);  
      			          				 				
      					}
					fclose(f);
				}
				
      		    		fclose(fichier_1);
      		    		fclose(fichier_2);
      		    		
      		    		
      		    }     		    
		j++;
      		}
     	 }	


	if (requete == 3){
	 	int fic;
	 	char dossier[20] = "images/";
	 	read(socketClient, &nom_de_fichier, sizeof(nom_de_fichier));
	 	strcat(dossier,nom_de_fichier);
	 	printf("fichier que le client a envoyé : %s\n",dossier);
	 	fic = open(dossier,O_CREAT,0644);
	 	close(fic);
	 	
	 	
	 	
	 	f = fopen(dossier,"w");	
	 	while(read(socketClient, &ligne_a_envoyer, sizeof(ligne_a_envoyer))){  // On lit 50 caractères du fichier_2, on stocke le tout dans "ligne"
      						
      		      printf("ligne_a_envoyer : %s",ligne_a_envoyer);		
      		      fputs(ligne_a_envoyer,f);  
      			          				 				
      		}
		fclose(f);
	
	}
	if(requete == 4){
		//fin de protocole
		exit(0);
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



