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

    tab = new QTabWidget( this );

    scheduleTab = new QWidget( this );
    scheduleTabLayout = new QVBoxLayout( scheduleTab );
    scheduleTabLayout->setSpacing( 2 );
    schedule = new Schedule( scheduleTab, progressData.scheduleForDay );
    scheduleSlider = new QSlider( 1, 4, 1, 1, QSlider::Horizontal, scheduleTab );
    connect( scheduleSlider, SIGNAL( valueChanged( int ) ), schedule, SLOT( setInterval( int ) ) );
    scheduleTabLayout->addWidget( schedule, 1 );
    scheduleTabLayout->addWidget( scheduleSlider );

    efDistributionTab = new QWidget( this );
    efDistributionTabLayout = new QVBoxLayout( efDistributionTab );
    efDistributionTabLayout->setSpacing( 2 );
    efDistribution = new EasinessFactorDistribution( efDistributionTab, progressData.efDistribution, progressData.efValueCount, progressData.efAverage, progressData.efStandardDeviation );
    efDistributionTabLayout->addWidget( efDistribution, 1 );

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
    }

    tab->addTab( scheduleTab, tr( "ScheduleInfo" ) );
    tab->addTab( efDistributionTab, tr( "EFDistributionInfo" ) );
    if( isDataForCurrTermAvailable )
        tab->addTab( currTermTab, tr( "CurrentTermInfo" ) );

    mainLayout->addWidget( tab, 1 );

    setCaption( tr( "ProgressDialogCaption" ) );
    setSizeGripEnabled( true );
}
