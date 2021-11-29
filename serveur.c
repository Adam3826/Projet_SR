#include<sys/types.h>
#include<sys/socket.h>
#include<netined/in.h>
#include<unistd.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


int main(void){


	//creation du socket du serveur tcp
	int socketDuServeur=socket(AF_INET,SOCK_STREAM,0);

	//une structure qui correspond à l'adrresse du serveur

	struct sockaddr_in addresseDuServeur;

	addresseDuServeur.sin_addr.s_addr=inet_addr("127.0.0.1");
	//adLocale.sin_addr.s_addr = htonl(INADDR_ANY); 
	addresseDuServeur.sin_family = AF_INET;

	//affectation du port 550000
	addresseDuServeur.sin_port = htons (550000);


	//attachement du socket avec bind (a verifier si bind retourne une valeur succes ou echec ainsi que la primitive listen

	 bind(socketDuServeur, (struct sockaddr*) &addresseDuServeur,sizeof(addresseDuServeur));


		if(socketDuServeur==--1){

		//ecoute du serveur maximum 5 client peuvent se connecter
		listen(socketDuServeur,5);

		}

      //lecturre d'un message par le serveur  a cooriger 
read(int descripteur, void *tampon, size_t nbcar); 

printf("je suis le serveur j'ai lu :"tampon);
  
  return 0 ;
}
