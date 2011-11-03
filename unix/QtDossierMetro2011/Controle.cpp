/* Programme qui 
   - initialise les ressources necessaires
   - lance les programmes
   - gere les differentes Borne
   - ...
   In.Pr.E.S.
   Controle.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "fctTrace.h"

#define PROCESS	  "Controle"
#define IDQ       (key_t)1234

#include "Commun.dat"
#include "Donnee.dat"

MESSAGE		MessageLu;
//#include "fichier.ini"


void Handler(int);
void Sortie(int);

int            idQ;

int main()
{
struct sigaction Action;
int            idChemin,rc;
char           szQueue[10];
pid_t          idPid;

/* Armement des signaux */
Trace("(Controle) Armement des signaux...");

Action.sa_handler = Handler;
sigemptyset(&Action.sa_mask);
Action.sa_flags = 0;

if (sigaction(SIGINT,&Action,NULL) == -1)
   { TraceErr(__LINE__,__FILE__,"Err. de sigaction()");
     exit(1);
   }

Trace("(Controle) Creation des ressources necessaires...");

if ((idQ = msgget(IDQ,IPC_CREAT | IPC_EXCL | 0600)) == -1)
   { TraceErr(__LINE__,__FILE__,"Err. de creation de la 1ere queue...\n");
     exit(1);
   }
// initialiser la station .
// ....

while (1)
   {
     idPid = 1;
     Trace("(Controle) attend");
     if ((rc = msgrcv(idQ,&MessageLu,sizeof(MESSAGE) - sizeof(long), 1L,0)) < 0)
        { TraceErr(__LINE__,__FILE__,"Err. de reception d'un message...");
          Sortie(1);
        }
     switch (MessageLu.Requete)
        { case IDENTIFICATION:
             Trace("(Controle) Demande d'identification");
             break;
          case RECHERCHE:
             Trace("(Controle) Demande de chemin vers %d",MessageLu.Message[1]);
             if ((idChemin = fork()) == -1) 
                { TraceErr(__LINE__,__FILE__,"Err. de fork()");
                  Sortie(1);
                }
             if (!idChemin)
                { /* processus fils   */
                  sprintf(szQueue,"%d",idQ);
                  execl("./Chemin","Chemin",szQueue,NULL);
                  TraceErr(__LINE__,__FILE__,"Err. de execl du process Chemin");
                }

             MessageLu.lType = idChemin;
             if (msgsnd(idQ,&MessageLu,sizeof(MessageLu) - sizeof(long),0) <0)
                { TraceErr(__LINE__,__FILE__,"Err. de msgsnd()");
                  Sortie(1);
                }
             break;
          default:
            break;
        }

         
   }
}

void Sortie(int rcSortie)
{
if (msgctl(idQ,IPC_RMID,0))
   TraceErr(__LINE__,__FILE__,"Err. de Suppression de IdRq");
exit(rcSortie);
}

void Handler(int Sig)
{
Sortie(Sig);
}

