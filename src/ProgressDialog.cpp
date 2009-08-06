#include "ProgressDialog.h"

ProgressDialog::ProgressDialog( QWidget* parent, const ProgressData& progressData )
    : QDialog( parent, "ProgressDialog", true ), progressData( progressData ) {
    init();
}

ProgressDialog::~ProgressDialog() {
}

void ProgressDialog::init() {
    mainLayout = new QVBoxLayout( this );

    bool isDataForCurrTermAvailable = ( progressData.currTerm.easinessFactor != 0.0f );

    if( isDataForCurrTermAvailable )
        tab = new QTabWidget( this );

    scheduleTab = new Schedule( this, progressData.scheduleForDay );

    if( isDataForCurrTermAvailable ) {
        currTermTab = new QWidget( this );
        currTermTabLayout = new QVBoxLayout( currTermTab );

        currTermInfo = new SmartListView( currTermTab );
        currTermInfo->setShowSortIndicator( false );
        currTermInfo->setAllColumnsShowFocus( true );
        currTermInfo->setSelectionMode( QListView::Extended );
        currTermInfo->addColumn( tr( "Property" ) );
        currTermInfo->addColumn( tr( "Value" ) );
        currTermInfo->setStretchColumn( 0 );

        QListViewItem* repetitionListViewItem = new QListViewItem( currTermInfo, tr( "Repetition" ), QString::number( progressData.currTerm.repetition ) );
        currTermInfo->insertItem( repetitionListViewItem );

        QListViewItem* easinessFactorListViewItem = new QListViewItem( currTermInfo, tr( "EasinessFactor" ), QString::number( progressData.currTerm.easinessFactor ) );
        currTermInfo->insertItem( easinessFactorListViewItem );

        QListViewItem* daysToNextRepetitionListViewItem = new QListViewItem( currTermInfo, tr( "DaysToNextRepetition" ), QString::number( progressData.currTerm.daysToNextRepetition ) );
        currTermInfo->insertItem( daysToNextRepetitionListViewItem );

        currTermTabLayout->addWidget( currTermInfo );

        tab->addTab( scheduleTab, tr( "ScheduleInfo" ) );
        tab->addTab( currTermTab, tr( "CurrentTermInfo" ) );

        mainLayout->addWidget( tab, 1 );
    }
    else {
        mainLayout->addWidget( scheduleTab, 1 );
    }

    setCaption( tr( "ProgressDialogCaption" ) );
    setSizeGripEnabled( true );
}
