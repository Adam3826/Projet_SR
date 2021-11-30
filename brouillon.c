#include <stdio.h>

void menu(){
	
	while(1){
		int commande;
		printf("saisir commande. \n-1 pour consulter les fichiers \n-2 pour upload\n-3pour quitter\n");
		scanf("%d", commande);
		switch(commande){
			case 1:
				consulter();
			case 2:
				upload();
			case 3:
				quitter();
			default:
				printf("entrée non  valide. veuillez saisir un entier entre 1 et 3");
				break;


		}
	}





}





void consulter();
