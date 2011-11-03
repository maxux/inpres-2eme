/****************************************************************************
** Form implementation generated from reading ui file 'FenetrePlanVille.ui'
**
** Created: ven. oct. 14 12:32:16 2011
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "FenetrePlanVille.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qframe.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qpainter.h>

#include <sys/ipc.h>
#include <sys/msg.h>

extern char*		NomS;
extern int		idQ;

#define CHEMIN

#include "Commun.dat"
#include "Donnee.dat"
#include "fctTrace.h"

MESSAGE		MessageEnvoie;
MESSAGEMAX	MessageMax;

void TracePlan(QFrame* F);
void TraceParcours(QFrame *,int);
void TraceChemin(QFrame* ,int,int[]);

/*
 *  Constructs a FenetrePlanVille as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
FenetrePlanVille::FenetrePlanVille( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "FenetrePlanVille" );

    LabelArrivee = new QLabel( this, "LabelArrivee" );
    LabelArrivee->setEnabled( TRUE );
    LabelArrivee->setGeometry( QRect( 284, 20, 180, 20 ) );

    LabelInformation = new QLabel( this, "LabelInformation" );
    LabelInformation->setGeometry( QRect( 40, 360, 100, 20 ) );

    LabelMinutes = new QLabel( this, "LabelMinutes" );
    LabelMinutes->setGeometry( QRect( 540, 20, 58, 20 ) );

    LabelDestination = new QLabel( this, "LabelDestination" );
    LabelDestination->setGeometry( QRect( 600, 60, 90, 20 ) );

    lineMinutes = new QLineEdit( this, "lineMinutes" );

    lineMinutes->setGeometry( QRect( 470, 20, 60, 21 ) );
    lineMinutes->setReadOnly( TRUE );

    lineDestination = new QLineEdit( this, "lineDestination" );
    lineDestination->setGeometry( QRect( 600, 90, 80, 21 ) );

    textInformation = new QTextEdit( this, "textInformation" );
    textInformation->setGeometry( QRect( 40, 390, 550, 62 ) );
    textInformation->setReadOnly( TRUE );

    framePlan = new QFrame( this, "framePlan" );
    framePlan->setGeometry( QRect( 40, 50, 550, 300 ) );
    framePlan->setFrameShape( QFrame::StyledPanel );
    framePlan->setFrameShadow( QFrame::Raised );

    ButtonSelection = new QPushButton( this, "ButtonSelection" );
    ButtonSelection->setGeometry( QRect( 600, 120, 84, 29 ) );
    ButtonSelection->setAutoDefault( FALSE );

    ButtonTerminer = new QPushButton( this, "ButtonTerminer" );
    ButtonTerminer->setGeometry( QRect( 600, 310, 84, 29 ) );
    ButtonTerminer->setAutoDefault( FALSE );

    LabelNomVille = new QLabel( this, "LabelNomVille" );
    LabelNomVille->setGeometry( QRect( 40, 20, 53, 20 ) );

    NomVille = new QLabel( this, "NomVille" );
    NomVille->setGeometry( QRect( 110, 20, 58, 20 ) );
    languageChange();
    resize( QSize(711, 480).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( ButtonSelection, SIGNAL( pressed() ), this, SLOT( Selection() ) );
    connect( ButtonTerminer, SIGNAL( pressed() ), this, SLOT( Terminer() ) );
    connect( lineDestination, SIGNAL( returnPressed() ), this, SLOT( Selection() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
FenetrePlanVille::~FenetrePlanVille()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void FenetrePlanVille::languageChange()
{
    setCaption( tr( "Plan de la ville." ) );
    LabelArrivee->setText( tr( "Arrivee d'une rame dans" ) );
    LabelInformation->setText( tr( "<b>Informations</b>" ) );
    LabelMinutes->setText( tr( "minutes" ) );
    LabelDestination->setText( tr( "Destination:" ) );
    ButtonSelection->setText( tr( "Selection" ) );
    ButtonTerminer->setText( tr( "Terminer" ) );
    LabelNomVille->setText( tr( "Station" ) );
    NomVille->setText( tr( NomS ) );

lineDestination->setFocus();
}

void FenetrePlanVille::Terminer()
{
Trace("Entree dans FenetrePlanVille::Terminer()" );
exit(0);
}

void FenetrePlanVille::Selection()
{
Trace("Entree dans FenetrePlanVille::Selection()");
int		Arrivee,rc;
char		Buff[5];

strcpy(Buff,lineDestination->text());
Arrivee = atoi(Buff);
// en fait, rechercher le numero de la station.....

TracePlan(framePlan);
MessageEnvoie.lType = 1L;
MessageEnvoie.Requete = RECHERCHE;
MessageEnvoie.idProcess = getpid();
MessageEnvoie.Message[0] = 1;
MessageEnvoie.Message[1] = Arrivee;

if (msgsnd(idQ,&MessageEnvoie,sizeof(MessageEnvoie) - sizeof(long),0))
   { TraceErr(__LINE__,__FILE__,"Err. de msgsnd()");
     exit(1);
   }
if ((rc = msgrcv(idQ,&MessageMax,200,getpid(),0)) == -1)
   { TraceErr(__LINE__,__FILE__,"Err. de msgrcv()");
     exit(1);
   }
int TailleChemin = (rc - sizeof(pid_t) - sizeof(int))/sizeof(int);
Trace("Taille du chemin : %d\n",TailleChemin);
TraceChemin(framePlan,TailleChemin,MessageMax.Message);
alarm(2);
return;
}

void AffichePub(QTextEdit *T,const char* P)
{
T->setText(P);
}

void TracePlan(QFrame* F)
{
int     i = 0;
QPainter        paint(F);
while (i<6)
   { TraceParcours(F,i);
     i++;
   }
}

void TraceParcours(QFrame* F,int Nb)
{
QPainter        paint(F);
int     i = 0;
paint.setPen(LigneMetro[Nb].Couleur);
while (LigneMetro[Nb].Position[i+1].N)
   { paint.drawLine( LigneMetro[Nb].Position[i].L,
                     LigneMetro[Nb].Position[i].C,
                     LigneMetro[Nb].Position[i+1].L,
                     LigneMetro[Nb].Position[i+1].C);

      i++;
    }
paint.setPen(Qt::black);
i = 1;
while (i <= 34)
   { paint.drawText(Donnee[i].L,Donnee[i].C,Donnee[i].Station);
     i++;
   }
return;
}

void TraceChemin(QFrame* F,int Nb,int Chemin[])
{
QPainter        paint(F);
int     i = 0;
paint.setPen(Qt::red);
while (i < Nb) 
   { paint.drawText(Donnee[Chemin[i]].L,
                    Donnee[Chemin[i]].C,
                    Donnee[Chemin[i]].Station);
     Trace("%s",Donnee[Chemin[i]].Station);
     i++;
   }

return;
}
