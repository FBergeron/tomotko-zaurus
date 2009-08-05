#include "ProgressDialog.h"

ProgressDialog::ProgressDialog( QWidget* parent, const ProgressData& progressData )
    : QDialog( parent, "ProgressDialog", true ), progressData( progressData ) {
    init();
}

ProgressDialog::~ProgressDialog() {
}

void ProgressDialog::init() {
    tab = new QTabWidget( this );

    scheduleTab = new Schedule( this, progressData.scheduleForDay );

    currTermTab = new QWidget( this );

    tab->addTab( scheduleTab, tr( "ScheduleInfo" ) );
    tab->addTab( currTermTab, tr( "CurrentTermInfo" ) );

    mainLayout = new QVBoxLayout( this );
    mainLayout->addWidget( tab, 1 );

    setCaption( tr( "ProgressDialogCaption" ) );
    setSizeGripEnabled( true );
}
