#ifndef QUIZ_H
#define QUIZ_H

#include <qapplication.h>
#include <qevent.h>
#include <qfont.h>
#include <qframe.h>
#include <qhbox.h>
#include <qhgroupbox.h>
#include <qiconset.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qmultilineedit.h>
#include <qobjectlist.h>
#include <qpushbutton.h>
#include <qsizepolicy.h>
#include <qstringlist.h>
#include <qtooltip.h>
#include <qvbox.h>
#include <qvgroupbox.h>
#include <qwidgetstack.h>
#include <qpe/resource.h>
#include "Controller.h"
#include "ImageBox.h"
#include "ScrollableLineEdit.h"
#include "ScrollableMultiLineEdit.h"
#include "Term.h"
#include "TermDialog.h"
#include "Translation.h"
#include "Util.h"

extern QAction* action[ ACTION_COUNT ];

class QuizFrame : public QWidget {

    Q_OBJECT

public:

    QuizFrame( Controller* controller, QWidget* parent = 0, const char* name = 0 );
    ~QuizFrame();

    void startQuiz();
    void restartQuiz();
    void resumeQuiz();
    void setTerm( const Term& term );
    void concludeQuiz();
    Term* askCurrentTerm();
    Term* askNextTerm();
    void askTerm( const Term& term );
    void updateLanguageLabels();
    void setButtonsHidden( bool areHidden );
    bool areButtonsHidden() const;

protected:

    void showEvent( QShowEvent* showEvt );
    void hideEvent( QHideEvent* hideEvt );
    bool event( QEvent* evt );

public slots:

    virtual bool eventFilter( QObject* obj, QEvent* evt );

    void scrollLeft();
    void scrollRight();

    void updateFonts();
    void retranslateUi();

    void toggleMaximizeComment( bool isOn );

signals:

    void quizShown();
    void quizHidden();

private slots:

    void rightAnswer();
    void wrongAnswer();
    void reveal();
    void revealAll();
    void revealAltTerm();
    void revealFirstLangTerm();
    void revealTestLangTerm();
    void revealComment();
    void revealImage();

    void editCurrentTerm();

private:

    void init();

    void hideAnswers();
    void setButtonsEnabled( bool isEnabled );

    bool isFirstLangTermRevealed() const;
    bool isAltTermRevealed() const;
    bool isTestLangTermRevealed() const;
    bool isCommentRevealed() const;
    bool isImageRevealed() const;

    void maximizeCommentField();
    void restoreCommentField();

    Controller*                 controller;

    QVBoxLayout*                mainLayout;

    QHBox*                      topPanel;
    QVBox*                      topLeftPanel;

    QHGroupBox*                 firstLangPanel;
    QVGroupBox*                 testLangPanel;

    QWidget*                    controlPanel;
    QHBoxLayout*                controlPanelLayout;
    QHBox*                      answerControlPanel;
    QPushButton*                revealAllDataButton;
    QPushButton*                rightAnswerButton;
    QPushButton*                wrongAnswerButton;
    QPushButton*                editionButton;

    QWidget*                    firstLangTermPanel;
    QHBoxLayout*                firstLangTermPanelLayout;
    
    QLabel*                     firstLangTermLabel;
    QWidgetStack*               firstLangTermStack;
    ScrollableLineEdit*         firstLangTermLineEdit;
    QPushButton*                firstLangTermButton;

    QWidget*                    testLangTopPanel;
    QBoxLayout*                 testLangTopPanelLayout;
    QVBox*                      testLangLabelsPanel;
    QVBox*                      testLangFieldsPanel;
    
    QLabel*                     testLangTermLabel;
    QWidgetStack*               testLangTermStack;
    ScrollableLineEdit*         testLangTermLineEdit;
    QPushButton*                testLangTermButton;

    QLabel*                     testLangTermAltLabel;
    QWidgetStack*               testLangTermAltStack;
    ScrollableLineEdit*         testLangTermAltLineEdit;
    QPushButton*                testLangTermAltButton;
   
    QVBox*                      commentBox;
    QWidget*                    commentLabelPanel;
    QHBoxLayout*                commentLabelPanelLayout;
    QLabel*                     commentLabel;
    QPushButton*                maximizeCommentButton;
    QWidgetStack*               commentStack;
    ScrollableMultiLineEdit*    commentMultiLineEdit;
    QPushButton*                commentButton;

    ImageBox*                   imageBox;
    QPushButton*                imageButton;

    bool                        buttonsHidden;

};

#endif
