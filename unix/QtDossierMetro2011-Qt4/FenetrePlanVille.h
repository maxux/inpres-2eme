/****************************************************************************
** Form interface generated from reading ui file 'FenetrePlanVille.ui'
**
** Created: ven. oct. 14 12:31:24 2011
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

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

class FenetrePlanVille : public QDialog
{
    Q_OBJECT

public:
    FenetrePlanVille( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
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

#endif // FENETREPLANVILLE_H
