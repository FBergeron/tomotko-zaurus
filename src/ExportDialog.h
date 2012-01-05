#ifndef EXPORT_DIALOG_H
#define EXPORT_DIALOG_H 

#include <qcheckbox.h>
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

class ExportDialog : public QDialog {

    Q_OBJECT
    
public:

    // Should be an enumeration?
    static const int selectionModeTargetLanguage    = 0;
    static const int selectionModeStudyLanguages    = 1;
    static const int selectionModeAllLanguages      = 2;

    ExportDialog( const QString& caption, const QString& message, const QStringList& languages, int selectionMode, Controller* controller, QWidget* parent );
    ~ExportDialog(); 

    QStringList getSelectedLanguages();

private slots:

    void checkAllLanguages() const;

private:

    void init( const QString& caption, const QString& message, const QStringList& languages, int selectionMode );

    QLabel*         messageLabel;
    SmartListView*  languageList;
    QPushButton*    checkAllLanguagesButton;

    QCheckBox*      includeStatsCheckBox;

    QVBoxLayout*    mainLayout;
    Controller*     controller;

};

#endif

