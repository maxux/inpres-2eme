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

global_t sys;

/* Metro Data */
station_t stations[METRO_MAX_STATION];
ligne_t lignes[METRO_MAX_LIGNE];
ligne_legacy_t legacy_lignes[METRO_MAX_LIGNE];


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
	// NomVille->setText(tr(NomS));

	lineDestination->setFocus();
}

void FenetrePlanVille::Terminer() {
	send_message(PROTO_QRY_LOGOUT, (void*) "kthxbye");
	exit(0);
}

int send_message(int request, void *data) {
	message_t message;	/* Message Queue */
	
	message.lType   = 1L;
	message.request = request;
	message.pid     = getpid();
	strcpy(message.text, (char*) data);
	
	if(msgsnd(*(sys.mkey_id), &message, sizeof(message) - sizeof(long), 0)) {
		perror("msgsnd");
		return 0;
	}
	
	return 1;
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
	int i, tmp[] = {1, 3, 2, 4, 5};
	
	if(draw.need_repaint) {
		printf("NEED REPAINT\n");
		
		
		switch(draw.todo) {
			case DRAW_PLAN:
				for(i = 0; i < 6; i++)
					TraceParcours(i, paint);
			break;
			
			case DRAW_PATH:
				for(i = 0; i < 6; i++)
					TraceParcours(i, paint);
					
				TraceChemin(5, tmp, paint);
			break;
		}
		
		draw.need_repaint = false;
	}
}

void FenetrePlanVille::TracePlan(QFrame* F) {
	/* Notify Redraw Event to redraw the map */
	draw.need_repaint = true;
	draw.todo = DRAW_PLAN;
	repaint();
}

void FenetrePlanVille::TraceParcours(int Nb, QPainter &paint) {
	int i = 0;
	
	paint.setPen(lignes[Nb].couleur);
	
	while(lignes[Nb].position[i + 1].N) {
		paint.drawLine(lignes[Nb].position[i].L + 40, lignes[Nb].position[i].C + 50, lignes[Nb].position[i+1].L + 40, lignes[Nb].position[i+1].C + 50);
		i++;
	}
	
	paint.setPen(Qt::black);
	
	i = 1;
	while(i <= 34) {
		paint.drawText(stations[i].L + 40, stations[i].C + 50, stations[i].station);
		i++;
	}
}

void FenetrePlanVille::TraceChemin(int Nb, int Chemin[], QPainter &paint) {
	int i = 0;
	QPen pen;
	
	pen.setWidth(5);
	pen.setColor(Qt::red);
	
	paint.setPen(pen);
	
	while(i < Nb) {
		// Drawing lines
		paint.drawLine(lignes[Chemin[i]].position[i].L + FIX_FRAME_Y, lignes[Chemin[i]].position[i].C + FIX_FRAME_X, lignes[Chemin[i]].position[i + 1].L + FIX_FRAME_Y, lignes[Chemin[i]].position[i + 1].C + FIX_FRAME_X);
		
		// Drawing over text
		paint.drawText(stations[Chemin[i]].L + FIX_FRAME_Y, stations[Chemin[i]].C + FIX_FRAME_X, stations[Chemin[i]].station);
		
		// Trace("%s",Donnee[Chemin[i]].Station);
		i++;
	}
}

void sig_handler(int signum) {
	switch(signum) {
		/* SIGUSR1: New ads */
		case SIGUSR1:
			printf("SIGUSR1 intercepted\n");
			sys.window->AffichePub(sys.window->textInformation, sys.shm);
		break;
		
		case SIGUSR2:
			printf("SIGUSR2 intercepted\n");
		break;
		
		case SIGINT:
			sys.window->Terminer();
		break;
	}
	
	// F1->TracePlan(F1->framePlan);
}

int signal_intercept(int signal, void (*function)(int)) {
	struct sigaction sig;
	int ret;
	
	/* Building empty signal set */
	sigemptyset(&sig.sa_mask);
	
	/* Building Signal */
	sig.sa_handler	 = function;
	sig.sa_flags	 = 0;
	/* sig.sa_sigaction = NULL; */
	
	/* Installing Signal */
	if((ret = sigaction(signal, &sig, NULL)) == -1)
		perror("sigaction");
	
	return ret;
}

void convert_legacy_lines(ligne_t *lines, ligne_legacy_t *legacy) {
	int i = 0;
	
	while(legacy[i].couleur != LEGACY_COLOR_EOF) {
		// Copy Position
		memcpy(lines[i].position, legacy[i].position, sizeof(legacy[i].position));
		
		switch(legacy[i].couleur) {
			case LEGACY_COLOR_BLUE:
				lines[i].couleur = Qt::blue;
			break;
			
			case LEGACY_COLOR_RED:
				lines[i].couleur = Qt::red;
			break;
			
			case LEGACY_COLOR_YELLOW:
				lines[i].couleur = Qt::yellow;
			break;
			
			case LEGACY_COLOR_GREEN:
				lines[i].couleur = Qt::green;
			break;
			
			case LEGACY_COLOR_WHITE:
				lines[i].couleur = Qt::white;
			break;
			
			case LEGACY_COLOR_BLACK:
				lines[i].couleur = Qt::black;
			break;
			
			default:
				debugc("Unknown Color Conversion: %d\n", legacy[i].couleur);
		}
		
		i++;
	}
	
	debug("DBG: %d lines converted\n", i);
}

int main(int argc, char *argv[]) {
	FenetrePlanVille *F1;
	message_t message;	/* Message Queue */
	int mkey_id;		/* Message Queue ID */
	key_t skey;		/* Shared Memory Segment ID */
	int skey_id;		/* Shared Memory key */
	char *shm;		/* Shared Memory Segment */

	char *cname;		/* Client Name */
	
	int rc;
	
	QApplication a(argc, argv);
	
	printf("Loading...\n");
	
	/* Affecting Global Settings */
	sys.mkey_id = &mkey_id;
	
	/* Intercept ALARM */
	signal_intercept(SIGALRM, sig_handler);
	
	/* Intercept SIGUSR1: Advertissment */
	signal_intercept(SIGUSR1, sig_handler);
	
	/* Incercept SIGUSR2: Sys Admin Message */
	signal_intercept(SIGUSR2, sig_handler);
	
	/* Interrupt SIGINT: Closing */
	signal_intercept(SIGINT, sig_handler);
	
	mkey_id = msgget(MESSAGE_KEY_ID, IPC_CREAT | 0666);
	
	/* Setting Client Name */
	if(argc > 1)
		cname = argv[1];
	else
		cname = "Untitled Client";
	
	/* Requesting Login */
	if(!send_message(PROTO_QRY_LOGIN, (void*) cname)) {
		debug("Cannot send message to server\n");
		return 1;
	}
	
	/* Attaching Shared Memory Segment */
	skey = SHARED_MEMORY_ID;
	if((skey_id = shmget(skey, SHARED_MEMORY_SIZE, 0666)) < 0) {
		perror("shmget");
		return 1;
	}
	
	if((shm = (char *) shmat(skey_id, NULL, 0)) == (char *) -1) {
		perror("shmat");
		return 1;
	}
	
	/* Linking shm to global */
	sys.shm = shm;
	
	/* DEBUG: shm content */
	printf("DEBUG: <%s>\n", shm);

	/* Building GUI */
	F1 = new FenetrePlanVille();
	
	/* Saving Window's Address */
	sys.window = F1;
	
	// a.setMainWidget(&F1);
	F1->show();
	F1->NomVille->setText(cname);
	
	/* Requesting Path List */
	if(!send_message(PROTO_QRY_PATHLIST, (void*) "Wantz Path List"))
		debugc("Cannot downloading map\n");
	
	printf("QRY: Path List\n");
	if((rc = msgrcv(mkey_id, &message, sizeof(message), getpid(), 0)) == -1) {
		perror("msgrcv:");
		exit(1);
	}
	
	if(message.request == PROTO_ACK_PATHLIST) {
		printf("ACK: Path List\n");
		memcpy(stations, message.text, sizeof(stations));
		
	} else debugc("Wrong opcode: %d\n", message.request);
	
	
	/* Requesting Lignes List */
	if(!send_message(PROTO_QRY_LINESLIST, (void*) "Wantz Lines List"))
		debugc("Cannot downloading lignes\n");
	
	printf("QRY: Lignes List\n");
	if((rc = msgrcv(mkey_id, &message, sizeof(message), getpid(), 0)) == -1) {
		perror("msgrcv:");
		exit(1);
	}
	
	if(message.request == PROTO_ACK_LINESLIST) {
		printf("ACK: Ligne List\n");
		
		/* Copy Data */
		memcpy(legacy_lignes, message.text, sizeof(legacy_lignes));
		
		/* Converting Legacy Lines to Qt-Compatible Lines */
		convert_legacy_lines(lignes, legacy_lignes);
		
	} else debugc("Wrong opcode: %d\n", message.request);
	
	/* Writing Map */
	F1->TracePlan(F1->framePlan);
	
	printf("Drawing GUI Interface\n");
	
	/* Let's GUI working */
	return a.exec();
}
