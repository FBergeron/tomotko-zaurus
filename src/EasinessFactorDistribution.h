#ifndef EASINESS_FACTOR_DISTRIBUTION_H
#define EASINESS_FACTOR_DISTRIBUTION_H

#include <iostream.h>
#include <qevent.h>
#include <qmap.h>
#include <qpainter.h>
#include <qwidget.h>
#include "Progress.h"
#include "Util.h"

class EasinessFactorDistribution : public QWidget {

    Q_OBJECT
    
public:

    EasinessFactorDistribution( QWidget* parent, const QMap<int,int>& termsForEF, const int& efValueCount, const float& efAverage, const float& efStandardDeviation );
    ~EasinessFactorDistribution();

public slots:

private slots:

protected:

    void paintEvent( QPaintEvent* event );

private:

    const QMap<int,int>& termsForEF;
    const int& efValueCount;
    const float& efAverage;
    const float& efStandardDeviation;

};

#endif
