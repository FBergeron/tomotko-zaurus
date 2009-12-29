#ifndef PROGRESS_DIALOG_H
#define PROGRESS_DIALOG_H

#include <iostream.h>
#include <qdialog.h>
#include <qlayout.h>
#include <qpainter.h>
#include <qmap.h>
#include <qslider.h>
#include <qtabwidget.h>
#include "EasinessFactorDistribution.h"
#include "Progress.h"
#include "Schedule.h"
#include "SmartListView.h"
#include "SuccessRateDistribution.h"

class ProgressDialog : public QDialog {

    Q_OBJECT
    
public:

    ProgressDialog( QWidget* parent, const ProgressData& progressData );
    ~ProgressDialog();

public slots:

private slots:

private:

    void init();

    const ProgressData&         progressData;

    QVBoxLayout*                mainLayout;
    QTabWidget*                 tab;

    QWidget*                    scheduleTab;
    QVBoxLayout*                scheduleTabLayout;
    QWidget*                    efDistributionTab;
    QVBoxLayout*                efDistributionTabLayout;
    QWidget*                    successRateDistributionTab;
    QVBoxLayout*                successRateDistributionTabLayout;
    QWidget*                    currTermTab;
    QVBoxLayout*                currTermTabLayout;

    Schedule*                   schedule;
    EasinessFactorDistribution* efDistribution;
    SuccessRateDistribution*    successRateDistribution;
    QSlider*                    scheduleSlider;

    SmartListView*              currTermInfo;

};

#endif
