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
	
	#define PROTO_LOGIN	0x01
	#define PROTO_LOGOUT	0x02
	#define PROTO_SEARCH	0x03
	#define PROTO_SHUTDOWN	0x04
	
	#define MESSAGE_KEY_ID		1342
	
	#define SHARED_MEMORY_ID	1342
	#define SHARED_MEMORY_SIZE	2048
	
	#define MESSAGE_MAX_SIZE	512
	
	typedef struct message_t {
		long lType;
		pid_t pid;
		int request;
		char text[MESSAGE_MAX_SIZE];
		
	} message_t;
#endif
