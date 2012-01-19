#ifndef FENETREPLANVILLE_H
	#define FENETREPLANVILLE_H

	#include <cstdio>
	#include <qvariant.h>
	#include <qdialog.h>
	#include <qpainter.h>
	#include "metro_protocol.hxx"

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
	
	#ifdef __sun
		#define FIX_FRAME_X		0
		#define FIX_FRAME_Y		0
		#define PAINTEVENT_FIX_OUTPUT	framePlan
	#else
		#define FIX_FRAME_X		50
		#define FIX_FRAME_Y		40
		#define PAINTEVENT_FIX_OUTPUT	this
	#endif
	
	#define METRO_MAX_LIGNE		20
	#define METRO_MAX_STATION	40
	
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
			void TracePlan();
			void TraceParcours(int Nb, QPainter &paint);
			void TraceChemin(int Nb,int Chemin[], QPainter &paint);
			void TraceStation(QPainter &paint);
			
			/* Fix QPainter */
			void paintEvent(QPaintEvent *event);
			
			draw_action_t draw;

		public slots:
			virtual void Terminer();
			virtual void Selection();

		protected:

		protected slots:
			virtual void languageChange();
	};
	
	typedef struct global_t {
		FenetrePlanVille *window;
		FILE *log;
		
		int *mkey_id;
		char *shm;
		char interface_ready;
		char first_start;
		int station_id;
		int next_timeout;
		
	} global_t;
#endif
