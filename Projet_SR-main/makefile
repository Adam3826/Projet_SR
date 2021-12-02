
CC = gcc 

all : serveur_main client_main

serveur.o : serveur.c serveur.h
	$(CC) -c serveur.c 

client.o : client.c client.h
	$(CC) -c client.c  

serveur_main.o : serveur_main.c serveur.h
	$(CC) -c serveur_main.c 

client_main.o : client_main.c client.h
	$(CC) -c client_main.c

serveur_main : serveur.o serveur_main.o
	$(CC) $^ -o $@

client_main : client.o client_main.o 
	$(CC) $^ -o $@
  
clean : 
	rm -rf *.o serveur_main client_main