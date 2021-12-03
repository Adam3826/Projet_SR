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


int menu(int *d){

	
	int requete  =0;

	 
	printf("-----------------------Menu---------------------------\n\n");
	printf("veuillez entrer une requête\n");	
	printf("            1. consulter les fichier dans le serveur\n"); 
	printf("            2. deposer un fichier\n");
	printf("            3. recuperer un fichier\n"); 	
	printf("            4. Quitter\n"); 	
	
		

	scanf("%d",&requete);


		switch(requete){
			case 1:
				write(*d,&requete, sizeof(int));
				break;
			case 2:
				printf("Veuillez entrer le nombre de fichiers à deposer\n");
				break;
			case 3:
				printf("Télécharger un fichier\n");
				break;
			case 4:
				printf("*************************************************************\n**************************Au revoir**************************\n****************************************************\n");
				break;
				
		}
	

	return requete;		
  	
}


void dialogue_serveur(int *descripteur_fichier_client){
    char nom_fichier[MAX_SIZE_NAME];
    int entier;
    FILE *f;
    char ligne_recu[MAX_SIZE_NAME];
    int requete = 0;
    int nbfic_a_deposer = 0;


    // Reception de l'entier
     

      requete = menu(descripteur_fichier_client);
      write(*descripteur_fichier_client,&requete,sizeof(int)); // envoie le code de la command 
    
  
     if (requete == 1){
      		int n =0;
      		printf("------------Liste des fichiers disponibles-------------\n");	
	        read(*descripteur_fichier_client,&entier,sizeof(int)); 

      	       while((n != entier)){
			 read(*descripteur_fichier_client,&ligne_recu,256);
      	       		printf("-----------> : %s\n",ligne_recu);
      	       		n++;
						
      		  }		
    	      }
    	      	
    	      			
      if (requete == 2){
      

      		scanf("%d",&nbfic_a_deposer);
      		write(*descripteur_fichier_client,&nbfic_a_deposer,sizeof(int));
      		int i =0;
      		while(i<nbfic_a_deposer){
      							
      			printf("Veuillez saisir le nom du fichier %d\n",i+1);
      			scanf("%s", nom_fichier);
      			write(*descripteur_fichier_client,nom_fichier,sizeof(nom_fichier));  							
      			i++;
      		}
      		   //printf("Veuillez entrer le nom du fichier à telecharger\n");
      		   //scanf("%s",nom_fichier);
      		   //write(*descripteur_fichier_client,message,sizeof(message));
      		f = fopen(nom_fichier,"r");		
      		while(fgets(ligne_recu,200,f) != NULL){  // On lit 50 caractères du fichier_2, on stocke le tout dans "ligne"
      		        write(*descripteur_fichier_client,ligne_recu,sizeof(ligne_recu));					
      		}	
      			     	
      }
      
      if (requete == 3){
      	  
      	  	printf("Veuillez entrer le nom du fichier à telecharger\n");
      		scanf("%s",nom_fichier);
      		write(*descripteur_fichier_client,nom_fichier,sizeof(nom_fichier));
      		f = fopen(nom_fichier,"r");		
      		while(fgets(ligne_recu,200,f) != NULL){  // On lit 50 caractères du fichier_2, on stocke le tout dans "ligne"
      		        write(*descripteur_fichier_client,ligne_recu,sizeof(ligne_recu));
      			printf(" ligne_recu = %s", ligne_recu);							
      		}	
      		
      		
      		fclose(f);
      				
      }		
	
 
    //fermeture de la connection
    close(*descripteur_fichier_client);
}
