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
#include <qwidget.h>

class HistoryField : public QComboBox {

    Q_OBJECT

public:

    HistoryField( QWidget* parent, const char* name = 0 );
    void activate();

protected:

    void keyPressEvent( QKeyEvent* e );

signals:

    void returnPressed();

};

#endif
