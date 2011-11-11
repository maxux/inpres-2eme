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
#include <qapplication.h>

#include <stdio.h>

#include <sys/ipc.h>
#include <sys/msg.h>

#include "FenetrePlanVille.hxx"

char *NomS;

#define CHEMIN

#include "Commun.dat"
#include "Donnee.dat"

message_t MessageEnvoie;

void TracePlan(QFrame* F);
void TraceParcours(QFrame *, int);
void TraceChemin(QFrame *, int, int[]);

/*
 *  Constructs a FenetrePlanVille as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
FenetrePlanVille::FenetrePlanVille(QWidget* parent, const char *name, bool modal, Qt::WFlags fl) : QDialog(parent, name, modal, fl) {
	if(!name)
		setName("FenetrePlanVille");

	LabelArrivee = new QLabel(this, "LabelArrivee");
	LabelArrivee->setEnabled(TRUE);
	LabelArrivee->setGeometry(QRect(284, 20, 180, 20));

	LabelInformation = new QLabel( this, "LabelInformation");
	LabelInformation->setGeometry(QRect(40, 360, 100, 20));

	LabelMinutes = new QLabel(this, "LabelMinutes");
	LabelMinutes->setGeometry(QRect(540, 20, 58, 20));

	LabelDestination = new QLabel(this, "LabelDestination" );
	LabelDestination->setGeometry(QRect(600, 60, 90, 20));

	lineMinutes = new QLineEdit(this, "lineMinutes");

	lineMinutes->setGeometry(QRect(470, 20, 60, 21));
	lineMinutes->setReadOnly(TRUE);

	lineDestination = new QLineEdit(this, "lineDestination");
	lineDestination->setGeometry(QRect(600, 90, 80, 21));

	textInformation = new QTextEdit(this, "textInformation");
	textInformation->setGeometry(QRect(40, 390, 550, 62));
	textInformation->setReadOnly(TRUE);

	framePlan = new QFrame(this, "framePlan");
	framePlan->setGeometry(QRect(40, 50, 550, 300));
	framePlan->setFrameShape(QFrame::StyledPanel);
	framePlan->setFrameShadow(QFrame::Raised);

	ButtonSelection = new QPushButton(this, "ButtonSelection");
	ButtonSelection->setGeometry(QRect(600, 120, 84, 29));
	ButtonSelection->setAutoDefault(FALSE);

	ButtonTerminer = new QPushButton(this, "ButtonTerminer");
	ButtonTerminer->setGeometry(QRect(600, 310, 84, 29));
	ButtonTerminer->setAutoDefault(FALSE);

	LabelNomVille = new QLabel(this, "LabelNomVille");
	LabelNomVille->setGeometry(QRect(40, 20, 53, 20 ) );

	NomVille = new QLabel(this, "NomVille");
	NomVille->setGeometry(QRect(110, 20, 58, 20));
	languageChange();
	resize(QSize(711, 480).expandedTo(minimumSizeHint()));
	
	// Qt4 - Qt3 Support
	// clearWState(WState_Polished);

	// signals and slots connections
	connect(ButtonSelection, SIGNAL(pressed()), this, SLOT(Selection()));
	connect(ButtonTerminer, SIGNAL(pressed()), this, SLOT(Terminer()));
	connect(lineDestination, SIGNAL(returnPressed()), this, SLOT(Selection()));
}

/*
 *  Destroys the object and frees any allocated resources
 */
FenetrePlanVille::~FenetrePlanVille() {
	// no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void FenetrePlanVille::languageChange() {
	setCaption(tr("Plan de la ville."));
	
	LabelArrivee->setText(tr("Arrivee d'une rame dans"));
	LabelInformation->setText(tr("<b>Informations</b>"));
	LabelMinutes->setText(tr("minutes"));
	LabelDestination->setText(tr("Destination:"));
	ButtonSelection->setText(tr("Selection"));
	ButtonTerminer->setText(tr("Terminer"));
	LabelNomVille->setText(tr("Station"));
	NomVille->setText(tr(NomS));

	lineDestination->setFocus();
}

void FenetrePlanVille::Terminer() {
	// Trace("Entree dans FenetrePlanVille::Terminer()" );
	exit(0);
}

void FenetrePlanVille::Selection() {
	// Trace("Entree dans FenetrePlanVille::Selection()");
	int Arrivee, rc, a;
	char Buff[5];

	strcpy(Buff, lineDestination->text());
	Arrivee = atoi(Buff);
	// en fait, rechercher le numero de la station.....

	// TracePlan(framePlan);
	MessageEnvoie.lType = 1L;
	MessageEnvoie.request = PROTO_SEARCH;
	MessageEnvoie.pid = getpid();
	/* MessageEnvoie.message[0] = 1;
	MessageEnvoie.message[1] = Arrivee; */
	strcpy(MessageEnvoie.text, "Testing Message");
	
	int key_id = msgget(1342, IPC_CREAT | 0666);

	if(msgsnd(key_id, &MessageEnvoie, sizeof(MessageEnvoie) - sizeof(long), 0)) {
		fprintf(stderr, "Err. de msgsnd: %d %s", __LINE__, __FILE__);
		exit(1);
	}
	
	if((rc = msgrcv(key_id, &MessageEnvoie, 200, getpid(), 0)) == -1) {
		fprintf(stderr, "Err. de msgrcv: %d %s", __LINE__, __FILE__);
		exit(1);
	}
	
	int TailleChemin = (rc - sizeof(pid_t) - sizeof(int)) / sizeof(int);
	// Trace("Taille du chemin : %d\n",TailleChemin);
	// TraceChemin(framePlan, TailleChemin, MessageEnvoie.text);
	a = 1;
	TraceChemin(framePlan, TailleChemin, &a);
	alarm(2);
	
	return;
}

void AffichePub(QTextEdit *T, const char* P) {
	T->setText(P);
}

void TracePlan(QFrame* F) {
	int i = 0;
	QPainter paint(F);
	
	while(i<6) {
		TraceParcours(F, i);
		i++;
	}
}

void TraceParcours(QFrame* F, int Nb) {
	QPainter paint(F);
	int i = 0;
	
	paint.setPen(LigneMetro[Nb].Couleur);
	
	while(LigneMetro[Nb].Position[i+1].N) {
		paint.drawLine(LigneMetro[Nb].Position[i].L, LigneMetro[Nb].Position[i].C, LigneMetro[Nb].Position[i+1].L, LigneMetro[Nb].Position[i+1].C);
		i++;
	}
	
	paint.setPen(Qt::black);
	
	i = 1;
	while(i <= 34) {
		paint.drawText(Donnee[i].L,Donnee[i].C,Donnee[i].Station);
		i++;
	}
}

void TraceChemin(QFrame* F,int Nb,int Chemin[]) {
	QPainter paint(F);
	int i = 0;
	
	paint.setPen(Qt::red);
	
	while(i < Nb) {
		paint.drawText(Donnee[Chemin[i]].L, Donnee[Chemin[i]].C, Donnee[Chemin[i]].Station);
		// Trace("%s",Donnee[Chemin[i]].Station);
		i++;
	}
}

int main(int argc, char *argv[]) {
	FenetrePlanVille *F1;
	message_t message;
	
	QApplication a(argc, argv);
	
	int key_id = msgget(1342, IPC_CREAT | 0666);
	
	message.lType = 1L;
	message.request = PROTO_LOGIN;
	message.pid = getpid();
	/* MessageEnvoie.message[0] = 1;
	MessageEnvoie.message[1] = Arrivee; */
	strcpy(message.text, "Hello World");
	
	
	if(msgsnd(key_id, &message, sizeof(message) - sizeof(long), 0)) {
		fprintf(stderr, "Err. de msgsnd: %d %s", __LINE__, __FILE__);
		exit(1);
	}
	

	F1 = new FenetrePlanVille();
	//a.setMainWidget(&F1 );
	F1->show();
	
	return a.exec();
}
