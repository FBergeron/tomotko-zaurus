#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <iostream.h>
#include <qevent.h>
#include <qpainter.h>
#include <qwidget.h>
#include "Progress.h"
#include "Util.h"

class Schedule : public QWidget {

    Q_OBJECT
    
public:

    Schedule( QWidget* parent, const int* termsForDay );
    ~Schedule();

public slots:

private slots:

protected:

    void paintEvent( QPaintEvent* event );

private:

    const int* termsForDay;

};

#endif
