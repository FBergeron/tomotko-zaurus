#ifndef PROPERTIES_PANEL_H
#define PROPERTIES_PANEL_H 

#include <qdialog.h>
#include <qhbox.h>
#include <qheader.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpe/qpeapplication.h>
#include <qpixmap.h>
#include <qpushbutton.h>
#include <qstring.h>
#include <qvbox.h>
#include <qwidget.h>
#include "ContentListView.h"
#include "DigraphLineEdit.h"
#include "DigraphMultiLineEdit.h"
#include "Folder.h"
#include "Preferences.h"
#include "Vocabulary.h"

class PropertiesPanel : public QVBox {

    Q_OBJECT
    
public:

    PropertiesPanel( const Preferences& prefs, QWidget* parent );
    ~PropertiesPanel(); 

    void setVocabulary( Vocabulary* vocab );
    void setFolder( Folder* folder );

    void updateFonts();
    void setDigraphEnabled( bool isEnabled );
    void retranslateUi();

public slots:

    void updateCounters();

private slots:
    
    void updateDescription();
    void updateAuthor( const QString& author );

private:

    void init();
    void updateModel();

    void addListeners();
    void removeListeners();

    QString getType() const;

    QHBox*                  simplePropsPanel;

    QVBox*                  simplePropsLabelsPanel;
    QVBox*                  simplePropsFieldsPanel;

    QLabel*                 descriptionLabel;
    DigraphMultiLineEdit*   descriptionMultiLineEdit;

    QLabel*                 contentLabel;
    ContentListView*        contentListView;

    QLabel*                 authorLabel;
    DigraphLineEdit*        authorLineEdit;

    QLabel*                 creationDateLabel;
    QLabel*                 creationDateValueLabel;

    QLabel*                 modificationDateLabel;
    QLabel*                 modificationDateValueLabel;

    const Preferences&      prefs;
    Vocabulary*             editedVocab;
    Folder*                 editedFolder;

};

#endif
