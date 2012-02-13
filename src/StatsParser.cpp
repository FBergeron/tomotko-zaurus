#include "StatsParser.h"

StatsParser::StatsParser(QMap<BiUidKey,TermData>& termData ) : termData( termData ), isStatsFile( false ) {
}

bool StatsParser::startDocument() {
    return( true );
}

bool StatsParser::startElement( const QString&, const QString&, const QString& qname, const QXmlAttributes& attribs ) {
    if( qname == QString( "stats" ) ) {
        termData.clear();
    }
    else if ( qname == QString( "termData" ) ) {
        int tempInterval;
        uint tempRepetition;
        float tempEasinessFactor; // EF
        QDate tempNextRepetitionDate;
        QDate tempLastRepetitionDate;
        uint tempSuccessCount;
        uint tempMissCount;
       
        currTermDataKey = BiUidKey( attribs.value( "biTransUidKey" ) ); 

        bool isOk;
        tempInterval = attribs.value( "interval" ).toInt( &isOk );
        if( !isOk )
            return( false );

        tempRepetition = attribs.value( "repetition" ).toUInt( &isOk );
        if( !isOk )
            return( false );

        tempEasinessFactor = attribs.value( "easinessFactor" ).toFloat( &isOk );
        if( !isOk )
            return( false );

        tempNextRepetitionDate = Util::parseDate( attribs.value( "nextRepetitionDate" ), &isOk );
        if( !isOk )
            return( false );

        tempLastRepetitionDate = Util::parseDate( attribs.value( "lastRepetitionDate" ), &isOk );
        if( !isOk )
            return( false );

        tempSuccessCount = attribs.value( "successCount" ).toUInt( &isOk );
        if( !isOk )
            return( false );

        tempMissCount = attribs.value( "missCount" ).toUInt( &isOk );
        if( !isOk )
            return( false );

        currTermData = new TermData();
        currTermData->interval = tempInterval;
        currTermData->repetition = tempRepetition;
        currTermData->easinessFactor = tempEasinessFactor;
        currTermData->nextRepetitionDate = tempNextRepetitionDate;
        currTermData->lastRepetitionDate = tempLastRepetitionDate;
        currTermData->successCount = tempSuccessCount;
        currTermData->missCount = tempMissCount;
    }
    return( true );
}

bool StatsParser::endElement( const QString&, const QString&, const QString& qname ) {
    if( qname == QString( "termData" ) ) {
        termData[ currTermDataKey ] = *currTermData; 
        delete( currTermData );
        currTermData = NULL;
    }
    
    return( true );
}

bool StatsParser::endDocument() {
    return( true );
}

bool StatsParser::fatalError( const QXmlParseException& exception ) {
    QMessageBox::warning( 0, QObject::tr( "Error" ), 
        QObject::tr( "ParseError" )
            .arg( exception.lineNumber() )
            .arg( exception.columnNumber() )
            .arg( exception.message() ) );
    return( false );
}

bool StatsParser::isStatisticsFile() {
    return( isStatsFile );
}

