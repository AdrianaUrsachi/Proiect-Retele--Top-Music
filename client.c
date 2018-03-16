#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include<signal.h>
#include<fcntl.h>

extern int errno;


int port;

void SIGQUIT_handler(int sig)
{
   exit(1);
}

int main (int argc, char *argv[])
{

  int sd;                       // descriptorul de socket
  struct sockaddr_in server;    // structura folosita pentru conectare 
  char msgintrare[1000],msg[100];                // mesajul trimis

  /* exista toate argumentele in linia de comanda? */
  if (argc != 3)
    {
      printf ("[client] Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  /* stabilim portul */
  port = atoi (argv[2]);

  /* cream socketul */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[client] Eroare la socket().\n");
      return errno;
    }


  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons (port);

  /* ne conectam la server */
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }

  int fiu;
  if(!(fiu=fork()))
    {
       while(1)
        {
             /* citirea mesajului */
             bzero (msgintrare, 1000);
             if (read (sd, msgintrare, 1000) < 0)
                {
                   perror ("[client]Eroare la read() de la server.\n");
                   return errno;
                }
             /* afisam mesajul primit */
             printf ("%s \n", msgintrare);
        }
     }
   else
     {
        while(1)
          {
             fflush (stdout);
             bzero(msg,100);
             read (0, msg, 100);

             /* trimiterea mesajului la server */
             if (write (sd, msg, strlen(msg)) <= 0)
                {
                  perror ("[client]Eroare la write() spre server.\n");
                  return errno;
                }
             if(strstr(msg,"quit")!=NULL)
                {
                   kill(fiu,SIGQUIT);
                   exit(1);
                }
           }
      }

  /* inchidem conexiunea, am terminat */
  close (sd);
}


