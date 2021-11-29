#include<sys/types.h>
#include<sys/socket.h>
#include<netined/in.h>
#include<unistd.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


int main (){
//dans un premier temps on utilise 550000 pour tester les port et  on va les parametter  sur la ligne de commande apres
        char t='c';
	int socketDuClient=socket(AF_INET,SOCK_STREAM,0)

	struct hostent* infoSurLeServeur;
	struct sockaddr_in addresseServeur;
	size_t  tailleAddresse = sizeof(addresseServeur);

	addresseServeur.sin_family = AF_INET;  
	addresseServeur.sin_port = htons(550000);  /* Affectation du port destination*/
	addresseServeur.sin_addr= *((struct in_addr *)infoSurLeServeur->h_addr);


	//recuperation des information du serveur via la fonction gethiostbyname qu'on stock dans la variable infoSurLeServeur

	infoSurLeServeur=infosServeur = gethostbyname(addresseServeur);




	//on verifie si la creation du socket a reussit ou pas au cas ou il y a erreur perror est poositionné et on sort totalement de la creation du socket 

	 if ((soc = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket");
		exit(1);
	    }

	// pareil pour la creation de la connection au serveur
	if (connect(socketDuClient, (struct sockaddr *)&server, tailleAddresse ) == -1){
		perror("connect");
		exit(1);
	    }
	//memcpy(&adServeur.sin_addr, infosServeur->h_addr, infosServeur->h_length);



	//communication client serveur


		/*
	while (1) {
			printf("saississez un message (q pour quitter): ");
			bzero(buffer, sizeof(buffer));
			scanf("%s", buffer);
			write(soc, buffer, strlen(buffer));

			// si n saissie q alors on quitte la sexion;

			if (!strcmp(buffer, "q")) break;
			
			bzero(buffer, sizeof(buffer));
			read(soc, buffer, 512);
			printf("%s", buffer);
		    }

		    //fermeture serveur
		    shutdown(soc,2);
		    close(soc);

	  */
	  
	//conversion de la taille du  mot en format reseau
	 size_t tailleducaracterre=htonl(strlen(t));



	//

	write(socketDuServeur, &tailleducaracterre, sizeof(long));   /* Envoie taille de la lettre et de la lettre*/
	write(socketDuServeur, t, tailleducaracterre);    

	  
	  return 0;
	}
