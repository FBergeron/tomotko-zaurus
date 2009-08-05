#include "Schedule.h"

Schedule::Schedule( QWidget* parent, const int* termsForDay )
    : QWidget( parent ), termsForDay( termsForDay ) {
}

Schedule::~Schedule() {
}

void Schedule::paintEvent( QPaintEvent* ) {
    int maxTermCount = 0;
    for( int i = 0; i < scheduleLength; i++ ) {
        if( termsForDay[ i ] > maxTermCount )
            maxTermCount = termsForDay[ i ];
    }

    QPainter p( this );

    // Paint title.
    p.setPen( black );
    p.drawText( 20, 10, size().width(), 40, AlignTop | WordBreak, tr( "NumberOfTermsScheduled" ) );

    p.moveTo( 20, 40 );
    p.lineTo( 20, size().height() - 60 );
    p.lineTo( size().width() - 20, size().height() - 60 );

    int interBarGap = 10;
    int barWidth = ( ( size().width() - 20 - 20 - 20 - 20 ) / scheduleLength ) - interBarGap;
    for( int i = 0; i < scheduleLength; i++ ) {

        int barX = 20 + 20 + ( i * ( barWidth + interBarGap ) );

        // Paint vertical bar.
        if( maxTermCount > 0 ) {
            int barHeight = (int)( ( size().height() - 120 ) * termsForDay[ i ] / maxTermCount );
            int barY = size().height() - 60 - barHeight;
            p.setBrush( black );
            if( termsForDay[ i ] > 0 ) 
                p.drawText( barX, barY - 20, barWidth, 20, AlignTop | AlignHCenter | DontClip, QString::number( termsForDay[ i ] ) );
            if( barHeight > 0 ) {
                p.setBrush( blue );
                p.drawRect( barX, barY, barWidth, barHeight );
            }
            p.setBrush( black );
        }

        // Paint weekday.
        QString day;
        if( i == 0 )
            day = tr( "Today" );
        else if( i == 1 ) 
            day = tr( "Tomorrow" );
        else 
            day = Util::getWeekday( QDate::currentDate().addDays( i ).dayOfWeek() );
        p.drawText( barX - interBarGap / 2, size().height() - 60 + i % 2 * 20, barWidth + interBarGap, 20, AlignTop | AlignHCenter | DontClip, day );
    }
}
