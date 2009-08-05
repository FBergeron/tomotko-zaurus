/****************************************************************************
* HistoryField.h
* 
* The code of this class is borrowed/inspired from qhistorylineedit.h found in 
* ZTen project.
****************************************************************************/


#ifndef HISTORY_FIELD_H
#define HISTORY_FIELD_H

#include <iostream.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qapplication.h>
#include <qwidget.h>
#include "Util.h"

class HistoryField : public QComboBox {

    Q_OBJECT

public:

    HistoryField( QWidget* parent, const char* name = 0 );
    //void activate();
    bool isDigraphEnabled() const;

public slots:

    void setDigraphEnabled( bool isEnabled );

protected:

    bool eventFilter( QObject* obj, QEvent* event );

signals:

    void returnPressed();

private:

    bool digraphEnabled;
    QString buffer;

};

#endif
