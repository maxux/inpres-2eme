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
#include <signal.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>

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
	
	/* Disable custom draw */
	draw.need_repaint = false;
	draw.todo 	  = 0;

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
	connect(lineDestination, SIGNAL(returnPressed()), this, SLOT(Selection()));
	
	connect(ButtonTerminer, SIGNAL(pressed()), this, SLOT(Terminer()));
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

	/* strcpy(Buff, lineDestination->text());
	Arrivee = atoi(Buff);
	// en fait, rechercher le numero de la station.....

	// TracePlan(framePlan);
	MessageEnvoie.lType = 1L;
	MessageEnvoie.request = PROTO_SEARCH;
	MessageEnvoie.pid = getpid();
	// MessageEnvoie.message[0] = 1;
	// MessageEnvoie.message[1] = Arrivee;
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
	*/
	
	int TailleChemin = (rc - sizeof(pid_t) - sizeof(int)) / sizeof(int);
	
	a = 1;
	draw.need_repaint = true;
	draw.todo = DRAW_PATH;
	// TraceChemin(framePlan, TailleChemin, &a);
	repaint();
	
	alarm(2);
	
	return;
}

void FenetrePlanVille::AffichePub(QTextEdit *T, const char* P) {
	T->setText(P);
}

void FenetrePlanVille::paintEvent(QPaintEvent *event) {
	QPainter paint(this);
	int i, tmp[3] = {1, 2, 3};
	
	printf("Drawing...\n");
	if(draw.need_repaint) {
		printf("NEED REPAINT\n");
		
		
		switch(draw.todo) {
			case DRAW_PLAN:
				for(i = 0; i < 6; i++)
					TraceParcours(i, paint);
			break;
			
			case DRAW_PATH:
				TraceChemin(4, tmp, paint);
			break;
		}
		
		draw.need_repaint = false;
	}
	
	// paint.setWindow(framePlan->frameRect());
	paint.drawText(10, 10, "Hello World");
}

void FenetrePlanVille::TracePlan(QFrame* F) {
	// int i = 0;
	// QPainter paint(F);
	// repaint();
	
	draw.need_repaint = true;
	draw.todo = DRAW_PLAN;
	repaint();
		
		
		// TraceParcours(F, i);
	//	i++;
	// }
}

void FenetrePlanVille::TraceParcours(int Nb, QPainter &paint) {
	// QPainter paint(F);
	int i = 0;
	
	printf("UPDATING FROM %d\n", Nb);
	
	paint.setPen(LigneMetro[Nb].Couleur);
	
	while(LigneMetro[Nb].Position[i + 1].N) {
		paint.drawLine(LigneMetro[Nb].Position[i].L, LigneMetro[Nb].Position[i].C, LigneMetro[Nb].Position[i+1].L, LigneMetro[Nb].Position[i+1].C);
		i++;
	}
	
	paint.setPen(Qt::black);
	
	i = 1;
	while(i <= 34) {
		paint.drawText(Donnee[i].L, Donnee[i].C, Donnee[i].Station);
		i++;
	}
}

void FenetrePlanVille::TraceChemin(int Nb, int Chemin[], QPainter &paint) {
	int i = 0;
	
	paint.setPen(Qt::red);
	
	while(i < Nb) {
		paint.drawText(Donnee[Chemin[i]].L, Donnee[Chemin[i]].C, Donnee[Chemin[i]].Station);
		// Trace("%s",Donnee[Chemin[i]].Station);
		i++;
	}
}

FenetrePlanVille *F1;

void sig_handler(int signum) {
	switch(signum) {
		/* SIGUSR1: New ads */
		case SIGUSR1:
			printf("SIGUSR1 intercepted\n");
			F1->AffichePub(F1->textInformation, "New ads incomming...");
		break;
		
		case SIGUSR2:
			printf("SIGUSR2 intercepted\n");
		break;
	}
	
	// F1->TracePlan(F1->framePlan);
}

int main(int argc, char *argv[]) {
	message_t message;	/* Message Queue */
	key_t skey;		/* Shared Memory Segment ID */
	int skey_id;		/* Shared Memory key */
	char *shm;		/* Shared Memory Segment */
	
	QApplication a(argc, argv);
	
	printf("Loading...\n");
	
	/* Intercept ALARM */
	signal(SIGALRM, sig_handler);
	
	/* Intercept SIGUSR1: Advertissment */
	signal(SIGUSR1, sig_handler);
	
	/* Incercept SIGUSR2: Sys Admin Message */
	signal(SIGUSR2, sig_handler);
	
	int key_id = msgget(MESSAGE_KEY_ID, IPC_CREAT | 0666);
	
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
	
	/* Attaching Shared Memory Segment */
	skey = SHARED_MEMORY_ID;
	if((skey_id = shmget(skey, SHARED_MEMORY_SIZE, 0666)) < 0) {
		perror("shmget");
		// exit(1);
	}
	
	if((shm = (char *) shmat(skey_id, NULL, 0)) == (char *) -1) {
		perror("shmat");
		// exit(1);
	}
	
	printf("DEBUG: <%s>\n", shm);

	F1 = new FenetrePlanVille();
	// a.setMainWidget(&F1);
	F1->show();
	
	/* printf("wait...\n");
	alarm(2); */
	F1->TracePlan(F1->framePlan);
	
	printf("Hello World\n");
	
	return a.exec();
}
