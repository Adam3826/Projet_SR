all : client/serveur


client/serveur:     client.o serveur.o
            gcc cient.o serveur.o client/serveur.o -o client/serveur

client.o: client.c 
            gcc -c client.c

serveur.o: serveur.c 
            gcc -c serveur.c          

clean : 
            @echo "on efface les fichiers objets"
            rm* .o           

