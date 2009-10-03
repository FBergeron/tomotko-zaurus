#include "EasinessFactorDistribution.h"

EasinessFactorDistribution::EasinessFactorDistribution( QWidget* parent, const QMap<int,int>& termsForEF )
    : QWidget( parent ), termsForEF( termsForEF ) {
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

    int colCount = valueCount / 2 + 1;

    // Paint title.
    p.setPen( black );
    p.drawText( 20, 10, size().width(), 40, AlignTop | WordBreak, tr( "NumberOfTermsPerEF" ) );

    p.moveTo( 20, 40 );
    p.lineTo( 20, size().height() - 40 );
    p.lineTo( size().width() - 20, size().height() - 40 );

    int interBarGap = 2;
    int barWidth = ( ( size().width() - 20 - 20 - 20 - 20 ) / colCount ) - interBarGap;
    ef = 13;
    total = 0;
    for( int i = 0; i < valueCount; i++, ef += 1 ) {
        int col = i / 2;

        if( termsForEF.contains( ef ) )
            total += termsForEF[ ef ];

        if( i % 2 == 0 ) {
            int barX = 20 + 20 + ( col * ( barWidth + interBarGap ) );

            // Paint vertical bar.
            if( maxTermCount > 0 ) {
                int barHeight = (int)( ( size().height() - 120 ) * total / maxTermCount );
                int barY = size().height() - 40 - barHeight;
                p.setBrush( black );
                if( total > 0 ) 
                    p.drawText( barX, barY - 20, barWidth, 20, AlignTop | AlignHCenter | DontClip, QString::number( total ) );
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
                strEF = QString( ">3.0" );
            else
                strEF = QString( "<%1" ).arg( (double)( ef / 10.0 ) );
            p.drawText( barX - interBarGap / 2, size().height() - 40 + i % 2 * 20, barWidth + interBarGap, 20, AlignTop | AlignHCenter | DontClip, strEF );

            total = 0;
        }
    }
}
