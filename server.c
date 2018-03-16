#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>

/* portul folosit */
#define PORT 2024

extern int errno;

int verifica(char user[100])
{
 int gasit=0,i,nrLinii;
 char usersFisier[100][100];
 FILE *file=fopen("usersRes.txt","r");

  i=0;
  while(fgets(usersFisier[i],sizeof(usersFisier[i]),file)!=NULL)
          {
            usersFisier[i][strlen(usersFisier[i])-1]='\0';
            i++;
          }
      nrLinii=i;
  fclose(file);

 for(i=0;i<nrLinii;i++)
    if(strcmp(usersFisier[i],user)==0)
       gasit=1;

 return gasit;
}

int login(int client)
{
  int logat=0,admin=0,i,nrLinii;
  char msg[100],msgrasp[100],msgintrare[100],usersFisier[100][100],userPrimit[100];

  FILE *file=fopen("users.txt","r");

  i=0;
  while(fgets(usersFisier[i],sizeof(usersFisier[i]),file)!=NULL)
          {
            usersFisier[i][strlen(usersFisier[i])-1]='\0';
            i++;
          }
      nrLinii=i;
  fclose(file);

  bzero(msgintrare,100);
  strcat(msgintrare,"Selecteaza:\n");
  strcat(msgintrare,"inregistrare username\n");
  strcat(msgintrare,"login username\n");

  if (write (client, msgintrare, strlen(msgintrare)) <= 0)
    {
      perror ("[server]Eroare la write() catre client din login.\n");
                 /* continuam sa ascultam */
    }
  while(logat==0)
  {

   bzero (msg, 100);
   printf ("[server]Asteptam mesajul...\n");
   fflush (stdout);

   /* citirea mesajului */
   if (read (client, msg, 100) <= 0)
   {
       perror ("[server]Eroare la read() de la client.\n");
       close (client);   /* inchidem conexiunea cu clientul */
       continue;         /* continuam sa ascultam */
   }
  printf("%s \n",msg);
  msg[strlen(msg)-1]='\0';
   if(strcmp(msg,"quit")==0)
     exit(0);
   else
     if(strstr(msg,"inregistrare")!=NULL)
     {
       strcpy(userPrimit,msg+13);
     	file=fopen("users.txt","a");
	fprintf(file,userPrimit,strlen(userPrimit));
	fprintf(file,"\n");
	fclose(file);
	strcpy(usersFisier[nrLinii],userPrimit);
	nrLinii++;
	bzero(msgrasp,100);
	strcat(msgrasp,"Te-ai inregistrat cu succes, urmeaza login");
	if (write (client, msgrasp, 100) <= 0)
           {
             perror ("[server]Eroare la write() catre client din login.\n");
             continue;           /* continuam sa ascultam */
           }
         else
          printf ("[server]Mesajul a fost trasmis cu succes.\n");
      }
     else
        if(strstr(msg,"login")!=NULL)
          {
            strcpy(userPrimit,msg+6);
            int continua=1;
            i=0;
            while(i<nrLinii && continua==1)
            {
               if(strcmp(userPrimit,usersFisier[i])==0)
                   continua=0;
               else
                  i++;
            }

            if(continua==0)
               logat=1;

            if(logat==1)
              {
                 if(strcmp(userPrimit,"adriana")==0)
                   admin=1;
                 else
                     if(verifica(userPrimit)==1)
                        admin=2;//are accesul la vot restrictionat
		char mesajAux[1000];
                 bzero(mesajAux,1000);
                 strcat(mesajAux,"Hello ");
                 strcat(mesajAux,userPrimit);

                 if(admin==1)
                    {
			strcat(mesajAux,"! Te-ai logat cu succes ca si administrator!\n");
			strcat(mesajAux,"Comezile pe care le poti executa sunt:\n add in top \n add comment \n afiseaza top\n afiseza comentarii\n voteaza\n sterge\n restrictioneaza vot\n quit \n");
			}
			else
			{
                    strcat(mesajAux,"! Te-ai logat cu succes!\n");
	 		strcat(mesajAux,"Comezile pe care le poti executa sunt:\n add in top \n add comment \n afiseaza top\n afiseaza comentarii\n voteaza\n quit \n");
			}
                printf("[server]Trimitem mesajul inapoi...%s\n",mesajAux);

                 /* returnam mesajul clientului */
                 if (write (client, mesajAux, strlen(mesajAux)) <= 0)
                   {
                      perror ("[server]Eroare la write() catre client din login.\n");
                      continue;           /* continuam sa ascultam */
                   }
                 else
                      printf ("[server]Mesajul a fost trasmis cu succes.\n");

              }
            if(logat==0)
               {
                  bzero(msgrasp,100);
                  strcat(msgrasp,"Nu ati introdus un nume de utilizator valid\n");

                  printf("[server]Trimitem mesajul inapoi...%s\n",msgrasp);

                  /* returnam mesajul clientului */
                  if (write (client, msgrasp, 100) <= 0)
                    {
                        perror ("[server]Eroare la write() catre client din login.\n");
                        continue;           /* continuam sa ascultam */
                    }
                 else
                    printf ("[server]Mesajul a fost trasmis cu succes.\n");
               }
           }//login
          else
             {
              bzero(msgrasp,100);
              strcat(msgrasp,"Ati introdus o comanda gresita\n");

              if(write(client,msgrasp,strlen(msgrasp))<=0)
                {
                  perror("[server]Eroare la write() catre client din login.\n");
                  continue;
                }
              else
                 printf("[server]Mesajul a fost transmis cu succes.\n");

             }
  }//while

  return admin;
}


void sortareFisier(char fisier[100])
{
  FILE * file;
  file=fopen(fisier,"r");
  char mat[100][100];
  int i,nrLinii,j,k,m;
  i=0;
  while(fgets(mat[i],sizeof(mat[i]),file)!=NULL)
  {
     mat[i][strlen(mat[i])-1]='\0';
     i++;
  }
  nrLinii=i;

 file=fopen(fisier,"w");

 char aux[100],a[5],b[5];
 for(i=0;i<nrLinii-1;i++)
  {
    k=0;
    while(mat[i][k]!='~')
      {
        a[k]=mat[i][k];
        k++;
      }
    a[k]='\0';
   for(j=i+1;j<nrLinii;j++)
     {
       m=0;
       while(mat[j][m]!='~')
        {
          b[m]=mat[j][m];
          m++;
        }
       b[m]='\0';
       if(atoi(a)<atoi(b))
         {
           strcpy(aux,mat[i]);
           strcpy(mat[i],mat[j]);
           strcpy(mat[j],aux);
         }
     }

  }
  file=fopen(fisier,"a");
  for(i=0;i<nrLinii;i++)
   {
      printf("%s\n",mat[i]);
      fflush(stdout);
      fputs(mat[i],file);
      fputc('\n',file);
   }

  fclose(file);
}

void adaugareInTop(int client)
{
  char msgintrare[1000],msg[100],msgraspuns[1000];
  int fisier;
  bzero(msgintrare,1000);
  strcat(msgintrare,"Alege topul:\ndance\nrock\nhip-hop\nclasica\n");

  if (write (client, msgintrare, strlen(msgintrare)) <= 0)
    {
      perror ("[server]Eroare la write() catre client din adaugareInTop.\n");
                 /* continuam sa ascultam */
    }
  while(1)
 {
  bzero(msg,100);
  if (read (client, msg, 100) <= 0)
   {
       perror ("[server]Eroare la read() de la client.\n");
       close (client);   /* inchidem conexiunea cu clientul */
       continue;         /* continuam sa ascultam */
   }
  msg[strlen(msg)-1]='\0';
  strcat(msg,".txt");
  if((fisier=open(msg,O_WRONLY|O_APPEND))==-1)
    {
       bzero(msgraspuns,1000);
       strcat(msgraspuns,"Topul nu exista\n");
       if(write(client,msgraspuns,strlen(msgraspuns))<=0)
         {
           perror("[server]Eroare la write() catre client din adaugareInTop.\n");
         }
    }
  else
     break;
 }//while

 char melodie[100],artist[100],link[100];
 bzero(msgraspuns,1000);
 strcat(msgraspuns,"Introdu numele artistului:\n");
 if(write(client,msgraspuns,strlen(msgraspuns))<=0)
   {
      perror("[server]Eroare la write() catre client din adaugareInTop.\n");
   }
 bzero(artist,100);
 if (read (client, artist, 100) <= 0)
   {
       perror ("[server]Eroare la read() de la client.\n");
       close (client);   /* inchidem conexiunea cu clientul */
//       continue;         /* continuam sa ascultam */
   }
 artist[strlen(artist)-1]='\0';
 fflush(stdout); 
 printf("%s\n",artist);

 bzero(msgraspuns,1000);
 strcat(msgraspuns,"Introdu numele melodiei:\n");
 if(write(client,msgraspuns,strlen(msgraspuns))<=0)
   {
      perror("[server]Eroare la write() catre client din adaugareInTop.\n");
   }
 bzero(melodie,100);
 if (read (client, melodie, 100) <= 0)
   {
       perror ("[server]Eroare la read() de la client.\n");
       close (client);   /* inchidem conexiunea cu clientul */
  //     continue;         /* continuam sa ascultam */
   }
  melodie[strlen(melodie)-1]='\0';
 fflush(stdout);
 printf("%s\n",melodie);

 bzero(msgraspuns,1000);
 strcat(msgraspuns,"Introdu link-ul melodiei:\n");
 if(write(client,msgraspuns,strlen(msgraspuns))<=0)
   {
      perror("[server]Eroare la write() catre client din adaugareInTop.\n");
   }
 bzero(link,100);
 if (read (client, link, 100) <= 0)
   {
       perror ("[server]Eroare la read() de la client.\n");
       close (client);   /* inchidem conexiunea cu clientul */
    //   continue;         /* continuam sa ascultam */
   }
 link[strlen(link)-1]='\0';
 fflush(stdout);
 printf("%s\n",link);

 char nume[200];
 bzero(nume,200);
 strcat(nume,"0~");
 strcat(nume,artist);
 strcat(nume,"~");
 strcat(nume,melodie);
 strcat(nume,"~");
 strcat(nume,link);
 write(fisier,nume,strlen(nume));
 close(fisier);
 fisier=open("general.txt",O_WRONLY|O_APPEND);
 write(fisier,nume,strlen(nume));
 close(fisier);

 sortareFisier(msg);
 sortareFisier("general.txt");

 bzero(msgraspuns,1000);
 strcat(msgraspuns,"Introdu un comentariu pentru melodie:\n");
 if(write(client,msgraspuns,strlen(msgraspuns))<=0)
   {
      perror("[server]Eroare la write() catre client din adaugareInTop.\n");
   }

 char descriere[100];
 bzero(descriere,100);
 if (read (client, descriere, 100) <= 0)
   {
       perror ("[server]Eroare la read() de la client.\n");
       close (client);   /* inchidem conexiunea cu clientul */
    //   continue;         /* continuam sa ascultam */
   }
 link[strlen(descriere)-1]='\0';
 fflush(stdout);

 char titluFisier[100];
 bzero(titluFisier,100);
 strcat(titluFisier,artist);
 strcat(titluFisier,"~");
 strcat(titluFisier,melodie);
 strcat(titluFisier,".txt");
 int i;
 for(i=0;i<strlen(titluFisier);i++)
    {
      if(titluFisier[i]==' ')
         titluFisier[i]='_';
    }
 printf("%s\n",titluFisier);
 fflush(stdout);
 int file;
 if((file=open(titluFisier,O_WRONLY|O_TRUNC|O_CREAT,00777))==-1)
     perror("Eroare la creare\n");
 if(write(file,descriere,strlen(descriere))<0)
	perror("Eroare la write\n");
 close(file);

}//sfarsit functie

void afiseazaTop(int client)
{
  char msgintrare[1000],msg[100],msgraspuns[100];
  FILE * fisier;
  bzero(msgintrare,1000);
  strcat(msgintrare,"Alege topul:\ndance\nrock\nhip-hop\nclasica\ngeneral\n");

  if (write (client, msgintrare, strlen(msgintrare)) <= 0)
    {
      perror ("[server]Eroare la write() catre client din adaugareInTop.\n");
    }

  while(1)
 {
  bzero(msg,100);
  if (read (client, msg, 100) <= 0)
   {
       perror ("[server]Eroare la read() de la client.\n");
       close (client);   /* inchidem conexiunea cu clientul */
       continue;         /* continuam sa ascultam */
   }
  msg[strlen(msg)-1]='\0';
  strcat(msg,".txt");
  printf("%s\n",msg);
  if((fisier=fopen(msg,"r"))==NULL)
    {
       bzero(msgraspuns,100);
       strcat(msgraspuns,"Topul nu exista\n");
       if(write(client,msgraspuns,strlen(msgraspuns))<=0)
         {
           perror("[server]Eroare la write() catre client din afiseazaTop.\n");
         }
    }
  else
    break;
 }//sfarsit while
 sortareFisier(msg);
 char linie[100];
 bzero(linie,100);
 while(fgets(linie,sizeof linie,fisier)!=NULL)
      {

         if(write(client,linie,strlen(linie))<=0)
         {
           perror("[server]Eroare la write() catre client din afiseazaTop.\n");
         }
        bzero(linie,100);
      }

 fclose(fisier);
}//sfarsit functie afiseaza

void adaugaComentariu(int client)
{
 afiseazaTop(client);
 char msg[100],artist[100],melodie[100];
 int file;

 while(1)
 {
   bzero(msg,100);
   strcat(msg,"Introdu numele artistului:\n");
   if(write(client,msg,strlen(msg))<=0)
     {
      perror("[server]Eroare la write() catre client din addComment.\n");
     }
   bzero(artist,100);
   if (read (client, artist, 100) <= 0)
     {
       perror ("[server]Eroare la read() de la client.\n");
       close (client);
     }
   artist[strlen(artist)-1]='\0';

   bzero(msg,100);
   strcat(msg,"Introdu numele melodiei:\n");
   if(write(client,msg,strlen(msg))<=0)
     {
      perror("[server]Eroare la write() catre client din addComment.\n");
     }
   bzero(melodie,100);
   fflush(stdout);
   if (read (client, melodie, 100) <= 0)
     {
       perror ("[server]Eroare la read() de la client.\n");
       close (client);
     }
   melodie[strlen(melodie)-1]='\0';

   char titluFisier[100];
   bzero(titluFisier,100);
   strcat(titluFisier,artist);
   strcat(titluFisier,"~");
   strcat(titluFisier,melodie);
   strcat(titluFisier,".txt");
   int i;
   for(i=0;i<strlen(titluFisier);i++)
     {
       if(titluFisier[i]==' ')
         titluFisier[i]='_';
     }
   printf("%s\n",titluFisier);
   fflush(stdout);
   if((file=open(titluFisier,O_WRONLY|O_APPEND))==-1)
      {
        perror("Eroare la deschidere\n");
        bzero(msg,100);
        strcat(msg,"Melodia nu exista in top! Mai incearca!\n");
        if(write(client,msg,strlen(msg))<=0)
           {
             perror("[server]Eroare la write() catre client din addComment.\n");
           }
      }
   else
      break;
 }//while

 bzero(msg,100);
 strcat(msg,"Introdu un comentariu:\n");
 if(write(client,msg,strlen(msg))<=0)
   {
     perror("[server]Eroare la write() catre client din addComment.\n");
   }

 char comentariu[100];
 bzero(comentariu,100);
 if (read (client, comentariu, 100) <= 0)
     {
       perror ("[server]Eroare la read() de la client.\n");
       close (client);
     }
  write(file,comentariu,strlen(comentariu));
  close(file);

  bzero(msg,100);
  strcat(msg,"Comentariu adaugat cu succes!\n");
  if(write(client,msg,strlen(msg))<=0)
   {
     perror("[server]Eroare la write() catre client din addComment.\n");
   }

}

void afiseazaComentarii(int client)
{
 afiseazaTop(client);
 char msg[100],artist[100],melodie[100];
 FILE * file;

 while(1)
 {
   bzero(msg,100);
   strcat(msg,"Introdu numele artistului:\n");
   if(write(client,msg,strlen(msg))<=0)
     {
      perror("[server]Eroare la write() catre client din addComment.\n");
     }
   bzero(artist,100);
   if (read (client, artist, 100) <= 0)
     {
       perror ("[server]Eroare la read() de la client.\n");
       close (client);
     }
   artist[strlen(artist)-1]='\0';

   bzero(msg,100);
   strcat(msg,"Introdu numele melodiei:\n");
   if(write(client,msg,strlen(msg))<=0)
     {
      perror("[server]Eroare la write() catre client din addComment.\n");
     }
   bzero(melodie,100);
   fflush(stdout);
   if (read (client, melodie, 100) <= 0)
     {
       perror ("[server]Eroare la read() de la client.\n");
       close (client);
     }
   melodie[strlen(melodie)-1]='\0';

   char titluFisier[100];
   bzero(titluFisier,100);
   strcat(titluFisier,artist);
   strcat(titluFisier,"~");
    strcat(titluFisier,melodie);
   strcat(titluFisier,".txt");
   int i;
   for(i=0;i<strlen(titluFisier);i++)
     {
       if(titluFisier[i]==' ')
         titluFisier[i]='_';
     }
   printf("%s\n",titluFisier);
   fflush(stdout);
   if((file=fopen(titluFisier,"r"))==-1)
      {
        perror("Eroare la deschidere\n");
        bzero(msg,100);
        strcat(msg,"Melodia nu exista in top! Mai incearca!\n");
        if(write(client,msg,strlen(msg))<=0)
           {
             perror("[server]Eroare la write() catre client din addComment.\n");
           }
      }
   else
      break;
 }//while

 char linie[100];
 while(fgets(linie,sizeof linie,file)!=NULL)
      {
         if(write(client,linie,strlen(linie))<=0)
         {
           perror("[server]Eroare la write() catre client din afiseazaTop.\n");
         }
      }

 fclose(file);

}

void voteaza(int client)
{
  char msgintrare[1000],msg[100],msgraspuns[100];
  FILE * file;
  bzero(msgintrare,1000);
  strcat(msgintrare,"Alege topul:\ndance\nrock\nhip-hop\nclasica\n");

  if (write (client, msgintrare, strlen(msgintrare)) <= 0)
    {
      perror ("[server]Eroare la write() catre client din adaugareInTop.\n");
    }

  while(1)
 {
  bzero(msg,100);
  if (read (client, msg, 100) <= 0)
   {
       perror ("[server]Eroare la read() de la client.\n");
       close (client);   /* inchidem conexiunea cu clientul */
       continue;         /* continuam sa ascultam */
   }
  msg[strlen(msg)-1]='\0';
  strcat(msg,".txt");
  printf("%s\n",msg);
  if((file=fopen(msg,"r"))==NULL)
    {
       bzero(msgraspuns,100);
       strcat(msgraspuns,"Topul nu exista\n");
       if(write(client,msgraspuns,strlen(msgraspuns))<=0)
         {
           perror("[server]Eroare la write() catre client din afiseazaTop.\n");
         }
    }
  else
    break;
 }//sfarsit while

 char mat[100][100],artist[100],melodie[100],aux[5];
 int i=0,nrLinii,gasit,j;

  while(fgets(mat[i],sizeof(mat[i]),file)!=NULL)
  {
     mat[i][strlen(mat[i])-1]='\0';
     strcat(mat[i],"\n");
     if(write(client,mat[i],strlen(mat[i]))<=0)
         {
           perror("[server]Eroare la write() catre client din voteaza.\n");
         }

     i++;
  }

 nrLinii=i;
 file=fopen(msg,"w");

 while(1)
 {
   bzero(msgraspuns,100);
   strcat(msgraspuns,"Introdu numele artistului:\n");
   if(write(client,msgraspuns,strlen(msgraspuns))<=0)
     {
      perror("[server]Eroare la write() catre client din voteaza.\n");
     }
   bzero(artist,100);
   if (read (client, artist, 100) <= 0)
     {
       perror ("[server]Eroare la read() de la client.\n");
       close (client);
     }
   artist[strlen(artist)-1]='\0';

   bzero(msgraspuns,100);
   strcat(msgraspuns,"Introdu numele melodiei:\n");
   if(write(client,msgraspuns,strlen(msgraspuns))<=0)
     {
      perror("[server]Eroare la write() catre client din voteaza.\n");
     }

   bzero(melodie,100);
   if (read (client, melodie, 100) <= 0)
     {
       perror ("[server]Eroare la read() de la client.\n");
       close (client);
     }
   melodie[strlen(melodie)-1]='\0';

   gasit=0;
   for(i=0;i<nrLinii;i++)
       if(strstr(mat[i],artist)!=NULL && strstr(mat[i],melodie)!=NULL)
          {
            gasit=1;
            j=0;
            while(mat[i][j]!='~')
            {
               aux[j]=mat[i][j];
               j++;
            }
           aux[j]='\0';
           sprintf(aux,"%d",atoi(aux)+1);
           for(j=0;j<strlen(aux);j++)
               mat[i][j]=aux[j];

          for(i=0;i<nrLinii;i++)
           {
             printf("%s\n",mat[i]);
             fflush(stdout);
             fputs(mat[i],file);
            }


          }
  if(gasit==0)
    {
       bzero(msgraspuns,100);
       strcat(msgraspuns,"Melodia nu exista in top! Mai incearca!\n");
       if(write(client,msgraspuns,strlen(msgraspuns))<=0)
           {
             perror("[server]Eroare la write() catre client din voteaza.\n");
           }
    }
  else
     break;
 }//sfarsit while

 fclose(file);

 FILE *f;
 f=fopen("general.txt","r");
 char m[100][100];
 i=0;
 while(fgets(m[i],sizeof(m[i]),f)!=NULL)
  {
     m[i][strlen(m[i])-1]='\0';
     strcat(m[i],"\n");
     i++;
  }
 nrLinii=i;

 f=fopen("general.txt","w");
 for(i=0;i<nrLinii;i++)
       if(strstr(m[i],artist)!=NULL && strstr(m[i],melodie)!=NULL)
          {
            j=0;
            while(m[i][j]!='~')
            {
               aux[j]=m[i][j];
               j++;
            }
           aux[j]='\0';
           sprintf(aux,"%d",atoi(aux)+1);
           for(j=0;j<strlen(aux);j++)
               m[i][j]=aux[j];

          for(i=0;i<nrLinii;i++)
            {
               printf("%s\n",mat[i]);
             fflush(stdout);

               fputs(m[i],f);
            }
          }
 fclose(f);

 bzero(msgraspuns,100);
 strcat(msgraspuns,"Ai votat!\n");
 if(write(client,msgraspuns,strlen(msgraspuns))<=0)
        {
             perror("[server]Eroare la write() catre client din voteaza.\n");
        }


}

void sterge(int client)
{
  char msgintrare[1000],msg[100],msgraspuns[100];
  FILE * file;
  bzero(msgintrare,1000);
  strcat(msgintrare,"Alege topul:\ndance\nrock\nhip-hop\nclasica\n");

  if (write (client, msgintrare, strlen(msgintrare)) <= 0)
    {
      perror ("[server]Eroare la write() catre client din sterge.\n");
    }

  while(1)
 {
  bzero(msg,100);
  if (read (client, msg, 100) <= 0)
   {
       perror ("[server]Eroare la read() de la client.\n");
       close (client);   /* inchidem conexiunea cu clientul */
       continue;         /* continuam sa ascultam */
   }
  msg[strlen(msg)-1]='\0';
  strcat(msg,".txt");
  printf("%s\n",msg);
  if((file=fopen(msg,"r"))==NULL)
    {
       bzero(msgraspuns,100);
       strcat(msgraspuns,"Topul nu exista\n");
       if(write(client,msgraspuns,strlen(msgraspuns))<=0)
         {
           perror("[server]Eroare la write() catre client din sterge.\n");
         }
    }
  else
    break;
 }//sfarsit while

 char mat[100][100],artist[100],melodie[100],aux[5];
 int i=0,nrLinii,gasit,j;

  while(fgets(mat[i],sizeof(mat[i]),file)!=NULL)
  {
     mat[i][strlen(mat[i])-1]='\0';
     strcat(mat[i],"\n");
     if(write(client,mat[i],strlen(mat[i]))<=0)
         {
           perror("[server]Eroare la write() catre client din sterge.\n");
         }

     i++;
  }

 nrLinii=i;
 file=fopen(msg,"w");

 while(1)
 {
   bzero(msgraspuns,100);
   strcat(msgraspuns,"Introdu numele artistului:\n");
   if(write(client,msgraspuns,strlen(msgraspuns))<=0)
     {
      perror("[server]Eroare la write() catre client din voteaza.\n");
     }
   bzero(artist,100);
   if (read (client, artist, 100) <= 0)
     {
       perror ("[server]Eroare la read() de la client.\n");
       close (client);
     }
   artist[strlen(artist)-1]='\0';

   bzero(msgraspuns,100);
   strcat(msgraspuns,"Introdu numele melodiei:\n");
   if(write(client,msgraspuns,strlen(msgraspuns))<=0)
     {
      perror("[server]Eroare la write() catre client din sterge.\n");
     }

   bzero(melodie,100);
   if (read (client, melodie, 100) <= 0)
     {
       perror ("[server]Eroare la read() de la client.\n");
       close (client);
     }
   melodie[strlen(melodie)-1]='\0';

   gasit=0;

   for(i=0;i<nrLinii;i++)
       if(strstr(mat[i],artist)!=NULL && strstr(mat[i],melodie)!=NULL)
          {
            gasit=1;
            for(j=i;j<nrLinii-1;j++)
               strcpy(mat[j],mat[j+1]);

          for(i=0;i<nrLinii-1;i++)
            {
             printf("%s\n",mat[i]);
             fflush(stdout);
             fputs(mat[i],file);

             }

          }
  if(gasit==0)
    {
       bzero(msgraspuns,100);
       strcat(msgraspuns,"Melodia nu exista in top! Mai incearca!\n");
       if(write(client,msgraspuns,strlen(msgraspuns))<=0)
           {
             perror("[server]Eroare la write() catre client din sterge.\n");
           }
    }
  else
     break;
}
 bzero(msgraspuns,100);
 strcat(msgraspuns,"Ai sters melodia!\n");
 if(write(client,msgraspuns,strlen(msgraspuns))<=0)
        {
             perror("[server]Eroare la write() catre client din sterge.\n");
        }


 fclose(file);

 file=fopen("general.txt","r");
 char m[100][100];
 i=0;
  while(fgets(m[i],sizeof(m[i]),file)!=NULL)
  {
     m[i][strlen(m[i])-1]='\0';
     strcat(m[i],"\n");
     i++;
  }
 nrLinii=i;
 file=fopen("general.txt","w");

  for(i=0;i<nrLinii;i++)
       if(strstr(m[i],artist)!=NULL && strstr(m[i],melodie)!=NULL)
          {
            for(j=i;j<nrLinii-1;j++)
               strcpy(m[j],m[j+1]);

          for(i=0;i<nrLinii-1;i++)
             fputs(m[i],file);

          }
 fclose(file);
}

int resUser(int client)
{
 FILE * file;
 file=fopen("users.txt","r");
 int i,nrLinii;
 char usersFisier[100][100],msg[100];
 bzero(msg,100);
 strcat(msg,"Alege un user:\n");
 if(write(client,msg,strlen(msg))<=0)
    {
       perror("[server]Eroare la write() catre client din resUser.\n");
    }


  i=0;
  while(fgets(usersFisier[i],sizeof(usersFisier[i]),file)!=NULL)
          {
            usersFisier[i][strlen(usersFisier[i])-1]='\0';
            strcat(usersFisier[i],"\n");
            if(write(client,usersFisier[i],strlen(usersFisier[i]))<=0)
               perror("[server]Eroare la write() catre client din sterge.\n");

            i++;
          }
  nrLinii=i;
  fclose(file);

  char utiliz[100],users[100][100];
  bzero(utiliz,100);
  if (read (client, utiliz, 100) <= 0)
     {
       perror ("[server]Eroare la read() de la client.\n");
       close (client);
     }

  file=fopen("usersRes.txt","r+");
  i=0;
  while(fgets(users[i],sizeof(users[i]),file)!=NULL)
          {
            users[i][strlen(users[i])-1]='\0';
            i++;
          }
  nrLinii=i;

  int gasit=0;
  for(i=0;i<nrLinii;i++)
     if(strcmp(users[i],utiliz)!=NULL)
       gasit=1;
 if(gasit==0)
    fputs(utiliz,file);
 fclose(file);

 bzero(msg,100);
 strcat(msg,"Ai restrictionat la vot userul: ");
 strcat(msg,utiliz);
 if(write(client,msg,strlen(msg))<=0)
    {
       perror("[server]Eroare la write() catre client din resUser.\n");
    }



}

int main ()
{
 struct sockaddr_in server;	// structura folosita de server
 struct sockaddr_in from;
 char msg[100];		//mesajul primit de la client
 char msgrasp[100]=" ";        //mesaj de raspuns pentru client
 int sd;			//descriptorul de socket
 int optval=1;

 /* crearea unui socket */
 if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
   {
      perror ("[server]Eroare la socket().\n");
      return errno;
   }
 setsockopt(sd, SOL_SOCKET, SO_REUSEADDR,&optval,sizeof(optval));

 /* pregatirea structurilor de date */
 bzero (&server, sizeof (server));
 bzero (&from, sizeof (from));

 /* umplem structura folosita de server */
 /* stabilirea familiei de socket-uri */
 server.sin_family = AF_INET;
 server.sin_addr.s_addr = htonl (INADDR_ANY);
 server.sin_port = htons (PORT);

 /* atasam socketul */
 if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
       perror ("[server]Eroare la bind().\n");
       return errno;
    }

 /* punem serverul sa asculte daca vin clienti sa se conecteze */
 if (listen (sd, 1) == -1)
    {
       perror ("[server]Eroare la listen().\n");
       return errno;
    }

 /* servim in mod concurent clientii... */
 while (1)
    {
      int client;
      int length = sizeof (from);
      char numeFisier[100],msgEr[100];
      printf ("[server]Asteptam la portul %d...\n",PORT);
      fflush (stdout);

      /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
      client = accept (sd, (struct sockaddr *) &from, &length);

      /* eroare la acceptarea conexiunii de la un client */
      if (client < 0)
       {
         perror ("[server]Eroare la accept().\n");
         continue;
       }

       int pid;
       if ((pid = fork()) == -1)
          {
            close(client);
            continue;
          }
       else
          if (pid > 0)
           {
             // parinte
             close(client);
             while(waitpid(-1,NULL,WNOHANG));
             continue;
           }
          else
            if (pid == 0)
               {
   	         // copil
                 int admin;
	         close(sd);
	         admin=login(client); //prima data clientul trebuie sa se logheze
char comenzi[1000];
                     bzero(comenzi,1000);
                     if(admin==1)
                       {
                         strcat(comenzi,"Comezile pe care le poti executa sunt:\n add in top \n add comment \n afiseaza top\n afiseza comentarii\n voteaza\n sterge\n restrictioneaza vot\n quit \n");
                       }
                     else
                         strcat(comenzi,"Comezile pe care le poti executa sunt:\n add in top \n add comment \n afiseaza top\n afiseaza comentarii\n voteaza\n quit \n");

         	 while(1)
	          {

	             bzero (msg, 100);
	             printf ("[server]Asteptam mesajul...\n");
	             fflush (stdout);

	             /* citirea mesajului */
	             if (read (client, msg, 100) <= 0)
	                {
	                  perror ("[server]Eroare la read() de la client.\n");
	                  close (client);	/* inchidem conexiunea cu clientul */
	                  continue;		/* continuam sa ascultam */
	                }
                     msg[strlen(msg)-1]='\0';
                     if(strstr(msg,"add in top")!=NULL)
                        adaugareInTop(client);
                     else
                        if(strstr(msg,"add comment")!=NULL)
                          adaugaComentariu(client);
                        else
                            if(strstr(msg,"afiseaza top")!=NULL)
                               afiseazaTop(client);
                            else
                               if(strstr(msg,"afiseaza comentarii")!=NULL)
                                  afiseazaComentarii(client);
                               else
                                   if(strstr(msg,"voteaza")!=NULL)
                                      {
                                       if(admin==2)
                                         {
                                             bzero(msgEr,100);
                                             strcat(msgEr,"Aveti accesul la vot restrictionat!\n");
                                             if (write (client, msgEr, strlen(msgEr)) <= 0)
                                               {
                                                 perror ("[server]Eroare la write() catre client.\n");
                                                 continue;
                                               }
                                         }
                                      else
                                        voteaza(client);
                                      }
                                   else
                                     if(strstr(msg,"sterge")!=NULL && admin==1)
                                         sterge(client);
                                     else
                                        if(strstr(msg,"restrictioneaza vot")!=NULL && admin==1)
                                          resUser(client);
                            else
		               if(strstr(msg,"quit")!=NULL)
			          exit(1);
                               else
                                  {
                                    bzero(msgEr,100);
                                    strcat(msgEr,"Ati introdus o comanda gresita! Mai incearca!\n");
                                    if (write (client, msgEr, strlen(msgEr)) <= 0)
                                       {
                                             perror ("[server]Eroare la write() catre client.\n");
                                             continue;           /* continuam sa ascultam */
                                       }
                                    else
                                         printf ("[server]Mesajul a fost trasmis cu succes.\n");

                                  }
	if (write (client, comenzi, strlen(comenzi)) <= 0)
                        {
                          perror ("[server]Eroare la write() catre client.\n");
                          continue;           /* continuam sa ascultam */
                        }
                     else
                        printf ("[server]Mesajul a fost trasmis cu succes.\n");
	    }

	}
 
    }				/* while */
}				/* main */
