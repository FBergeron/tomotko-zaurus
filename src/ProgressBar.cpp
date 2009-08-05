#include "ProgressBar.h"

ProgressBar::ProgressBar( QWidget* parent, const char* name = 0 ) : QProgressBar( parent, name ) {
}

void ProgressBar::mouseReleaseEvent( QMouseEvent* ) {
    emit( progressBarClicked() );
}
