#include <qapplication.h>
#include "FenetrePlanVille.h"
//#include "fctTrace.h"

#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
int idQ;

char*		NomS;

#include "Commun.dat"

void AffichePub(QTextEdit*,const char*);
void TracePlan(QFrame*);

#define IDQ       (key_t)1234

struct sigaction Action;

FenetrePlanVille *F1; 

void Handler(int);

int main( int argc, char **argv )
{
MESSAGE		MessageId;

printf("%d---\n",argc);
if (argc != 2)
   { printf("Trop ou trop peu d'argument(s)....\n");
     exit(1);
   }

NomS = (char*)malloc(20);
strcpy(NomS,argv[1]);

// Recureration des ressources

Action.sa_handler = Handler;
sigemptyset(&Action.sa_mask);
Action.sa_flags = 0;
if (sigaction(SIGALRM,&Action,NULL))
   { perror("Err. de siagaction()");
     exit(1);
   }

alarm(5);

if ((idQ = msgget(IDQ,0)) == -1)
   { // TraceErr(__LINE__,__FILE__,"Err. de msgget()");
     exit(1);
   }

MessageId.lType = 1L;
MessageId.idProcess = getpid();
MessageId.Requete = IDENTIFICATION;

if (msgsnd(idQ,&MessageId,sizeof(pid_t) + sizeof(int),0))
   { perror("Err. de msgsnd()");
     exit(1);
   }

// traiter la reponse ...

QApplication a( argc, argv );
F1 = new FenetrePlanVille();
    //a.setMainWidget(&F1 );
F1->show();
return a.exec();
}

void Handler(int Sig)
{
TracePlan(F1->framePlan);
char* T[] = { "Dossier Unix Janvier 2011",
               "ne perdez pas de temps",
               "bon travail",
               "dddddddddddddd\ndddddddd"
            };

static int 	i = 0;
printf("%s\n",T[i]);
AffichePub(F1->textInformation,T[i]);
i++;
if (i == 4) i = 0;
alarm(5);
return;
}
