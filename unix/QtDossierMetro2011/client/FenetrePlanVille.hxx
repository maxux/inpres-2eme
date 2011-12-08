#ifndef FENETREPLANVILLE_H
	#define FENETREPLANVILLE_H

	#include <qvariant.h>
	#include <qdialog.h>
	#include <qpainter.h>

	class QVBoxLayout;
	class QHBoxLayout;
	class QGridLayout;
	class QSpacerItem;
	class QLabel;
	class QLineEdit;
	class QTextEdit;
	class QFrame;
	class QPushButton;
	class QColor;
	
	#define DRAW_PLAN		0x01
	#define DRAW_PATH		0x02
	
	#define FIX_FRAME_X		50
	#define FIX_FRAME_Y		40
	
	typedef struct draw_action_t {
		bool need_repaint;
		char todo;
		
	} draw_action_t;

	class FenetrePlanVille : public QDialog {
		Q_OBJECT

		public:
			FenetrePlanVille(QWidget* parent = 0, const char *name = 0, bool modal = FALSE, Qt::WFlags fl = 0);
			~FenetrePlanVille();

			QLabel* LabelArrivee;
			QLabel* LabelInformation;
			QLabel* LabelMinutes;
			QLabel* LabelDestination;
			QLineEdit* lineMinutes;
			QLineEdit* lineDestination;
			QTextEdit* textInformation;
			QFrame* framePlan;
			QPushButton* ButtonSelection;
			QPushButton* ButtonTerminer;
			QLabel* LabelNomVille;
			QLabel* NomVille;
			
			void AffichePub(QTextEdit *T, const char* P);
			void TracePlan(QFrame* F);
			void TraceParcours(int Nb, QPainter &paint);
			void TraceChemin(int Nb,int Chemin[], QPainter &paint);
			
			/* Fix QPainter */
			void paintEvent(QPaintEvent *event);

		public slots:
			virtual void Terminer();
			virtual void Selection();

		protected:

		protected slots:
			virtual void languageChange();
		
		private:
			draw_action_t draw;

	};
	
	typedef struct global_t {
		FenetrePlanVille *window;
		int *mkey_id;
		char *shm;
		
	} global_t;
	
	#define PROTO_QRY_LOGIN		0x01	// QRY for authenficate client
	#define PROTO_QRY_LOGOUT	0x02	// QRY for unauthentificate client
	#define PROTO_QRY_SEARCH	0x03	// QRY for path finding
	#define PROTO_QRY_SHUTDOWN	0x04	// QRY for daemon's shutdown
	
	#define PROTO_QRY_JOIN_GROUP	0x05	// QRY client to join a process-group
	#define PROTO_ACK_JOIN_GROUP	0x06	// ACK for client process-group
	
	#define PROTO_QRY_PATHLIST	0x07	// QRY for map Path List
	#define PROTO_ACK_PATHLIST	0x08	// ACK for Path List: list on reply
	
	#define PROTO_QRY_LINESLIST	0x09	// QRY for map Lines List
	#define PROTO_ACK_LINESLIST	0x0A	// ACK for Line List: list on reply
	
	#define MESSAGE_KEY_ID		1342
	
	#define SHARED_MEMORY_ID	1342
	#define SHARED_MEMORY_SIZE	2048
	
	#define MESSAGE_MAX_SIZE	2048
	
	/* DEBUG */
	#ifdef COLOR
		#define COLOR_BLUE	"\033[1;34m"
		#define COLOR_GREY	"\033[1;30m"
		#define COLOR_GREEN	"\033[1;32m"
		#define COLOR_RED	"\033[1;31m"
		#define COLOR_NONE	"\033[0m"
	#else
		#define COLOR_BLUE	""
		#define COLOR_GREY	""
		#define COLOR_GREEN	""
		#define COLOR_RED	""
		#define COLOR_NONE	""
	#endif
	
	/* Printing Message */
	#define __debug(...)	{ fprintf(stderr, __VA_ARGS__); \
				fprintf(stderr, COLOR_NONE); }
	
	/* Setting Colors */
	/* Classic Debug */
	#define debug(...)	{ fprintf(stderr, COLOR_GREY); \
				__debug(__VA_ARGS__); }
	
	/* Critical Debug */	
	#define debugc(...)	{ fprintf(stderr, COLOR_RED); \
				__debug(__VA_ARGS__); }
	
	typedef struct message_t {
		long lType;
		pid_t pid;
		int request;
		char text[MESSAGE_MAX_SIZE];
		
	} message_t;
	
	int send_message(int request, void *data);
	
	#define METRO_MAX_LIGNE		20
	#define METRO_MAX_STATION	40
	
	typedef struct station_t {
		char station[20];	/* Nom de la station */
		int L; 	/* Ligne */
		int C;	/* Colonne */
	} station_t;
	
	typedef struct position_t {
		int N; // Numero de la station
		int L; // Ligne
		int C; // Colonne
	} position_t;
	
	/* Colored Schema (QT) */
	typedef struct {
		position_t position[15];
		QColor couleur;
		
	} ligne_t;
	
	/* Colored Schema (interface independant) */
	typedef struct {
		position_t position[15];
		short couleur;
		
	} ligne_legacy_t;
	
	#define LEGACY_COLOR_BLUE		0x00
	#define LEGACY_COLOR_RED		0x01
	#define LEGACY_COLOR_YELLOW		0x02
	#define LEGACY_COLOR_GREEN		0x03
	#define LEGACY_COLOR_WHITE		0x04
	#define LEGACY_COLOR_BLACK		0x05
	#define LEGACY_COLOR_EOF		0xFF
#endif
