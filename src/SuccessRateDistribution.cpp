#include "SuccessRateDistribution.h"

const QString SuccessRateDistribution::numericLabels[] = {
    QString( "0" ), QString( "1-10" ), QString( "11-30" ), QString( "31-50" ), 
    QString( "51-70" ), QString( "71-90" ), QString( "91-99" ), QString( "100" ) };

SuccessRateDistribution::SuccessRateDistribution( QWidget* parent, const QMap<int,int>& termsForSuccessRate, const int& successRateValueCount, const float& successRateAverage, const float& successRateStandardDeviation )
    : QWidget( parent ), termsForSuccessRate( termsForSuccessRate ), successRateValueCount( successRateValueCount ), successRateAverage( successRateAverage ), successRateStandardDeviation( successRateStandardDeviation ) {
}

SuccessRateDistribution::~SuccessRateDistribution() {
}

void SuccessRateDistribution::paintEvent( QPaintEvent* ) {
    int colCount = sizeof( SuccessRateDistribution::numericLabels ) / sizeof( QString );
    int maxTermCount = 0;

    for( int i = 0; i < colCount; i++ ) {
        if( termsForSuccessRate.contains( i ) && termsForSuccessRate[ i ] > maxTermCount )
            maxTermCount = termsForSuccessRate[ i ];
    }

    QPainter p( this );
    QFontMetrics fm( p.fontMetrics() );

    int margin = 20;
    int padding = 10;
    int y = margin;

    // Paint title.
    p.setPen( black );
    int titleHeight = ( fm.width( tr( "NumberOfTermsPerSuccessRate" ) ) > width() - 2 * margin ? 2 * fm.height() : fm.height() );
    p.drawText( margin, y, size().width() - 2 * margin, titleHeight, AlignTop | WordBreak, tr( "NumberOfTermsPerSuccessRate" ) );

    y += padding + titleHeight;

    p.moveTo( margin, y );
    p.lineTo( margin, size().height() - margin - 3 * fm.height() - padding );
    p.lineTo( size().width() - margin, size().height() -margin - 3 * fm.height() - padding );

    int interBarGap = 2;
    int barWidth = ( ( size().width() - 20 - 20 - 20 - 20 ) / colCount ) - interBarGap;
    int yNumLabels = size().height() - margin - 3 * fm.height() - padding;
    for( int i = 0; i < colCount; i++ ) {
        int count = ( termsForSuccessRate.contains( i ) ? termsForSuccessRate[ i ] : 0 );
        int barX = 2 * margin + ( i * ( barWidth + interBarGap ) );

        // Paint vertical bar.
        if( maxTermCount > 0 ) {
            int barHeight = (int)( ( size().height() - y - padding - 4 * fm.height() - margin ) * count / maxTermCount );
            int barY = size().height() - margin - 3 * fm.height() - padding - barHeight;
            p.setBrush( black );
            if( count > 0 ) 
                p.drawText( barX, barY - fm.height(), barWidth, fm.height(), AlignTop | AlignHCenter | DontClip, QString::number( count ) );
            if( barHeight > 0 ) {
                p.setBrush( blue );
                p.drawRect( barX, barY, barWidth, barHeight );
            }
            p.setBrush( black );
        }

        // Paint success rate values.
        QString strSuccessRate = SuccessRateDistribution::numericLabels[ i ];
        p.drawText( barX - interBarGap / 2, yNumLabels, barWidth + interBarGap, fm.height(), AlignTop | AlignHCenter | DontClip, strSuccessRate );
    }

    QString strPercent( "%" );
    p.drawText( size().width() - 2 * p.fontMetrics().width( strPercent ) - margin, yNumLabels, p.fontMetrics().width( strPercent ), fm.height(), AlignTop | AlignHCenter, strPercent );

    y = size().height() - margin - fm.height(); 
    p.drawText( margin, y, tr( "SuccessRateValueCount" ).arg( successRateValueCount ) );

    QString averageString = tr( "SuccessRateAverage" ).arg( successRateAverage );
    p.drawText( size().width() - p.fontMetrics().width( averageString ) - margin, y, averageString );

    y += fm.height();
    
    QString standardDeviationString = tr( "SuccessRateStandardDeviation" ).arg( successRateStandardDeviation );
    p.drawText( size().width() - p.fontMetrics().width( standardDeviationString ) - margin, y, standardDeviationString );
}

