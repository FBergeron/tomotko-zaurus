#ifndef PROGRESS_DIALOG_H
#define PROGRESS_DIALOG_H

#include <iostream.h>
#include <qdialog.h>
#include <qlayout.h>
#include <qpainter.h>
#include <qtabwidget.h>
#include "Progress.h"
#include "Schedule.h"
#include "SmartListView.h"

class ProgressDialog : public QDialog {

    Q_OBJECT
    
public:

    ProgressDialog( QWidget* parent, const ProgressData& progressData );
    ~ProgressDialog();

public slots:

private slots:
    
private:

    void init();

    const ProgressData&     progressData;

    QVBoxLayout*            mainLayout;
    QTabWidget*             tab;

    Schedule*               scheduleTab;
    QWidget*                currTermTab;
    QVBoxLayout*            currTermTabLayout;

    SmartListView*          currTermInfo;

};

#endif
