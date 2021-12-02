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

	
	int command  =0;

	 
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
				printf("Veuillez donner le nombre de fichier a deposer jjjjjjjjj\n");
				break;
			case 3:
				printf("Telecharger un fichier\n");
				break;
			case 4:
				printf("Au revoir\n");
				break;
				
		}
	

	return command;		
  	
}


void dialogue_serveur(int *descripteur_fichier_client){
    char message[10000];
    int entier;
    FILE *f;
    char ligne_recu[1000];
     char *ligne_recu2;
    int command = 0;
    int nbfic_a_deposer = 0;
    int taille_ligne =8;

    //message à envoyer
    //message = malloc(sizeof(char*));
    printf("Veuillez taper un message :\n");
    scanf("%[^\n]", message);         //scanne tout sauf mm les espaces les retours à la ligne.
    printf("Message en attente d'envoi... \n");

    //envoi du message
    
     write(*descripteur_fichier_client, message, sizeof(message));
    //printf("Message envoyé\n");

    // Reception de l'entier
     

      command = menu(descripteur_fichier_client);
      write(*descripteur_fichier_client,&command,sizeof(int)); // envoie le code de la command 
    
  
     if (command == 1){
      		read(*descripteur_fichier_client,&entier, sizeof(int));                      //reception de la taille du dossier images 
      		printf("taile du dossier %d",entier);
      		printf("------------La liste des fichier images disponible-------------\n");
      		
      		
     	       int j=0;
      	       while(j<entier){ 
      	       
      	       read(*descripteur_fichier_client,&taille_ligne, sizeof(int));
      	       printf("taille de la ligne %d\n",taille_ligne);   							
      	       ligne_recu2 = (char *)malloc(sizeof(char *) * taille_ligne);      							//  boucle sur la taille du dossier images 
      	       read(*descripteur_fichier_client,ligne_recu2, sizeof(ligne_recu2));	//  faire le read a chaque envoie du server 
      	       printf(" ----> %s\n", ligne_recu2);					// affichage  du contenue lu 
    	       j++;
	   
      		}
    	      }
    	      			
      if (command == 2){
      

      		scanf("%d",&nbfic_a_deposer);
      		write(*descripteur_fichier_client,&nbfic_a_deposer,sizeof(int));
      		int i =0;
      		while(i<nbfic_a_deposer){
      							
      			printf("Veuillez entre le fichier %d\n",i+1);
      			scanf("%s", message);
      			write(*descripteur_fichier_client,message,sizeof(message));  							
      			i++;
      		}
      		   //printf("Veuillez entrer le nom du fichier à telecharger\n");
      		   //scanf("%s",message);
      		   //write(*descripteur_fichier_client,message,sizeof(message));
      		f = fopen(message,"r");		
      		while(fgets(ligne_recu,200,f) != NULL){  // On lit 50 caractères du fichier_2, on stocke le tout dans "ligne"
      		        write(*descripteur_fichier_client,ligne_recu,sizeof(ligne_recu));					
      		}	
      			     	
      }
      
      if (command == 3){
      	  
      	  	printf("Veuillez entrer le nom du fichier à telecharger\n");
      		scanf("%s",message);
      		write(*descripteur_fichier_client,message,sizeof(message));
      		f = fopen(message,"r");		
      		while(fgets(ligne_recu,200,f) != NULL){  // On lit 50 caractères du fichier_2, on stocke le tout dans "ligne"
      		        write(*descripteur_fichier_client,ligne_recu,sizeof(ligne_recu));
      			printf(" ligne_recu = %s", ligne_recu);							
      		}	
      		
      		
      		fclose(f);
      				
      }		
	
 
    //fermeture de la connection
    close(*descripteur_fichier_client);
}
