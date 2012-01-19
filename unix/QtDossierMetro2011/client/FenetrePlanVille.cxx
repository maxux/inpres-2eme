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

#include <cstdio>
#include <cstdlib>
#include <signal.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>


#include "FenetrePlanVille.hxx"
#include "ipc_messages.hxx"
#include "metro_protocol.hxx"
#include "debug.hxx"

global_t sys;

/* Metro Data */
station_t stations[METRO_MAX_STATION];
ligne_t lignes[METRO_MAX_LIGNE];
ligne_legacy_t legacy_lignes[METRO_MAX_LIGNE];
pathway_t pathway;


void TracePlan();
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
	textInformation->setText(tr("Chargement de la borne..."));
	LabelMinutes->setText(tr("minutes"));
	LabelDestination->setText(tr("Destination:"));
	ButtonSelection->setText(tr("Selection"));
	ButtonTerminer->setText(tr("Terminer"));
	LabelNomVille->setText(tr("Station"));

	lineDestination->setFocus();
}

void FenetrePlanVille::Terminer() {
	send_message(QRY_LOGOUT, (void*) "kthxbye", 0);
	fclose(sys.log);
	
	exit(0);
}

int get_station_id(const char *name) {
	int i = 1;
	
	while(stations[i].L != 0 && stations[i].C != 0) {
		if(strcmp(stations[i].station, name) == 0)
			return i;
		
		i++;
	}
	
	return 0;
}

void FenetrePlanVille::Selection() {
	ask_pathway_t ask;
	message_t message;
	
	/* Empty Station */
	if(lineDestination->text().length() == 0)
		return;
	
	ask.client = getpid();
	ask.from   = sys.station_id;
	
	/* Qt3 Fix */
	#ifdef __sun
		ask.to     = get_station_id(lineDestination->text().ascii()); // LineEdit -> char
	#else
		ask.to     = get_station_id(lineDestination->text().toStdString().c_str()); // LineEdit -> string -> char
	#endif
	
	/* Invalid Station */
	if(ask.to == 0)
		return;
	
	/* Requesting */
	debug("Searching path from %d to %d\n", ask.from, ask.to);
	send_message(QRY_SEARCH, (void*) &ask, sizeof(ask));
	
	/* Waiting response */
	read_message(&message);
	
	if(message.request == ACK_SEARCH) {
		memcpy(&pathway, message.text, sizeof(pathway));
		
		/* Repainting */
		draw.need_repaint = true;
		draw.todo = DRAW_PATH;
		repaint();
		
	} else debugc("ERR: Invalid answer from server: %d\n", message.request);
}

void FenetrePlanVille::AffichePub(QTextEdit *T, const char *P) {
	T->setText(P);
}

void FenetrePlanVille::paintEvent(QPaintEvent *event) {
	QPainter paint(PAINTEVENT_FIX_OUTPUT);
	int i = 0;
	
	if(sys.first_start && sys.interface_ready) {
		/* Redraw map if it's the first time */
		sys.first_start = 0;
		
		AffichePub(textInformation, " ");		
		TracePlan();
	}
	
	if(draw.need_repaint) {
		event = NULL;
		
		/* Qt3 Fix */
		framePlan->erase(2, 2, 546, 296);

		switch(draw.todo) {
			case DRAW_PLAN:
				debug("Drawing map\n");
				while(legacy_lignes[i].couleur != LCOLOR_EOF)
					TraceParcours(i++, paint);
					
			break;
			
			case DRAW_PATH:
				debug("Drawing map with lines\n");
				while(legacy_lignes[i].couleur != LCOLOR_EOF)
					TraceParcours(i++, paint);
					
				TraceChemin(pathway.nbstation, pathway.step, paint);
			break;
		}
		
		TraceStation(paint);
		draw.need_repaint = false;
	}
}

void FenetrePlanVille::TracePlan() {
	/* Notify Redraw Event to redraw the map */
	draw.need_repaint = true;
	draw.todo = DRAW_PLAN;
	repaint();
}

int on_pathway(int id) {
	int i;
	
	for(i = 1; i < pathway.nbstation; i++)
		if(pathway.step[i] == id)
			return 1;
	
	return 0;
}

void FenetrePlanVille::TraceParcours(int Nb, QPainter &paint) {
	int i = 0;
	
	paint.setPen(lignes[Nb].couleur);
	
	printf("LINE: %d\n", Nb);
	
	while(lignes[Nb].position[i + 1].N && lignes[Nb].position[i + 1].L) {
		printf("Writing Line (%d, %d) -> (%d, %d)\n", lignes[Nb].position[i].L, lignes[Nb].position[i].C, lignes[Nb].position[i + 1].L, lignes[Nb].position[i + 1].C);
		paint.drawLine(lignes[Nb].position[i].L + FIX_FRAME_Y, lignes[Nb].position[i].C + FIX_FRAME_X, lignes[Nb].position[i + 1].L + FIX_FRAME_Y, lignes[Nb].position[i + 1].C + FIX_FRAME_X);
		i++;
	}
}

void FenetrePlanVille::TraceStation(QPainter &paint) {
	QPen pen;
	int i = 1;
	char buffer[64];
	
	while(stations[i].L && stations[i].C) {
		/* If station is disabled */
		if(!stations[i].enabled) {
			/* Draw "Disabled" */
			debug("DBG: Station #%d is disabled\n", i);
			
			sprintf(buffer, "%s (Ferme)", stations[i].station);
			
			/* Writing on map */
			paint.setPen(Qt::darkRed);
			paint.drawText(stations[i].L + FIX_FRAME_Y, stations[i].C + FIX_FRAME_X, buffer);
			
		} else {
			/* Station enabled */
			
			/* Avoid overdraw pathway (red) */
			if(!on_pathway(i)) {
				pen.setWidth(1);
				paint.setPen(pen);
				
				/* Marking current station in blue */
				if(i == sys.station_id)
					paint.setPen(Qt::blue);
				else 
					paint.setPen(Qt::black);
				
				/* Writing on map */
				paint.drawText(stations[i].L + FIX_FRAME_Y, stations[i].C + FIX_FRAME_X, stations[i].station);
			}
		}
		
		i++;
	}
}

void find_xy_station(int station_id, int *x, int *y) {
	int i = 0;
	int j = 0;
	
	while(legacy_lignes[i].couleur != LCOLOR_EOF) {
		for(j = 0; j < 15; j++) {
			if(lignes[i].position[j].N == station_id) {
				*x = lignes[i].position[j].L;
				*y = lignes[i].position[j].C;
				
				return;
			}
		}
		
		i++;
	}
}

void FenetrePlanVille::TraceChemin(int Nb, int Chemin[], QPainter &paint) {
	QPen pen;
	int i = 0;
	int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	
	pen.setWidth(5);
	pen.setColor(Qt::red);
	
	paint.setPen(pen);
	
	while(i < Nb - 1) {
		// Drawing lines
		find_xy_station(Chemin[i], &x1, &y1);
		find_xy_station(Chemin[i + 1], &x2, &y2);
		
		debug("DBG: Drawing Station #%d (%d, %d) -> Station #%d (%d, %d)\n", Chemin[i], x1, y1, Chemin[i + 1], x2, y2);
		
		paint.drawLine(x1 + FIX_FRAME_Y, y1 + FIX_FRAME_X, x2 + FIX_FRAME_Y, y2 + FIX_FRAME_X);
		
		// Drawing over text
		if(i > 0)
			paint.drawText(stations[Chemin[i]].L + FIX_FRAME_Y, stations[Chemin[i]].C + FIX_FRAME_X, stations[Chemin[i]].station);
		
		i++;
	}
	
	if(i == Nb - 1) {		
		pen.setColor(Qt::blue);
		paint.setPen(pen);
		
		paint.drawText(stations[Chemin[i]].L + FIX_FRAME_Y, stations[Chemin[i]].C + FIX_FRAME_X, stations[Chemin[i]].station);
	}
}

void sig_handler(int signum) {
	message_t message;
	char buffer[16];
	
	switch(signum) {
		/* SIGALRM: Next timeout */
		case SIGALRM:
			sprintf(buffer, "%d", sys.next_timeout);
			sys.window->lineMinutes->setText(buffer);
			
			if(sys.next_timeout > 0) {
				sys.next_timeout--;
				alarm(1);
			}
		break;
		
		/* SIGPWR: Ping */
		case SIGPWR:
			send_message(ACK_PONG, (void*) "Pong !", 0);
		break;
		
		/* SIGUSR1: New ads */
		case SIGUSR1:
			if(!sys.interface_ready)
				return;
				
			printf("SIGUSR1 intercepted\n");
			sys.window->AffichePub(sys.window->textInformation, sys.shm);
		break;
		
		case SIGUSR2:
			if(!sys.interface_ready)
				return;
			
			printf("SIGUSR2 intercepted\n");
			sys.window->AffichePub(sys.window->textInformation, sys.shm);
		break;
		
		case SIGWINCH:
			if(!sys.interface_ready)
				return;
			
			send_message(QRY_PATHLIST, (void*) "Ask for Redraw", 0);
			read_message(&message);
			
			if(message.request == ACK_PATHLIST) {
				debug("ACK: Path List\n");
				memcpy(stations, message.text, sizeof(stations));
				
				/* Reset pathway to null */
				memset(&pathway, 0x0, sizeof(pathway));
				
				sys.window->TracePlan();
				
			} else debugc("Wrong opcode: %d\n", message.request);
		break;
		
		case SIGINT:
			sys.window->Terminer();
		break;
		
		case SIGCONT:
			sys.next_timeout = 5;
			sig_handler(SIGALRM);
		break;
	}
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
	
	while(legacy[i].couleur != LCOLOR_EOF) {
		// Copy Position
		memcpy(lines[i].position, legacy[i].position, sizeof(legacy[i].position));
		
		switch(legacy[i].couleur) {
			case LCOLOR_BLUE:
				lines[i].couleur = Qt::blue;
			break;
			
			case LCOLOR_RED:
				lines[i].couleur = Qt::red;
			break;
			
			case LCOLOR_YELLOW:
				lines[i].couleur = Qt::yellow;
			break;
			
			case LCOLOR_GREEN:
				lines[i].couleur = Qt::green;
			break;
			
			case LCOLOR_WHITE:
				lines[i].couleur = Qt::white;
			break;
			
			case LCOLOR_BLACK:
				lines[i].couleur = Qt::black;
			break;
			
			default:
				debugc("Unknown Color Conversion: %d\n", legacy[i].couleur);
		}
		
		i++;
	}
	
	lines[i].couleur = Qt::color0;
	
	debug("DBG: %d lines converted\n", i);
}

int main(int argc, char *argv[]) {
	FenetrePlanVille *F1;
	message_t message;	/* Message Queue */
	int mkey_id;		/* Message Queue ID */
	key_t skey;		/* Shared Memory Segment ID */
	int skey_id;		/* Shared Memory key */
	char *shm;		/* Shared Memory Segment */
	char logs[32];		/* Log Filename */

	char *cname;		/* Client Name */
	
	QApplication a(argc, argv);
	
	/* Init Logs */
	sprintf(logs, "../log/client-%d.log", (int) getpid());
	sys.log = fopen(logs, "w");
	if(!sys.log) {
		perror("fopen");
		return 2;
	}
	
	debugn("Loading...\n");
	
	/* Affecting Global Settings */
	sys.mkey_id         = &mkey_id;
	sys.interface_ready = 0;
	sys.first_start     = 1;
	
	/* Intercept ALARM: Station timeout */
	signal_intercept(SIGALRM, sig_handler);
	
	/* Intercept SIGUSR1: Advertissment */
	signal_intercept(SIGUSR1, sig_handler);
	
	/* Intercept SIGUSR2: Sys Admin Message */
	signal_intercept(SIGUSR2, sig_handler);
	
	/* Intercept SIGPWR: Ping Query */
	signal_intercept(SIGPWR, sig_handler);
	
	/* Intercept SIGINT: Closing */
	signal_intercept(SIGINT, sig_handler);

	/* Intercept SIGWINCH: Refresh map */
	signal_intercept(SIGWINCH, sig_handler);
	
	/* Intercept SIGWINCH: Metro on Station */
	signal_intercept(SIGCONT, sig_handler);
	
	if((mkey_id = msgget(MESSAGE_KEY_ID, IPC_CREAT | 0666)) < 0) {
		perror("msgget");
		return 1;
	}
	
	/* Setting Client Name */
	if(argc > 1)
		cname = argv[1];
	else
		cname = (char*) "Untitled Client";
	
	debug("DBG: Init Client <%s>...\n", cname);
	
	/* Requesting Login */
	if(!send_message(QRY_LOGIN, (void*) cname, 0)) {
		debug("Cannot send message to server\n");
		return 1;
	}
	
	read_message(&message);
	
	if(message.request == ACK_LOGIN) {
		debug("ACK: Login valid\n");
	
	/*
	 * if(setpgid(getpid(), message.data) == -1)
	 *	perror("setpgid");
	 * 
	 */
		
	} else if(message.request == ERR_DENIED) {
		debugc("ERR: Cannot login: (#%d) %s\n", message.request, message.text);
		return 1;
		
	} else {
		debugc("ERR: Unknown opcode (%d) from server\n", message.request);
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
	
	/* Init pathway to null */
	memset(&pathway, 0x0, sizeof(pathway));
	
	/* DEBUG: shm content */
	debug("Shared Message: <%s>\n", shm);

	/* Building GUI */
	F1 = new FenetrePlanVille();
	
	/* Saving Window's Address */
	sys.window = F1;
	
	// a.setMainWidget(&F1);
	F1->show();
	F1->NomVille->setText(cname);
	
	/* Requesting Path List */
	if(!send_message(QRY_PATHLIST, (void*) "Wantz Path List", 0))
		debugc("Cannot downloading map\n");
	
	debug("QRY: Path List\n");
	read_message(&message);
	
	if(message.request == ACK_PATHLIST) {
		debug("ACK: Path List\n");
		memcpy(stations, message.text, sizeof(stations));
		
	} else debugc("Wrong opcode: %d\n", message.request);
	
	/* Finding our station id */
	sys.station_id = get_station_id(cname);
	debug("DBG: We are id #%d\n", sys.station_id);
	
	/* Requesting Lignes List */
	if(!send_message(QRY_LINESLIST, (void*) "Wantz Lines List", 0))
		debugc("Cannot downloading lignes\n");
	
	debug("QRY: Lignes List\n");
	read_message(&message);
	
	if(message.request == ACK_LINESLIST) {
		debug("ACK: Ligne List\n");
		
		/* Copy Data */
		memcpy(legacy_lignes, message.text, sizeof(legacy_lignes));
		
		/* Converting Legacy Lines to Qt-Compatible Lines */
		convert_legacy_lines(lignes, legacy_lignes);
		
	} else debugc("Wrong opcode: %d\n", message.request);
	
	/* Writing Map */	
	debugn("Drawing GUI Interface\n");
	
	/* Let's GUI working */
	sys.interface_ready = 1;
	
	return a.exec();
}
