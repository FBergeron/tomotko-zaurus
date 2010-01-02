#include "EasinessFactorDistribution.h"

EasinessFactorDistribution::EasinessFactorDistribution( QWidget* parent, const QMap<int,int>& termsForEF, const int& efValueCount, const float& efAverage, const float& efStandardDeviation )
    : QWidget( parent ), termsForEF( termsForEF ), efValueCount( efValueCount ), efAverage( efAverage ), efStandardDeviation( efStandardDeviation ) {
}

EasinessFactorDistribution::~EasinessFactorDistribution() {
}

void EasinessFactorDistribution::paintEvent( QPaintEvent* ) {
    int valueCount = 30 - 13 + 2;
    int ef = 13;
    int total = 0;
    int maxTermCount = 0;

    for( int i = 0; i < valueCount; i++, ef += 1 ) {
        if( termsForEF.contains( ef ) )
            total += termsForEF[ ef ];

        if( i % 2 == 0 ) {
            if( total > maxTermCount )
                maxTermCount = total;

            total = 0;
        }
    }

    QPainter p( this );
    QFontMetrics fm( p.fontMetrics() );

    int margin = 20;
    int padding = 10;
    int y = margin;

    int colCount = valueCount / 2 + 1;

    // Paint title.
    p.setPen( black );
    int titleHeight = ( fm.width( tr( "NumberOfTermsPerEF" ) ) > width() - 2 * margin ? 2 * fm.height() : fm.height() );
    p.drawText( margin, y, size().width() - 2 * margin, titleHeight, AlignTop | WordBreak, tr( "NumberOfTermsPerEF" ) );

    y += padding + titleHeight;

    p.moveTo( margin, y );
    p.lineTo( margin, size().height() - margin - 3 * fm.height() - padding );
    p.lineTo( size().width() - margin, size().height() -margin - 3 * fm.height() - padding );

    int interBarGap = 2;
    int barWidth = ( ( size().width() - 20 - 20 - 20 - 20 ) / colCount ) - interBarGap;
    ef = 13;
    total = 0;
    for( int i = 0; i < valueCount; i++, ef += 1 ) {
        int col = i / 2;

        if( termsForEF.contains( ef ) )
            total += termsForEF[ ef ];

        if( i % 2 == 0 ) {
            int barX = 2 * margin + ( col * ( barWidth + interBarGap ) );

            // Paint vertical bar.
            if( maxTermCount > 0 ) {
                int barHeight = (int)( ( size().height() - y - padding - 4 * fm.height() - margin ) * total / maxTermCount );
                int barY = size().height() - margin - 3 * fm.height() - padding - barHeight;
                p.setBrush( black );
                if( total > 0 ) 
                    p.drawText( barX, barY - fm.height(), barWidth, fm.height(), AlignTop | AlignHCenter | DontClip, QString::number( total ) );
                if( barHeight > 0 ) {
                    p.setBrush( blue );
                    p.drawRect( barX, barY, barWidth, barHeight );
                }
                p.setBrush( black );
            }

            // Paint EF values.
            QString strEF;
            if( ef == 13 )
                strEF = QString( "%1" ).arg( (double)( ef / 10.0 ) );
            else if( ef == 31 )
                strEF = QChar( 0x2265 ) + QString( "3.0" ); // "Greater than or equal to" unicode character
            else
                strEF = QChar( 0x2264 ) + QString( "%1" ).arg( (double)( ef / 10.0 ) ); // "Less than or equal" unicode character
            p.drawText( barX - interBarGap / 2, size().height() - margin - 3 * fm.height() - padding + i % 2 * fm.height(), barWidth + interBarGap, fm.height(), AlignTop | AlignHCenter | DontClip, strEF );

            total = 0;
        }
    }

    y = size().height() - margin - fm.height(); 
    p.drawText( margin, y, tr( "EFValueCount" ).arg( efValueCount ) );

    QString averageString = tr( "EFAverage" ).arg( efValueCount > 0 ? QString::number( efAverage ) : QObject::tr( "N/A" ) );
    p.drawText( size().width() - p.fontMetrics().width( averageString ) - margin, y, averageString );

    y += fm.height();
    
    QString standardDeviationString = tr( "EFStandardDeviation" ).arg( efValueCount > 0 ? QString::number( efStandardDeviation ) : QObject::tr( "N/A" ) );
    p.drawText( size().width() - p.fontMetrics().width( standardDeviationString ) - margin, y, standardDeviationString );
}
