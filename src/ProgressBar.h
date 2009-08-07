#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include <iostream.h>
#include <qevent.h>
#include <qprogressbar.h>

class ProgressBar : public QProgressBar {

    Q_OBJECT

public:

    ProgressBar( QWidget* parent, const char* name = 0 );
    QSize sizeHint() const;

signals:

    void progressBarClicked();

protected:

    void mouseReleaseEvent( QMouseEvent* evt );
    bool setIndicator( QString & indicator, int progress, int totalSteps );

};

#endif
