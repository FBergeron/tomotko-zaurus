#include "Schedule.h"

Schedule::Schedule( QWidget* parent, const int* termsForDay )
    : QWidget( parent ), termsForDay( termsForDay ), interval( 1 ) {
}

Schedule::~Schedule() {
}

void Schedule::setInterval( int weeks ) {
    interval = weeks;
    repaint();
}

void Schedule::paintEvent( QPaintEvent* ) {
    int scheduleLength = interval * 7;
    int maxTermCount = 0;
    for( int i = 0; i < scheduleLength; i++ ) {
        if( termsForDay[ i ] > maxTermCount )
            maxTermCount = termsForDay[ i ];
    }

    QPainter p( this );
    QFontMetrics fm( p.fontMetrics() );

    int margin = 20;
    int padding = 10;
    int y = margin; 

    // Paint title.
    p.setPen( black );
    int titleHeight = ( fm.width( tr( "NumberOfTermsScheduled" ) ) > width() - 2 * margin ? 2 * fm.height() : fm.height() );
    p.drawText( margin, y, width() - 2 * margin, titleHeight, AlignTop | WordBreak, tr( "NumberOfTermsScheduled" ) );

    y += padding + titleHeight;

    p.moveTo( margin, y );
    p.lineTo( margin, size().height() - padding - 2 * fm.height() - margin );
    p.lineTo( size().width() - margin, size().height() - padding - 2 * fm.height() - margin );

    int interBarGap = (int)( 10 / interval );
    int barWidth = ( ( size().width() - 4 * margin ) / scheduleLength ) - interBarGap;
    for( int i = 0; i < scheduleLength; i++ ) {

        int barX = 2 * margin + ( i * ( barWidth + interBarGap ) );

        // Paint vertical bar.
        if( maxTermCount > 0 ) {
            int barHeight = (int)( ( size().height() - y - padding - 3 * fm.height() - margin ) * termsForDay[ i ] / maxTermCount );
            int barY = size().height() - margin - 2 * fm.height() - padding - barHeight;
            p.setBrush( black );
            if( termsForDay[ i ] > 0 ) 
                p.drawText( barX, barY - fm.height(), barWidth, fm.height(), AlignTop | AlignHCenter | DontClip, QString::number( termsForDay[ i ] ) );
            if( barHeight > 0 ) {
                p.setBrush( blue );
                p.drawRect( barX, barY, barWidth, barHeight );
            }
            p.setBrush( black );
        }

        // Paint weekday.
        if( interval <= 2 ) {
            QString day;

            if( i == 0 )
                day = tr( "Today" );
            else if( i == 1 ) 
                day = tr( "Tomorrow" );
            else {
                day = Util::getWeekday( QDate::currentDate().addDays( i ).dayOfWeek() );

                if( interval > 2 )
                    day = day.left( 1 ); 
                else if( interval > 1 )
                    day = day.left( 3 );
            }

            p.drawText( barX - interBarGap / 2, size().height() - margin - 2 * fm.height() + i % 2 * fm.height(), barWidth + interBarGap, fm.height(), AlignTop | AlignHCenter | DontClip, day );
        }
    }

    // Paint weeks
    if( interval > 2 ) {
        int y = size().height() - margin - 2 * fm.height();
        for( int w = 1; w <= interval; w++ ) {
            QString week = tr( "ScheduleWeekNumber" ).arg( w );
            int leftX = 2 * margin + ( w - 1 ) * ( ( barWidth + interBarGap ) * 7 );
            int rightX = leftX + ( barWidth + interBarGap ) * 7 - interBarGap; 
            p.drawLine( leftX, y - 4, leftX, y );
            p.drawLine( leftX, y, rightX, y );
            p.drawLine( rightX, y - 4, rightX, y );
            int middleX = (int)( leftX + ( ( rightX - leftX - fm.width( week ) ) / 2.0 ) );
            p.drawText( middleX, y + fm.height(), week );
        }
    }
}
