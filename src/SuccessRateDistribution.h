#ifndef SUCCESS_RATE_DISTRIBUTION_H
#define SUCCESS_RATE_DISTRIBUTION_H

#include <iostream.h>
#include <qevent.h>
#include <qmap.h>
#include <qpainter.h>
#include <qwidget.h>
#include "Progress.h"
#include "Util.h"

class SuccessRateDistribution : public QWidget {

    Q_OBJECT

    static const QString numericLabels[];

public:

    SuccessRateDistribution( QWidget* parent, const QMap<int,int>& termsForSuccessRate, const int& successRateValueCount, const float& successRateAverage, const float& successRateStandardDeviation );
    ~SuccessRateDistribution();

public slots:

private slots:

protected:

    void paintEvent( QPaintEvent* event );

private:

    const QMap<int,int>& termsForSuccessRate;
    const int& successRateValueCount;
    const float& successRateAverage;
    const float& successRateStandardDeviation;

};

#endif

