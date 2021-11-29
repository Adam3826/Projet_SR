#include "client.h"

void initialisation(int *p, char *hostName, int noport)
{
    struct hostent *h;
    struct sockaddr_in s;
    
    //strcpy(hostName, argv[1]);

    //creation du socket
    *p = socket(AF_INET, SOCK_STREAM, 0);

    if (*p == -1)
    {
        perror("socket");
        exit(-1);
    }
    else
    {
        printf("Descripteur créé : %d\n", &p);
    }

    //récupération dans h des informations concernant le serveur
    h = gethostbyname(hostName);
    //memcpy(&s.sin_addr.s_addr, h->h_addr, h->h_length);

    //preparation des champs sin_family, sin_addr et sin_port
    s.sin_addr = *((struct in_addr *)h->h_addr);
    s.sin_family = AF_INET;
    s.sin_port = htons(noport);

    //Demande de connexion
    if (connect(*p, (const struct sockaddr *)&s, sizeof(s)) == -1)
    {
        perror("connect");
        exit(-1);
    }
    else
    {
        printf("connect succes \n");
    }
}

void dialogue(int *p)
{
    char *message;
    int entier;


    //message à envoyer
    message = malloc(sizeof(char*));
    printf("Veuillez entrer un message!!\n");
    scanf("%s", message);
    printf("Message en attente d'envoi \n");

    //envoi du message
    
    send(*p, message, sizeof(message), 0);
    printf("Message envoyé avec succes!!!\n");

    // Reception de l'entier
    recv(*p, &entier, sizeof(int), 0);
    printf("Entier recu : %d\n", entier);

    //fermeture de la connection
    close(*p);
}