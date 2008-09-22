#ifndef TERM_DIALOG_H
#define TERM_DIALOG_H 

#include <qaction.h>
#include <qpe/qpeapplication.h>
#include <qcanvas.h>
#include <qdialog.h>
#include <qevent.h>
#include <qfont.h>
#include <qgrid.h>
#include <qhbox.h>
#include <qhgroupbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qmovie.h>
#include <qmultilineedit.h>
#include <qpixmap.h>
#include <qpushbutton.h>
#include <qstring.h>
#include <qtooltip.h>
#include <qvbox.h>
#include <qvgroupbox.h>
#include <qwidget.h>
#include <qpe/resource.h>
#include "BilingualKey.h"
#include "Controller.h"
#include "DigraphLineEdit.h"
#include "DigraphMultiLineEdit.h"
#include "PixMap.h"
#include "Term.h"
#include "Util.h"
#include "Vocabulary.h"
#include "ZFileDialog.h"

class TermDialog : public QDialog {

    Q_OBJECT
    
public:

    TermDialog( Vocabulary& vocab, Controller* controller, QWidget* parent );
    TermDialog( Vocabulary& vocab, Controller* controller, QWidget* parent, const Term& term );
    ~TermDialog(); 

    const Term& getTerm();

public slots:

    void show();

private slots:
    
    void cut();
    void copy();
    void paste();

    void setImage();
    void clearImage();

private:

    void init();
    void updateUi();
    void updateModel();

    void initImage( const QString& imagePath );
    void resizeImage() const;

    QMenuBar*               menuBar;

    QVBoxLayout*            mainLayout;

    QHBox*                  topPanel;
    QVBox*                  topLeftPanel;

    QHGroupBox*             firstLangPanel;
    QVGroupBox*             testLangPanel;

    QHBox*                  firstLangTermPanel;
    QLabel*                 firstLangTermLabel;
    DigraphLineEdit*        firstLangTermLineEdit;

    QWidget*                testLangTopPanel;
    QBoxLayout*             testLangTopPanelLayout;
    QVBox*                  testLangLabelsPanel;
    QVBox*                  testLangFieldsPanel;
    
    QLabel*                 testLangTermLabel;
    DigraphLineEdit*        testLangTermLineEdit;

    QLabel*                 testLangTermAltLabel;
    DigraphLineEdit*        testLangTermAltLineEdit;
    
    QLabel*                 commentLabel;
    DigraphMultiLineEdit*   commentMultiLineEdit;

    QVGroupBox*             imageBox;
    QWidget*                imagePanel;
    QVBoxLayout*            imagePanelLayout;
    QVBox*                  imageWrapper;
    QLabel*                 image;
    QHBox*                  imageButtonsPanel;
    QPushButton*            setImageButton;
    QPushButton*            clearImageButton;

    Vocabulary&             vocab;
    Controller*             controller;
    Term*                   editedTerm;

    QAction*                cutAction;
    QAction*                copyAction;
    QAction*                pasteAction;

    QString                 tempImagePath; // Contains absolute path.
    QString                 imageFormat;

};

#endif
