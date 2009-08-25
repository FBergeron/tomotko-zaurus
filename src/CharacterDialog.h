#ifndef CHARACTER_DIALOG_H
#define CHARACTER_DIALOG_H 

#include <iostream.h>
//#include <qaction.h>
//#include <qcheckbox.h>
//#include <qcombobox.h>
#include <qdialog.h>
#include <qfileinfo.h>
//#include <qfontdatabase.h>
//#include <qgrid.h>
//#include <qhbox.h>
//#include <qheader.h>
//#include <qhgroupbox.h>
#include <qlabel.h>
#include <qlayout.h>
//#include <qlist.h>
//#include <qmessagebox.h>
#include <qmovie.h>
//#include <qobjectlist.h>
#include <qpixmap.h>
//#include <qpushbutton.h>
//#include <qradiobutton.h>
//#include <qscrollview.h>
//#include <qslider.h>
//#include <qstring.h>
//#include <qstringlist.h>
//#include <qtabwidget.h>
//#include <qtooltip.h>
//#include <qvbox.h>
//#include <qvbuttongroup.h>
//#include <qvgroupbox.h>
//#include "KeyActionListViewItem.h"
//#include "OriginalQuiz.h"
//#include "CharacterDialog.h"
//#include "SequenceDialog.h"
//#include "SequenceListItem.h"
//#include "SmartListView.h"
//#include "StudyLanguageItem.h"

class CharacterDialog : public QDialog {

    Q_OBJECT
    
public:

    CharacterDialog( QWidget* parent, const QString& imageFile );
    ~CharacterDialog(); 

public slots:

protected slots:

    //virtual void accept()s

protected:

    //virtual void resizeEvent( QResizeEvent* evt );

private slots:

    //void updateUi();

private:

    //void keyPressEvent( QKeyEvent *evt );

    QVBoxLayout*        mainLayout;

    QLabel*             image;
    QString             imageFormat;
    QString             imageAbsolutePath;

};

#endif
