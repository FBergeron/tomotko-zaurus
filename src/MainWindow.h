#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream.h>
#include <qaction.h>
#include <qclipboard.h>
#include <qcombobox.h>
#include <qdir.h>
#include <qevent.h>
#include <qfile.h>
#include <qfont.h>
#include <qfontdatabase.h>
#include <qlistview.h>
#include <qpixmap.h>
#include <qpopupmenu.h>
#include <qprogressbar.h>
#include <qmainwindow.h>
//#include <qmenubar.h>
#include <qmessagebox.h>
#include <qpe/fileselector.h>
#include <qpe/resource.h>
#include <qpe/qpemenubar.h>
#include <qpe/qpetoolbar.h>
#include <qpixmap.h>
#include <qpushbutton.h>
#include <qstringlist.h>
#include <qthread.h>
#include <qtranslator.h>
#include <qwidgetstack.h>
#include <stdio.h>
#include "Controller.h"
#include "PixMap.h"
#include "PreferencesDialog.h"
#include "QuizFrame.h"
#include "VocabularyManagerFrame.h"
#include "Term.h"

extern QAction* action[ ACTION_COUNT ];

class MainWindow : public QMainWindow {

    // Should be an enumeration.
    static const int frameQuizIndex         = 0;
    static const int frameVocabManagerIndex = 1;

    Q_OBJECT
    
public:

    MainWindow( Controller* controller );
    ~MainWindow();

    Controller* controller();

    bool isDigraphEnabled() const;

public slots:

    void updateMenus( QListViewItem* item );
    void updateFonts();
    void setDigraphEnabled( bool isEnabled );
    void retranslateUi();
    void switchLanguage( const QString& language );
    void switchLanguage( QAction* langAction );
    void toggleMaximize( bool isOn ); 
    void search();

protected:

    void closeEvent( QCloseEvent* );

private slots:

    void about();
    void startQuiz();
    void showQuiz();
    void resumeQuiz();
    void invokeVocabularyManager();
    void importData();
    void exportData();
    void preferences();
    void copy();
    void cut();
    void paste();
    void toggleLanguageFilter();
    void quit();
    void setFirstLanguage( const QString& lang );
    void setTestLanguage( const QString& lang );
    void switchFirstAndTestLanguages();
    
private:

    void setLanguageFilterEnabled( bool isEnabled );
    void updateFirstLanguageValues();
    void updateTestLanguageValues();
    void selectLanguage( QComboBox* comboBox, const QString& langCode );
    void updateLanguageSelector( QComboBox* comboBox );

    QMap<QString,QString>   availableLanguages;

    int                     editionMenuId;
    int                     viewMenuId;
    int                     showAllVocabSeparatorId;

    Controller*             control;

    QWidgetStack*           mainPanel;

    QuizFrame*              quizFrame;
    VocabularyManagerFrame* vocabManagerFrame;

    QPEToolBar*             toolBar;

    QPopupMenu*             actionsMenu;
    QPopupMenu*             helpMenu;

    QAction*                copyAction;
    QAction*                cutAction;
    QAction*                pasteAction;

    QActionGroup*           languageActionGroup;

    QHBox*                  languageSelectorPanel;
    QComboBox*              firstLanguageComboBox;
    QLabel*                 languageSelectorLabel;
    QComboBox*              testLanguageComboBox;

    QProgressBar*           progressBar;

    QPopupMenu*             editionMenu;
    QPopupMenu*             viewMenu;
    QMenuBar*               menuBar;

    QTranslator*            translator;

};

#endif
