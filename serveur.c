#include "serveur.h"

void initialisation(int *p, int noport)
{
  struct sockaddr_in s;

  // Creation du socket de type stream
  *p = socket(AF_INET, SOCK_STREAM, 0);

  if (*p == -1)  //descripteur non créé
  {
    perror("socket");
    exit(-1);
  }
  else
  {
    printf("Descripteur créé : %d\n", &p);
  }

  //preparation des champs sin_family, sin_addr et sin_port
  s.sin_addr.s_addr = htonl(INADDR_ANY);
  s.sin_family = AF_INET;
  s.sin_port = htons(noport);

  //Attachement du socket à l'adresse
  if (bind(*p, (const struct sockaddr *)&s, sizeof(s)) == -1) //si bind echoue
  {
    perror("erreur de bind");
    exit(-1);
  }
  else
  {
    printf("Bind succes \n");
  }

  //ouverture du service
  if (listen(*p, 2) == -1)
  {
    perror("Listen");
    exit(-1);
  }
  else
  {
    printf("Listening\n");
  }
}

void dialogueClient(int *p)
{
  int socketClient;
  struct sockaddr_in addresseClient;
  int  taille = sizeof(addresseClient);
  struct sigaction ac;
  char *chaine_carac;
  int entier = 2;

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
      printf("client accepted\n");
    }

    ac.sa_handler = end_child;
    ac.sa_flags = SA_RESTART;

    sigaction(SIGCHLD, &ac, NULL);

    //Création d'un processus fils
    switch (fork())
    {
    case -1:
      perror("fork");
      exit(-1);

    case 0:
      printf("en attente du message Client... \n");
      recv(socketClient, &chaine_carac, sizeof(chaine_carac), 0);

      if (chaine_carac != NULL)
      {
        printf("message \"%s\" bien reçu \n", &chaine_carac);
        send(socketClient, &entier, sizeof(int), 0);
      }

      close(socketClient);
      exit(0);
    default:
      continue;
    }
  }
}

void end_child()
{
  while (wait(NULL) != -1)
    ;
}
