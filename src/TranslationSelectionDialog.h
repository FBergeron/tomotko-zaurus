#ifndef TRANSLATION_SELECTION_DIALOG_H
#define TRANSLATION_SELECTION_DIALOG_H 

#include <qdialog.h>
#include <qfont.h>
#include <qhbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qstringlist.h>
#include <qvbox.h>
#include "Controller.h"
#include "SmartListView.h"

class TranslationSelectionDialog : public QDialog {

    Q_OBJECT
    
public:

    // Should be an enumeration?
    static const int selectionModeTargetLanguage    = 0;
    static const int selectionModeStudyLanguages    = 1;
    static const int selectionModeAllLanguages      = 2;

    TranslationSelectionDialog( const QString& caption, const QString& message, const QStringList& languages, int selectionMode, Controller* controller, QWidget* parent );
    ~TranslationSelectionDialog(); 

    QStringList getSelectedLanguages();

private slots:

    void checkAllLanguages() const;

private:

    void init( const QString& caption, const QString& message, const QStringList& languages, int selectionMode );

    QLabel*         messageLabel;
    SmartListView*  languageList;
    QPushButton*    checkAllLanguagesButton;

    QVBoxLayout*    mainLayout;
    Controller*     controller;

};

#endif
