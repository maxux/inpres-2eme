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
	
	#define MESSAGE_MAX_SIZE	20
	
	typedef struct {
		long lType;
		pid_t idProcess;
		int requete;
		int message[2];
	} message_t;

	typedef struct {
		long lType;
		pid_t idProcess;
		int requete;
		int message[MESSAGE_MAX_SIZE];
		
	} message_max_t;

#endif
