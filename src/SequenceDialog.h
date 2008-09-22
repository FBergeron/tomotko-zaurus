#ifndef SEQUENCE_DIALOG_H
#define SEQUENCE_DIALOG_H 

#include <qdialog.h>
#include <qevent.h>
#include <qhbox.h>
#include <qhgroupbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qvaluestack.h>
#include <qvbox.h>
#include <qvgroupbox.h>
#include "Preferences.h"
#include "Sequence.h"

class SequenceDialog : public QDialog {

    Q_OBJECT
    
public:

    SequenceDialog( Preferences* prefs, QWidget* parent );
    ~SequenceDialog(); 

    Sequence getSequence() const;

public slots:

protected slots:

    virtual void accept();

private slots:

    virtual bool eventFilter( QObject* obj, QEvent* evt );

    void addSequenceItem( const QString& itemStr );
    void addSequenceMark();
    void addGroupMark();
    void removeLastToken();

private:

    void init();
    void setUnionButtonsEnabled( bool isEnabled );
    bool isItemUsed( const Sequence::Item& item ) const;
    bool areItemsLeft() const;
    void setItemButtonsEnabled( bool isEnabled );

    Preferences*                prefs;

    Sequence                    sequence;

    QValueStack<Sequence::Item> items; 
    Sequence::ItemList          currGroup;

    QVBoxLayout*                mainLayout;

    QHBox*                      sequencePanel;

    QVGroupBox*                 quizPanelWrapper;
    QVBox*                      quizPanel;

    QHBox*                      quizTopPanel;
    QVBox*                      quizTopLeftPanel;
    QVGroupBox*                 quizImagePanel;
    QPushButton*                quizImageButton;
    
    QHGroupBox*                 quizFirstLangPanel;
    QHBox*                      quizFirstLangPanelWrapper;
    QLabel*                     quizFirstLangLabel;
    QPushButton*                quizFirstLangTermButton;

    QHGroupBox*                 quizTestLangPanel;
    QHBox*                      quizTestLangPanelWrapper;
    QVBox*                      quizTestLangLabelsPanel;
    QVBox*                      quizTestLangButtonsPanel;;

    QLabel*                     quizTestLangAltLabel;
    QLabel*                     quizTestLangTermLabel;
    QPushButton*                quizTestLangAltButton;
    QPushButton*                quizTestLangTermButton;

    QVBox*                      quizCommentBox;
    QLabel*                     quizCommentLabel;
    QPushButton*                quizCommentButton;

    QVBox*                      sequencePanelButtons;
    QPushButton*                addSequenceMarkButton;
    QPushButton*                addGroupMarkButton;
    QPushButton*                removeLastMarkButton;

    QHBox*                      sequenceLinePanel;
    QLabel*                     sequenceLineLabel;
    QLineEdit*                  sequenceLineLineEdit;

};

#endif
