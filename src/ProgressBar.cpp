#include "ProgressBar.h"

ProgressBar::ProgressBar( QWidget* parent, const char* name = 0 ) : QProgressBar( parent, name ) {
}

void ProgressBar::mouseReleaseEvent( QMouseEvent* ) {
    emit( progressBarClicked() );
}

bool ProgressBar::setIndicator( QString& indicator, int progress, int totalSteps ) {
    bool hasChanged = QProgressBar::setIndicator( indicator, progress, totalSteps ) || ( prevProgress != progress );
    int np = progress * 100 / totalSteps;
    QString newIndicator;
    newIndicator.sprintf( "(%d/%d) %d%%", progress, totalSteps, np );
    hasChanged = hasChanged || ( indicator != newIndicator );
    if( hasChanged && !indicator.isEmpty()  )
        indicator = newIndicator;
    return( hasChanged );
}

QSize ProgressBar::sizeHint() const {
    QSize size = QProgressBar::sizeHint();
    return( QSize( size.width() * 2, size.height() ) );
}
