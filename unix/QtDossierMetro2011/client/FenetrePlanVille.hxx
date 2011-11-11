#ifndef FENETREPLANVILLE_H
	#define FENETREPLANVILLE_H

	#include <qvariant.h>
	#include <qdialog.h>

	class QVBoxLayout;
	class QHBoxLayout;
	class QGridLayout;
	class QSpacerItem;
	class QLabel;
	class QLineEdit;
	class QTextEdit;
	class QFrame;
	class QPushButton;

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

		public slots:
			virtual void Terminer();
			virtual void Selection();

		protected:

		protected slots:
			virtual void languageChange();

	};
	
	#define PROTO_LOGIN	0x01
	#define PROTO_LOGOUT	0x02
	#define PROTO_SEARCH	0x03
	#define PROTO_SHUTDOWN	0x04
	
	#define MESSAGE_MAX_SIZE	512
	
	typedef struct message_t {
		long lType;
		pid_t pid;
		int request;
		char text[MESSAGE_MAX_SIZE];
		
	} message_t;
#endif
